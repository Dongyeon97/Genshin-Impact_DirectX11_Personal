#pragma once
#include "MonsterStateMachine.h"
#include "MonsterStateDefine.h"

enum DEFENDER_ANIM
{
	IDLE = 31,

	WALK_FORWARD = 62,
	WAKE_UP = 60,

	TRACE = 62,

	SWIPE = 48,
	SWIPEANDLASER = 49,
	STAMP = 46,

	WHIRL_START = 71,
	WHIRL_CYCLE = 72,
	WHIRL_END = 70,

	HIT1 = 28,
	HIT2 = 29,
	HIT3 = 30,

	DIE = 20
};

class CDefenderStateMachine final : public CMonsterStateMachine
{
public:
	explicit CDefenderStateMachine() = default;
	virtual ~CDefenderStateMachine() = default;

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
	// [ ������ ���� ���¿� ���� �������� ���� ]
	virtual void ManagementActorState(const _float& _fTimeDelta) override;

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
private:


};

