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
	// 스테이트머신은 매 프레임마다 액터의 상태를 관리해야 함.
	virtual void ManagementActorState(const _float& _fTimeDelta);

	// [ 애님액터로부터 들어온 요청에 대한 조건 처리 ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override;

	// [ 몬스터의 현재 상태에 따른 쿨타임을 제어 ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) override;

	// [ 방향으로 투사체 생성 ]
	void MakeProjectile(_vector _vShootDirection, ATTACK_DESC _AttackDesc);

	// [ 여러 방향으로 투사체 생성 ]
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

