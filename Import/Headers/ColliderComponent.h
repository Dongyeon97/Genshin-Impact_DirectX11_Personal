#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class CBounding;

class ENGINE_DLL CCollider : public CPrimitiveComponent
{
public:
	explicit CCollider() = default;
	virtual ~CCollider() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
public:
	void BindShaderResource(BasicEffect* _pEffect);

	// [ ��� �ݶ��̴��� �浹�ߴ��� üũ ]
	_bool Intersect(CCollider* pTargetCollider);

/********************
	Getter / Setter
********************/
public:
	// [ �ݶ��̴� Ÿ�� ���� ] -> �� Initialize ���� �ݵ�� ���־�� �� ��
	void SetColliderType(COLLIDER_TYPE _eColliderType) { m_eColliderType = _eColliderType; }

	// [ �ݶ��̴� ���� ���� ]
	void SetColliderCenter(_float3 _vCenter)	{ m_vCenter = _vCenter; }
	void SetColliderRadius(_float _fRadius)		{ m_fRadius = _fRadius; }
	void SetColliderExtents(_float3 _vExtents)	{ m_vExtents = _vExtents; }
	void SetColliderRadians(_float3 _vRadians)	{ m_vRadians = _vRadians; }

	// [ �浹üũ ��, �׸��� ��, ���� ��Ʈ���� ���� ] -> �ʼ�, ������ ����
	void SetOwnerWorld(_matrix _matOwnerWorld) { XMStoreFloat4x4(&m_matOwnerWorld, _matOwnerWorld); }

	// [ �ݶ��̴� on/off ]
	void SetIsCollideOn(_bool _bCollideOn) { m_bCollideOn = _bCollideOn; }
	_bool GetISCollideOn() { return m_bCollideOn; }

	// [ �� �ݶ��̴��� �浹�� �ٸ� �ݶ��̴��� ]
	unordered_set<shared_ptr<CCollider>>& GetColliderComponents() { return m_usetColliders; }

/********************
	Data Members
********************/
protected:
	// [ �ݶ��̴� Ÿ�� ]
	COLLIDER_TYPE m_eColliderType = COLLIDER_END;
	shared_ptr<CBounding> m_pBounding;

protected:
	// [ �ݶ��̴� �������� �����Ʈ���� ]
	_float4x4 m_matOwnerWorld = {};

	// [ �ݶ��̴� on/off ]
	_bool m_bCollideOn = true;

protected:
	// [ �ݶ��̴� ���� ]
	_float3 m_vCenter = {};		// Sphere / AABB / OBB
	_float	m_fRadius = 0.0f;	// Sphere
	_float3 m_vExtents = {};	// AABB / OBB
	_float3 m_vRadians = {};	// OBB

	// [ �� �ݶ�� ������Ʈ�� �浹�� �ٸ� �ݶ��̴� ������Ʈ�� ]
	unordered_set<shared_ptr<CCollider>> m_usetColliders;

private:
#pragma region �� ��ü
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };
#pragma endregion �� ��ü
};

END