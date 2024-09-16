#pragma once
#include "PlayerStateMachine.h"
#include "PlayerStateDefine.h"

class CEula_StateMachine : public CPlayerStateMachine
{
public:
	explicit CEula_StateMachine() = default;
	virtual ~CEula_StateMachine() = default;

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
	// [ 애님액터로부터 들어온 요청에 대한 조건 처리 ]
	virtual void RequestChangeState(_ullong _iPlayerStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override;

private:
	// [ 현재 상태에 따른 움직임을 제어 ]
	virtual void ManagementActorState(const _float& _fTimeDelta) override;

	// [ 현재 상태에 따른 쿨타임을 제어 ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) override;

	// [ 이펙트  ]
	virtual void MakeParticle_E(_vector _vOffset);
	virtual void MakeParticle_Q(_vector _vOffset);


private:
	// [ 보이스 ]
	void PlayNormalAttackVoice();
	void PlayHeavyAttackVoice();
	void PlayHitVoice(ATTACK_DESC _AttackDesc);
	void PlaySprintVoice();
	void PlayJumpVoice();

	void PlayElementalSkillVoice();
	void PlayElementalChargeVoice();
	void PlayElementalBurstVoice();

	// [ 효과음 ]
	// void PlayNormalAttackSound();

private:
	virtual void PlayRunSound_FootSteps() override;
	virtual void PlaySprintSound_FootSteps() override;

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:

};

