#pragma once
#include "StateMachine.h"

BEGIN(Engine)
class CAttackCollider;
END

class CPlayerStateMachine : public CStateMachine
{
public:
	explicit CPlayerStateMachine() = default;
	virtual ~CPlayerStateMachine() = default;

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
// [ ���� ���¿� ���� ��Ÿ���� ���� ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) = 0;

protected:
	// [ ���� ���¿� ���� �������� ���� ]
	virtual void ManagementActorState(const _float& _fTimeDelta) {};

	// [ ������ �ൿ ������ ���� ]
	_float CalculateActorRotation();

	// [ �ڵ� Ÿ���� ]
	_vector AutoTargetting();

	// [ �ٴڿ� ��Ҵ°�? ]
	_float IsLand();

	// [ ���� �ݶ��̴� On / Off ]
	void NormalAttackColliderOnOff(_float _fOnTime, _float _fOffTime);
	void SkillColliderOnOff(_float _fOnTime, _float _fOffTime);

protected:
	// [ �ǰ� ����Ʈ ]
	void MakeHitEffect(_vector _vOffset);

	// [ ����Ʈ  ]
	virtual void MakeParticle(const wstring& _wstrParticleName, _vector _vOffset);
	virtual void MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset);
	virtual void MakeParticle_E(_vector _vOffset) {};
	virtual void MakeParticle_Q(_vector _vOffset) {};

protected:
	// [ ���� ] 
	virtual void PlayRunSound_FootSteps() {};
	virtual void PlaySprintSound_FootSteps() {};

	void PlayFlyingStartSound();

/********************
	Getter / Setter
********************/
public:
	// [ ������Ʈ �ӽſ� �÷��̾��� ���� �ݶ��̴��� ���� ]
	void SetWeaponCollider(weak_ptr<CAttackCollider> _wpAttackCollider) { m_wpWeaponCollider = _wpAttackCollider; };
	void SetSkillCollider(weak_ptr<CAttackCollider> _wpSkillCollider) { m_wpSkillCollider = _wpSkillCollider; }

	// [ �ñر� ���� �ð� ]
	_float GetElementalBurst_RunTime() { return m_fElemental_Burst_RunTime; }

	_float2 GetSkillCoolTime_E() { return _float2(m_fElemental_Skill_RunTime, m_fElemental_Skill_CoolTime); }
	_float2 GetSkillCoolTime_Q() { return _float2(m_fElemental_Burst_RunTime, m_fElemental_Burst_CoolTime); }

/********************
	Data Members
********************/
protected:
	// [ �÷��̾� ������ ���� �ݶ��̴� ]
	weak_ptr<CAttackCollider> m_wpWeaponCollider;

	// [ �÷��̾� ��ų ���� �ݶ��̴� ]
	weak_ptr<CAttackCollider> m_wpSkillCollider;

	// [ ���Ϳ��� ��û���� �ൿ ���� ]
	REQUEST_DIR m_eDirrection = REQUEST_DIR::DIR_NONE;

	// [ ������ ���� ���� ] - ������
	_float m_fPrevAngle = 0.0f;

protected:
#pragma region �ӵ�, �ð� ���� ��ü���� ����
	/* == WALK == */
	_float m_fWalk_Speed = 0.0f;
	_float m_fTotal_Walk_Speed = 0.0f;

	/* == RUN == */
	_float m_fRun_Speed = 0.0f;

	_uint  m_iRunSoundCount = 0;
	_float m_fRunTimeAcc = 0.f;

	_float m_fRun_Start_MinTime = 0.0f;
	_float m_fRun_Start_RunTime = 0.0f;
	_float m_fRun_Stop_MinTime = 0.0f;
	_float m_fRun_Stop_RunTime = 0.0f;

	/* == SPRINT == */
	_float m_fSprint_Speed = 0.0f;

	_float m_fSprint_Start_MinTime = 0.0f;
	_float m_fSprint_Start_RunTime = 0.0f;
	_float m_fSprint_Stop_MinTime = 0.0f;
	_float m_fSprint_Stop_RunTime = 0.0f;

	/* == JUMP == */
	_float m_fJump_Power = 0.0f;
	_float m_fJump_RunTime = 0.0f;
	_float m_fJump_MinTime = 0.0f;

	_float m_fJumpYAcc = 0.0f;

	/* == CLIMB == */
	_float m_fClimb_Speed = 0.0f;
	_float m_fClimbDash_Speed = 0.0f;

	/* == FLY == */
	_float m_fFly_Speed = 0.0f;

	_float m_fFlyStart_MinTime = 0.0f;
	_float m_fFlyStart_RunTime = 0.0f;

	////////////////
	/* == ���� == */
	////////////////

	/* ATTACK */
	_float m_fAttack_ExitTime = 0.0f; // ���� ���� �� �ð��� ������ Ż�� ����
	_float m_fAttack_MinTime = 0.0f; // m_fAttack_RunTime�� ������� ������
	_float m_fAttack_RunTime = 0.0f;	// ��ŸŸ�Ӹ�ŭ �����ϸ�, RunTime�� MinTime�� �Ѿ�� �������� ���� �����̳� �ൿ�� ����.
	_float m_fAttackMove_Speed = 0.0f; // ���� �� �̵��ӵ�

	/* ELEMENTAL_SKILL */
	_uint m_iElemental_Skill_ParticleCount = 0;
	_float m_fElemental_Skill_MinTime = 0.0f;	// ��ų ����� ���� �ൿ�� �ϱ���� �ʿ��� �ּ� �ð�
	_float m_fElemental_Skill_CoolTime = 0.0f;
	_float m_fElemental_Skill_RunTime = 0.0f;	// ��ŸŸ�Ӹ�ŭ �����ϸ�, RunTime�� MinTime�� �Ѿ�� �������� ���� �����̳� �ൿ�� ����.

	/* ELEMENTAL_BURST */
	_float m_fElemental_Burst_MinTime = 0.0f;	// ��ų ����� ���� �ൿ�� �ϱ���� �ʿ��� �ּ� �ð�
	_float m_fElemental_Burst_CoolTime = 0.0f;
	_float m_fElemental_Burst_RunTime = 0.0f;	// ��ŸŸ�Ӹ�ŭ �����ϸ�, RunTime�� MinTime�� �Ѿ�� �������� ���� �����̳� �ൿ�� ����.
	_uint  m_iProductionCount = 0;	// ī�޶� ���� ī��Ʈ(������ ������ �� ��� �̰ɷ� ��Ʈ��)

	_float m_fHitH_MinTime = 0.0f;
	_float m_fHitH_RunTime = 0.0f;

	_float m_fHitL_MinTime = 0.0f;
	_float m_fHitL_RunTime = 0.0f;

	_float m_fHitThrow_MinTime = 0.0f;
	_float m_fHitThrow_RunTime = 0.0f;

	_float m_fHitFall_MinTime = 0.0f;
	_float m_fHitFall_RunTime = 0.0f;
#pragma endregion �ӵ�, �ð� ���� ��ü���� ����

	
//////////////////
/* == ���� == */
//////////////////
protected:
	_uint m_iSoundCount_SprintStart = 0;
	_uint m_iSoundCount_Jump = 0;

	// _uint m_iSoundCount_Hit_L = 0;
	// _uint m_iSoundCount_Hit_H = 0;

	_uint m_iSoundCount_LightAttack = 0;
	_uint m_iSoundCount_HeavyAttack = 0;

	_uint m_iSoundCount_ElementalSkill = 0;
	_uint m_iSoundCount_ElementalBurst = 0;

protected:
	_bool m_bAttack1_One_Completed = false;
	_bool m_bAttack1_Two_Completed = false;

	_bool m_bAttack2_One_Completed = false;
	_bool m_bAttack2_Two_Completed = false;

	_bool m_bAttack3_One_Completed = false;
	_bool m_bAttack3_Two_Completed = false;

	_bool m_bAttack4_One_Completed = false;
	_bool m_bAttack4_Two_Completed = false;

	_bool m_bAttack5_One_Completed = false;
	_bool m_bAttack5_Two_Completed = false;
};

