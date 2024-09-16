#pragma once
#include "EffectMeshActor.h"

BEGIN(Engine)

class CAttackCollider;

class ENGINE_DLL CMonsterAttackEffect : public CEffectMeshActor
{
public:
	explicit CMonsterAttackEffect() = default;
	virtual ~CMonsterAttackEffect() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render();
	virtual void EndPlay() override;
	virtual void Release() override;

	// [ JSON 파싱 용]
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ 오브젝트 풀 용 ]
	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	void SetAttackColliderType(COLLIDER_TYPE _eColliderType) { m_eColliderType = _eColliderType; }

	void SetAttackDesc(ATTACK_DESC _AttackDesc);
	ATTACK_DESC GetAttackDesc();

	void SetColliderOn(_bool _bColliderOn);
	_bool GetColliderOn();

	void SetColliderCenter(_float3 _vCenter) { m_vCenter = _vCenter; }
	void SetColliderRadius(_float _fRadius) { m_fRadius = _fRadius; }
	void SetColliderExtents(_float3 _vExtents) { m_vExtents = _vExtents; }
	void SetColliderRadians(_float3 _vRadians) { m_vRadians = _vRadians; }

/********************
	Getter / Setter
********************/
public:
	

/********************
	Data Members
********************/
protected:
	weak_ptr<CAttackCollider> m_wpAttackCollider;
	
protected:
	// [ 콜라이더 정보 ]
	COLLIDER_TYPE m_eColliderType = COLLIDER_TYPE::SPHERE;

	_float3 m_vCenter = {};		// Sphere / AABB / OBB
	_float	m_fRadius = 0.0f;	// Sphere
	_float3 m_vExtents = {};	// AABB / OBB
	_float3 m_vRadians = {};	// OBB
};

END