#pragma once
#include "MeshActor.h"

BEGIN(Engine)

class CAttackCollider;
class CProjectile;

class ENGINE_DLL CProjectileActor : public CMeshActor
{
public:
	explicit CProjectileActor() = default;
	virtual ~CProjectileActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual HRESULT BindShaderResources();

	void ProgressToDirrection(const _float& _fTimeDelta);

	_bool CheckDelete();

/********************
	Getter / Setter
********************/
public:
	shared_ptr<CAttackCollider> GetAttackCollider() { return m_wpAttackCollider.lock(); }

	// [ �ݶ��̴� ���� ���� ]
	void SetAttackColliderType(COLLIDER_TYPE _eColliderType) { m_eColliderType = _eColliderType; }
	void SetAttackDesc(ATTACK_DESC _eAttackDesc) { m_AttackDesc = _eAttackDesc; }

	void SetColliderCenter(_float3 _vCenter) { m_vCenter = _vCenter; }
	void SetColliderRadius(_float _fRadius) { m_fRadius = _fRadius; }
	void SetColliderExtents(_float3 _vExtents) { m_vExtents = _vExtents; }
	void SetColliderRadians(_float3 _vRadians) { m_vRadians = _vRadians; }

public:
	void SetShootDirrection(_vector _vShootDirrection) { XMStoreFloat3(&m_vShootDirrection, _vShootDirrection); }
	_vector GetShootDirrection() { return XMLoadFloat3(&m_vShootDirrection); }

	void SetShootSpeed(_float _fShootSpeed) { m_fShootSpeed = _fShootSpeed; }
	_float GetShootSpeed() { return m_fShootSpeed; }

	void SetAcceleration(_float _fAcceleration) { m_fAcceleration = _fAcceleration; }
	_float GetAcceleration() { return m_fAcceleration; }

	void SetLifeTime(_float _fLifeTime) { m_fLifeTime = _fLifeTime; }
	_float GetLifeTime() { return m_fLifeTime; }

	void SetIsDeleted(_bool _bDeleted) { m_bDeleted = _bDeleted; }
	_bool GetIsDeleted() { return m_bDeleted; }

/********************
	Data Members
********************/
protected:
	// [ ����ü ������Ʈ ]
	weak_ptr<CProjectile> m_wpProjectile;

	// [ ���� �ݶ��̴� ]
	weak_ptr<CAttackCollider> m_wpAttackCollider;

protected:
	// [ �߻� ���� ]
	_float3 m_vShootDirrection = {};

	// [ �߻� �ð� ]
	_float m_fShootTime = 0.0f;

	// [ �߻� �ӵ� ]
	_float m_fShootSpeed = 0.0f;

	// [ �߻� ���ӵ� ]
	_float m_fAcceleration = 1.0f;

	// [ ������ Ÿ�� ]
	_float m_fLifeTime = 0.0f;

	// [ �浹 �� ����� ���� ]
	_bool m_bDeleted = true;

protected:
	// [ �ݶ��̴� ���� ]
	COLLIDER_TYPE m_eColliderType = COLLIDER_TYPE::SPHERE;

	_float3 m_vCenter = {};		// Sphere / AABB / OBB
	_float	m_fRadius = 0.0f;	// Sphere
	_float3 m_vExtents = {};	// AABB / OBB
	_float3 m_vRadians = {};	// OBB

	ATTACK_DESC m_AttackDesc = {};
};

END