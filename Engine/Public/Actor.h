#pragma once
#include "Object.h"

/* CActor */
// 1. [����]�� ������ [Ʈ������ ����]�� ������ �ֽ��ϴ�.
// 2. [����]�� [������Ʈ]�� �����մϴ�.
// 3. [����]�� [������Ʈ]�� [����], [�߰�], [�˻�], [����]�մϴ�.
// 4. [����]�� ������ [������Ʈ]�� [����]�� [Ʈ������ ����]�� ���󰩴ϴ�.

BEGIN(Engine)

class CLevel;
class CLayer;
class CActorComponent;
class CCollider;

class ENGINE_DLL CActor : public CObject
{
public:
	explicit CActor() = default;
	virtual ~CActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void Render_Shadow();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual _int OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});
	virtual void OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});
	virtual void OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});

public:
	// [ ������Ʈ Ǯ���� ������ ���͸� Ȱ��ȭ �� �ÿ� �Ҹ� �Լ� ]
	virtual void RecycleBegin() {};

	// [ ī�޶���� �Ÿ� ��� ] -> ������ ���ÿ�
	_float CalculateDistanceFromCam();

	// [ ȣ�� �������� ��� �����Ʈ������ ����ؼ� ���� ]
	_matrix CalculateWorldMatrix_Immediately();

	// [ �ڡڡ� LookAt �ڡڡ� ]
	void LookAt(_vector _vPoint);

	// [ ���� �� ������Ʈ �˻� ]
	shared_ptr<CActor> FindActor(const wstring& _wstrActorName);
	shared_ptr<CActorComponent> FindActorComponent(const wstring& _wstrActorComponent);

	// [ ���Ϳ� �ڽ� ���� �߰� ]
	template<typename T, typename = enable_if_t<is_base_of_v<CActor, T>>>
	shared_ptr<T> CreateActor(const wstring& _wstrActorName)
	{
		return AddActor<T>(_wstrActorName);
	}

	// [ ���Ϳ� �ڽ� ������Ʈ �߰� ]
	template<typename T, typename = enable_if_t<is_base_of_v<CActorComponent, T>>>
	shared_ptr<T> CreateActorComponent(const wstring& _wstrActorComponent)
	{
		return AddActorComponent<T>(_wstrActorComponent);
	}

private:
	template<typename T>
	shared_ptr<T> AddActor(const wstring& _wstrActorName)
	{
		if (m_umapActors.find(_wstrActorName) != m_umapActors.end()) { return nullptr; }

		std::shared_ptr<T> spActor = make_shared<T>();
		spActor->SetLevel(m_wpLevel);
		spActor->SetLayer(m_wpLayer);
		spActor->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
		spActor->SetActorName(_wstrActorName);
		spActor->SetActorUse(true);

		m_umapActors.insert({ _wstrActorName, spActor });
		return spActor;
	}

	template<typename T>
	shared_ptr<T> AddActorComponent(const wstring& _wstrActorComponent)
	{
		if (m_umapActorComponents.find(_wstrActorComponent) != m_umapActorComponents.end()) { return nullptr; }

		shared_ptr<T> spActorComponent = make_shared<T>();
		spActorComponent->SetLevel(m_wpLevel);
		spActorComponent->SetLayer(m_wpLayer);
		spActorComponent->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
		spActorComponent->SetComponentName(_wstrActorComponent);

		m_umapActorComponents.insert({ _wstrActorComponent, spActorComponent });
		return spActorComponent;
	}

protected:
	virtual void UpdateActorTransform();

	// [ ���̴��� �� ���ε� ]
	virtual HRESULT BindShaderResources() { return S_OK; };

/********************
	Getter/Setter
********************/
public:
	// [ ���Ͱ� ���� ���� ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	weak_ptr<CLevel> GetLevel() const { assert(!m_wpLevel.expired()); return m_wpLevel; }

	// [ ���Ͱ� ���� ���̾� ]
	void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	weak_ptr<CLayer> GetLayer() const { assert(!m_wpLayer.expired()); return m_wpLayer; }

	// [ ������ ���� ���� ]
	void SetOwnerActor(weak_ptr<CActor> _wpActor) { m_wpOwnerActor = _wpActor; }
	weak_ptr<CActor> GetOwnerActor() const { assert(!m_wpOwnerActor.expired()); return m_wpOwnerActor; }

	// [ ���� �̸� ]
	void SetActorName(const wstring _wstrActorName) { m_wstrActorName = _wstrActorName; }
	const wstring GetActorName() { return m_wstrActorName; }

	// [ ���� �׷� ]
	void SetRenderGroup(RENDER_GROUP _eRenderGroup) { m_eRenderGroup = _eRenderGroup; }
	RENDER_GROUP GetRenderGroup() { return m_eRenderGroup; }

public:
	// [ ���� ��� ]
	void SetWorldMatrix(_matrix _matWorld) { XMStoreFloat4x4(&m_matWorld, _matWorld); }
	_matrix GetWorldMatrix() { return XMLoadFloat4x4(&m_matWorld); }

	// [ Right, Up, Look, Position ]
	_vector GetRightVector() { return XMLoadFloat4x4(&m_matWorld).r[0]; }
	_vector GetUpVector() { return XMLoadFloat4x4(&m_matWorld).r[1]; }
	_vector GetLookVector() { return XMLoadFloat4x4(&m_matWorld).r[2]; }
	_vector GetPositionVector() { return XMLoadFloat4x4(&m_matWorld).r[3]; }

	const _matrix GetTotalRotationMatrix() { return XMLoadFloat4x4(&m_matTotalRotation); }

	// [ Scale ]
	void SetScale(_vector _vScale) { XMStoreFloat4(&m_vScale, _vScale); }
	_vector GetScale() { return XMLoadFloat4(&m_vScale); }

	// [ Rotation ]
	void SetRotation(_vector _vRotation) { XMStoreFloat4(&m_vRotation, _vRotation); }
	_vector GetRotation() { return XMLoadFloat4(&m_vRotation); }

	void SetAdditionalRotationMatrix(_matrix _matAdditionalRotation) { XMStoreFloat4x4(&m_matAdditionalRotation, _matAdditionalRotation); }
	_matrix GetAdditionalRotationMatrix() { return XMLoadFloat4x4(&m_matAdditionalRotation); }

	void SetMouseRotateMatrix(const _matrix _matMouseRotate) { XMStoreFloat4x4(&m_matMouseRotate, _matMouseRotate); }
	const _matrix GetMouseRotateMatrix() const { return XMLoadFloat4x4(&m_matMouseRotate); }

	// [ Translation ]
	virtual void SetActorTranslation(_fvector _vPosition, _int* _iCurNaviCellIndex); // -> �÷��̾��� ����, Ȱ�� ������ ���� �����Լ�ȭ ��, �÷��̾ �������̵� �Ǿ�����
	void SetActorTranslation_NoNavigation(_fvector _vPosition) { XMStoreFloat4(&m_vTranslate, _vPosition); }
	_vector GetActorTranslation() { return XMLoadFloat4(&m_vTranslate); }

	// [ �׺� �� �ε��� ]
	void SetCurNaviCellIndex(_int _iIndex) { m_iCurNaviCellIndex = _iIndex; }
	_int& GetCurNaviCellIndex() { return m_iCurNaviCellIndex; }

	// [ ���� Ȱ��ȭ ] -> ������Ʈ Ǯ���� Ȱ���� ����
	void SetActorUse(_bool _bUse) { m_bUse = _bUse; }
	_bool GetActorUse() { return m_bUse; }

/********************
	Data Members
********************/
protected:
	// [ ����̽� ]
	ID3D11Device* m_pDevice = { nullptr };

	// [ ����̽� ���ؽ�Ʈ ]
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ ���Ͱ� ���� ���� ]
	weak_ptr<CLevel> m_wpLevel;

	// [ ���Ͱ� ���� ���̾� ]
	weak_ptr<CLayer> m_wpLayer;

	// [ ������ ���� ���� ]
	weak_ptr<CActor> m_wpOwnerActor;

	// [ ���� �̸� ]
	wstring m_wstrActorName = L"";

	// [ ���� �׷� ]
	RENDER_GROUP m_eRenderGroup = RENDER_NONBLEND;

	// [ ���� Ȱ��ȭ ]
	_bool m_bUse = true;

protected:
	// ������ ���� ���
	_float4x4 m_matLocal = {};

	// ������ ���� ���
	_float4x4 m_matWorld = {};

	// ������ ũ��
	_float4 m_vScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// ������ ȸ��
	_float4 m_vRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

	// ��Ÿ ȸ�� ���
	_float4x4 m_matAdditionalRotation = {};

	// ���콺�� ȸ�� ���
	_float4x4 m_matMouseRotate = {};

	// ���� ȸ�� ���
	_float4x4 m_matTotalRotation = {};

	// ������ �̵�, ������ ���� ��ǥ
	_float4 m_vTranslate = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ������ ���� �׺���̼� �� �ε���
	_int m_iCurNaviCellIndex = -1;

protected:
	// [ ���Ͱ� ������ ���� ]
	unordered_map<wstring, shared_ptr<CActor>> m_umapActors;

	// [ ���Ͱ� ������ ������Ʈ ]
	unordered_map<wstring, shared_ptr<CActorComponent>> m_umapActorComponents;
};

END