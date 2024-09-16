#pragma once
#include "MonsterStateMachine.h"

enum HILI_ICE_ANIM
{
	IDLE = 0,

	WALK_FORWARD = 1,
	WALK_LEFT = 2,
	WALK_BACK = 3,
	WALK_RIGHT = 4,

	RUN = 5,

	DODGE = 6,

	AIM = 7,
	ATTACK = 8,

	HIT1 = 9,
	HIT2 = 10,
	HIT3 = 11,
	HIT_THROW_TO_GROUND = 12,
	HIT_THROW_TO_DIE = 13
};

class CHili_Ice_StateMachine final : public CMonsterStateMachine
{
public:
	explicit CHili_Ice_StateMachine() = default;
	virtual ~CHili_Ice_StateMachine() = default;

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
	// ������Ʈ�ӽ��� �� �����Ӹ��� ������ ���¸� �����ؾ� ��.
	virtual void ManagementActorState(const _float& _fTimeDelta);

	// [ �ִԾ��ͷκ��� ���� ��û�� ���� ���� ó�� ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override;

	// [ ������ ���� ���¿� ���� ��Ÿ���� ���� ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) override;

	// [ �������� ����ü ���� ]
	void MakeProjectile(_vector _vShootDirection, ATTACK_DESC _AttackDesc);

	// [ ���� �������� ����ü ���� ]
	void MakeMultipleProjectile(_vector _vShootDirection, ATTACK_DESC _AttackDesc);

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:


};

