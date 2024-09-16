#pragma once
#include "NormalMonster.h"

BEGIN(Engine)
class CAttackCollider;
END

class CDefenderStateMachine;

class CDefender final : public CNormalMonster
{
public:
	explicit CDefender() = default;
	virtual ~CDefender() = default;

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
private:
	void ReadyBoneMatrixToArm();
	void UpdateArmColliderTransform();

/********************
	Data Members
********************/
private:
	// [ 왼팔 공격 콜라이더 ]
	shared_ptr<CAttackCollider> m_spLeftArm_AttackCollider;

	// [ 오른팔 공격 콜라이더 ]
	shared_ptr<CAttackCollider> m_spRightArm_AttackCollider;

	// [ 발 공격 콜라이더 ]
	shared_ptr<CAttackCollider> m_spLeg_AttackCollider;

	// [ 왼팔 뼈 ]
	const _float4x4* m_pSocketMatrix_LeftArm = nullptr;
	// [ 오른팔 뼈 ]
	const _float4x4* m_pSocketMatrix_RightArm = nullptr;

	// [ 왼팔 월드행렬 ]
	_float4x4 m_matLeftArm = {};

	// [ 오른팔 월드행렬 ]
	_float4x4 m_matRightArm = {};

private:
	// [ 몬스터 상태 플래그 ]
	_ullong m_State = MONSTER_STATE::IDLE;

};