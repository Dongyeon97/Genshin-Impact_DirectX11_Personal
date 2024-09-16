#pragma once
#include "PlayerStateMachine.h"
#include "PlayerStateDefine.h"

BEGIN(Engine)
class CAttackCollider;
END

class CNahida_StateMachine : public CPlayerStateMachine
{
public:
	explicit CNahida_StateMachine() = default;
	virtual ~CNahida_StateMachine() = default;

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
	// void MakeArrow(ATTACK_DESC& _AttackDesc, const wstring& _wstrEffectName);

	// [ 이펙트  ]
	void MakeNormalAttackEffect_1(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook);
	void MakeNormalAttackEffect_2(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook);
	void MakeNormalAttackEffect_3(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook);
	void MakeNormalAttackEffect_4(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook);

	void MakeCaptureEffect(const wstring& _wstrEffectMeshName, _vector _vOffset);
	void MakeEffect_LifeTime(const wstring& _wstrEffectMeshName, _vector _vOffset, _float2 _vLifeTime);
	void MakeParticle_LifeTime(const wstring& _wstrParticleName, _vector _vOffset, _float2 _vLifeTime);

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

private:
	void RemoveCaptureEffect();

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:
	_float m_fElemental_Charge_MinTime = 0.8f;
	_float m_fElemental_Charge_CoolTime = 6.0f;
	_float m_fElemental_Charge_RunTime = 0.0f;

	_float m_fElemental_Charge_EndRunTime = 0.0f;
	_float m_fElemental_Charge_EndMinTime = 2.0f;

	_float m_fOriginRadius = 0.f;

	_float m_fElementalHoldTimeAcc = 0.f;
};

