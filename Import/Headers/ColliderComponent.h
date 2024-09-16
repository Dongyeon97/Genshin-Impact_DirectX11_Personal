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

	// [ 상대 콜라이더와 충돌했는지 체크 ]
	_bool Intersect(CCollider* pTargetCollider);

/********************
	Getter / Setter
********************/
public:
	// [ 콜라이더 타입 세팅 ] -> ※ Initialize 전에 반드시 해주어야 함 ※
	void SetColliderType(COLLIDER_TYPE _eColliderType) { m_eColliderType = _eColliderType; }

	// [ 콜라이더 정보 세팅 ]
	void SetColliderCenter(_float3 _vCenter)	{ m_vCenter = _vCenter; }
	void SetColliderRadius(_float _fRadius)		{ m_fRadius = _fRadius; }
	void SetColliderExtents(_float3 _vExtents)	{ m_vExtents = _vExtents; }
	void SetColliderRadians(_float3 _vRadians)	{ m_vRadians = _vRadians; }

	// [ 충돌체크 전, 그리기 전, 월드 매트릭스 세팅 ] -> 필수, 시점에 주의
	void SetOwnerWorld(_matrix _matOwnerWorld) { XMStoreFloat4x4(&m_matOwnerWorld, _matOwnerWorld); }

	// [ 콜라이더 on/off ]
	void SetIsCollideOn(_bool _bCollideOn) { m_bCollideOn = _bCollideOn; }
	_bool GetISCollideOn() { return m_bCollideOn; }

	// [ 이 콜라이더와 충돌한 다른 콜라이더들 ]
	unordered_set<shared_ptr<CCollider>>& GetColliderComponents() { return m_usetColliders; }

/********************
	Data Members
********************/
protected:
	// [ 콜라이더 타입 ]
	COLLIDER_TYPE m_eColliderType = COLLIDER_END;
	shared_ptr<CBounding> m_pBounding;

protected:
	// [ 콜라이더 소유자의 월드매트릭스 ]
	_float4x4 m_matOwnerWorld = {};

	// [ 콜라이더 on/off ]
	_bool m_bCollideOn = true;

protected:
	// [ 콜라이더 정보 ]
	_float3 m_vCenter = {};		// Sphere / AABB / OBB
	_float	m_fRadius = 0.0f;	// Sphere
	_float3 m_vExtents = {};	// AABB / OBB
	_float3 m_vRadians = {};	// OBB

	// [ 이 콜라더 컴포넌트와 충돌한 다른 콜라이더 컴포넌트들 ]
	unordered_set<shared_ptr<CCollider>> m_usetColliders;

private:
#pragma region 컴 객체
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };
#pragma endregion 컴 객체
};

END