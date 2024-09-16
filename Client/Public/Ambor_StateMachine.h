#pragma once
#include "PlayerStateMachine.h"
#include "PlayerStateDefine.h"

BEGIN(Engine)
class CAttackCollider;
END

class CAmbor_StateMachine : public CPlayerStateMachine
{
public:
	explicit CAmbor_StateMachine() = default;
	virtual ~CAmbor_StateMachine() = default;

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
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta);

	// [ 투사체의 행동 방향을 결정 ]
	_float CalculateArrowRotation();
	void MakeArrow(ATTACK_DESC& _AttackDesc, const wstring& _wstrEffectName);

	// [ 이펙트  ]
	void MakeParticle_E(_vector _vOffset);
	void MakeParticle_Q(_vector _vOffset);

	void PlayNormalAttackVoice();
	void PlayHeavyAttackVoice();
	void PlayHitVoice(ATTACK_DESC _AttackDesc);
	void PlaySprintVoice();
	void PlayJumpVoice();

	void PlayElementalSkillVoice();
	void PlayElementalChargeVoice();
	void PlayElementalBurstVoice();

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
	_bool	m_bBuffed_E = false;
	_float	m_fBuff_E_TimeAcc = 0.f;
	_float	m_fBuff_E_Duration = 8.f;
};

