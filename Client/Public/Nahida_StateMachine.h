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
	// [ �ִԾ��ͷκ��� ���� ��û�� ���� ���� ó�� ]
	virtual void RequestChangeState(_ullong _iPlayerStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override;

private:
	// [ ���� ���¿� ���� �������� ���� ]
	virtual void ManagementActorState(const _float& _fTimeDelta) override;

	// [ ���� ���¿� ���� ��Ÿ���� ���� ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta);

	// [ ����ü�� �ൿ ������ ���� ]
	_float CalculateArrowRotation();
	// void MakeArrow(ATTACK_DESC& _AttackDesc, const wstring& _wstrEffectName);

	// [ ����Ʈ  ]
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

