#pragma once
#include "MonsterStateMachine.h"

enum HILI_FIRE_ANIM
{
	IDLE = 0,

	WALK_FORWARD = 1,
	WALK_LEFT = 2,
	WALK_BACK = 3,
	WALK_RIGHT = 4,

	RUN = 5,

	DEFEND = 6,

	NORMAL_ATTACK = 7,
	RUSH_ATTACK_START = 8,
	RUSH_ATTACK_CYCLE = 9,
	RUSH_ATTACK_END = 10,

	HIT1 = 11,
	HIT2 = 12,
	HIT3 = 13,
	HIT_THROW_TO_GROUND = 14,
	HIT_THROW_TO_DIE = 15
};

class CHili_Fire_StateMachine final : public CMonsterStateMachine
{
public:
	explicit CHili_Fire_StateMachine() = default;
	virtual ~CHili_Fire_StateMachine() = default;

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

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:

};
