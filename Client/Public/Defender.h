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
	// [ ���� ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spLeftArm_AttackCollider;

	// [ ������ ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spRightArm_AttackCollider;

	// [ �� ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spLeg_AttackCollider;

	// [ ���� �� ]
	const _float4x4* m_pSocketMatrix_LeftArm = nullptr;
	// [ ������ �� ]
	const _float4x4* m_pSocketMatrix_RightArm = nullptr;

	// [ ���� ������� ]
	_float4x4 m_matLeftArm = {};

	// [ ������ ������� ]
	_float4x4 m_matRightArm = {};

private:
	// [ ���� ���� �÷��� ]
	_ullong m_State = MONSTER_STATE::IDLE;

};