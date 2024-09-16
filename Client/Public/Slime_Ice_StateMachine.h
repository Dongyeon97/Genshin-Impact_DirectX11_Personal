#pragma once
#include "MonsterStateMachine.h"
#include "MonsterStateDefine.h"

enum SLIME_ICE_ANIM
{
	IDLE = 11,

	WALK_FORWARD = 4,

	NORMALATTACK = 2,
	CHARGE = 1,
	ATTACK_UNDER = 14,
	COMPRESS_ATTACK = 16,

	JUMP_ON_FIRE = 13,
	TURN = 7,

	HIT_H = 0,
	HIT_L = 3,
	HIT_THROW = 10,
	HIT_THROW_DROP = 8,
	HIT_THROW_TO_GROUND = 5,
	
	BORN = 12,

	DIE1 = 9,
	DIE2 = 15,
};

class CSlime_Ice_StateMachine final : public CMonsterStateMachine
{
public:
	explicit CSlime_Ice_StateMachine() = default;
	virtual ~CSlime_Ice_StateMachine() = default;

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

