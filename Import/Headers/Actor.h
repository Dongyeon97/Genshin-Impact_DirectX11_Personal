#pragma once
#include "Object.h"

/* CActor */
// 1. [액터]는 고유의 [트랜스폼 정보]를 가지고 있습니다.
// 2. [액터]는 [컴포넌트]를 소유합니다.
// 3. [액터]는 [컴포넌트]를 [생성], [추가], [검색], [삭제]합니다.
// 4. [액터]가 소유한 [컴포넌트]는 [액터]의 [트랜스폼 정보]를 따라갑니다.

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
	// [ 오브젝트 풀에서 꺼내어 액터를 활성화 할 시에 불릴 함수 ]
	virtual void RecycleBegin() {};

	// [ 카메라와의 거리 계산 ] -> 렌더러 소팅용
	_float CalculateDistanceFromCam();

	// [ 호출 시점에서 즉시 월드매트릭스를 계산해서 리턴 ]
	_matrix CalculateWorldMatrix_Immediately();

	// [ ★★★ LookAt ★★★ ]
	void LookAt(_vector _vPoint);

	// [ 액터 및 컴포넌트 검색 ]
	shared_ptr<CActor> FindActor(const wstring& _wstrActorName);
	shared_ptr<CActorComponent> FindActorComponent(const wstring& _wstrActorComponent);

	// [ 액터에 자식 액터 추가 ]
	template<typename T, typename = enable_if_t<is_base_of_v<CActor, T>>>
	shared_ptr<T> CreateActor(const wstring& _wstrActorName)
	{
		return AddActor<T>(_wstrActorName);
	}

	// [ 액터에 자식 컴포넌트 추가 ]
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

	// [ 셰이더에 값 바인딩 ]
	virtual HRESULT BindShaderResources() { return S_OK; };

/********************
	Getter/Setter
********************/
public:
	// [ 액터가 속한 레벨 ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	weak_ptr<CLevel> GetLevel() const { assert(!m_wpLevel.expired()); return m_wpLevel; }

	// [ 액터가 속한 레이어 ]
	void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	weak_ptr<CLayer> GetLayer() const { assert(!m_wpLayer.expired()); return m_wpLayer; }

	// [ 액터의 주인 액터 ]
	void SetOwnerActor(weak_ptr<CActor> _wpActor) { m_wpOwnerActor = _wpActor; }
	weak_ptr<CActor> GetOwnerActor() const { assert(!m_wpOwnerActor.expired()); return m_wpOwnerActor; }

	// [ 액터 이름 ]
	void SetActorName(const wstring _wstrActorName) { m_wstrActorName = _wstrActorName; }
	const wstring GetActorName() { return m_wstrActorName; }

	// [ 렌더 그룹 ]
	void SetRenderGroup(RENDER_GROUP _eRenderGroup) { m_eRenderGroup = _eRenderGroup; }
	RENDER_GROUP GetRenderGroup() { return m_eRenderGroup; }

public:
	// [ 월드 행렬 ]
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
	virtual void SetActorTranslation(_fvector _vPosition, _int* _iCurNaviCellIndex); // -> 플레이어의 점프, 활공 판정을 위해 가상함수화 함, 플레이어에 오버라이딩 되어있음
	void SetActorTranslation_NoNavigation(_fvector _vPosition) { XMStoreFloat4(&m_vTranslate, _vPosition); }
	_vector GetActorTranslation() { return XMLoadFloat4(&m_vTranslate); }

	// [ 네비 셀 인덱스 ]
	void SetCurNaviCellIndex(_int _iIndex) { m_iCurNaviCellIndex = _iIndex; }
	_int& GetCurNaviCellIndex() { return m_iCurNaviCellIndex; }

	// [ 액터 활성화 ] -> 오브젝트 풀에서 활용할 예정
	void SetActorUse(_bool _bUse) { m_bUse = _bUse; }
	_bool GetActorUse() { return m_bUse; }

/********************
	Data Members
********************/
protected:
	// [ 디바이스 ]
	ID3D11Device* m_pDevice = { nullptr };

	// [ 디바이스 컨텍스트 ]
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ 액터가 속한 레벨 ]
	weak_ptr<CLevel> m_wpLevel;

	// [ 액터가 속한 레이어 ]
	weak_ptr<CLayer> m_wpLayer;

	// [ 액터의 주인 액터 ]
	weak_ptr<CActor> m_wpOwnerActor;

	// [ 액터 이름 ]
	wstring m_wstrActorName = L"";

	// [ 렌더 그룹 ]
	RENDER_GROUP m_eRenderGroup = RENDER_NONBLEND;

	// [ 액터 활성화 ]
	_bool m_bUse = true;

protected:
	// 액터의 로컬 행렬
	_float4x4 m_matLocal = {};

	// 액터의 월드 행렬
	_float4x4 m_matWorld = {};

	// 액터의 크기
	_float4 m_vScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// 액터의 회전
	_float4 m_vRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 기타 회전 행렬
	_float4x4 m_matAdditionalRotation = {};

	// 마우스의 회전 행렬
	_float4x4 m_matMouseRotate = {};

	// 최종 회전 행렬
	_float4x4 m_matTotalRotation = {};

	// 액터의 이동, 액터의 로컬 좌표
	_float4 m_vTranslate = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 액터의 현재 네비게이션 셀 인덱스
	_int m_iCurNaviCellIndex = -1;

protected:
	// [ 액터가 소유한 액터 ]
	unordered_map<wstring, shared_ptr<CActor>> m_umapActors;

	// [ 액터가 소유한 컴포넌트 ]
	unordered_map<wstring, shared_ptr<CActorComponent>> m_umapActorComponents;
};

END