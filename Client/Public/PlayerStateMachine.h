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
// [ 현재 상태에 따른 쿨타임을 제어 ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) = 0;

protected:
	// [ 현재 상태에 따른 움직임을 제어 ]
	virtual void ManagementActorState(const _float& _fTimeDelta) {};

	// [ 액터의 행동 방향을 결정 ]
	_float CalculateActorRotation();

	// [ 자동 타겟팅 ]
	_vector AutoTargetting();

	// [ 바닥에 닿았는가? ]
	_float IsLand();

	// [ 무기 콜라이더 On / Off ]
	void NormalAttackColliderOnOff(_float _fOnTime, _float _fOffTime);
	void SkillColliderOnOff(_float _fOnTime, _float _fOffTime);

protected:
	// [ 피격 이펙트 ]
	void MakeHitEffect(_vector _vOffset);

	// [ 이펙트  ]
	virtual void MakeParticle(const wstring& _wstrParticleName, _vector _vOffset);
	virtual void MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset);
	virtual void MakeParticle_E(_vector _vOffset) {};
	virtual void MakeParticle_Q(_vector _vOffset) {};

protected:
	// [ 사운드 ] 
	virtual void PlayRunSound_FootSteps() {};
	virtual void PlaySprintSound_FootSteps() {};

	void PlayFlyingStartSound();

/********************
	Getter / Setter
********************/
public:
	// [ 스테이트 머신에 플레이어의 공격 콜라이더를 세팅 ]
	void SetWeaponCollider(weak_ptr<CAttackCollider> _wpAttackCollider) { m_wpWeaponCollider = _wpAttackCollider; };
	void SetSkillCollider(weak_ptr<CAttackCollider> _wpSkillCollider) { m_wpSkillCollider = _wpSkillCollider; }

	// [ 궁극기 시전 시간 ]
	_float GetElementalBurst_RunTime() { return m_fElemental_Burst_RunTime; }

	_float2 GetSkillCoolTime_E() { return _float2(m_fElemental_Skill_RunTime, m_fElemental_Skill_CoolTime); }
	_float2 GetSkillCoolTime_Q() { return _float2(m_fElemental_Burst_RunTime, m_fElemental_Burst_CoolTime); }

/********************
	Data Members
********************/
protected:
	// [ 플레이어 무기의 공격 콜라이더 ]
	weak_ptr<CAttackCollider> m_wpWeaponCollider;

	// [ 플레이어 스킬 공격 콜라이더 ]
	weak_ptr<CAttackCollider> m_wpSkillCollider;

	// [ 액터에게 요청받은 행동 방향 ]
	REQUEST_DIR m_eDirrection = REQUEST_DIR::DIR_NONE;

	// [ 액터의 이전 방향 ] - 보간용
	_float m_fPrevAngle = 0.0f;

protected:
#pragma region 속도, 시간 등의 구체적인 정보
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
	/* == 전투 == */
	////////////////

	/* ATTACK */
	_float m_fAttack_ExitTime = 0.0f; // 공격 도중 이 시간이 지나야 탈출 가능
	_float m_fAttack_MinTime = 0.0f; // m_fAttack_RunTime이 여기까지 누적됨
	_float m_fAttack_RunTime = 0.0f;	// 델타타임만큼 누적하며, RunTime이 MinTime을 넘어가는 순간부터 다음 공격이나 행동이 가능.
	_float m_fAttackMove_Speed = 0.0f; // 공격 중 이동속도

	/* ELEMENTAL_SKILL */
	_uint m_iElemental_Skill_ParticleCount = 0;
	_float m_fElemental_Skill_MinTime = 0.0f;	// 스킬 사용후 다음 행동을 하기까지 필요한 최소 시간
	_float m_fElemental_Skill_CoolTime = 0.0f;
	_float m_fElemental_Skill_RunTime = 0.0f;	// 델타타임만큼 누적하며, RunTime이 MinTime을 넘어가는 순간부터 다음 공격이나 행동이 가능.

	/* ELEMENTAL_BURST */
	_float m_fElemental_Burst_MinTime = 0.0f;	// 스킬 사용후 다음 행동을 하기까지 필요한 최소 시간
	_float m_fElemental_Burst_CoolTime = 0.0f;
	_float m_fElemental_Burst_RunTime = 0.0f;	// 델타타임만큼 누적하며, RunTime이 MinTime을 넘어가는 순간부터 다음 공격이나 행동이 가능.
	_uint  m_iProductionCount = 0;	// 카메라 연출 카운트(연출이 여러번 들어갈 경우 이걸로 컨트롤)

	_float m_fHitH_MinTime = 0.0f;
	_float m_fHitH_RunTime = 0.0f;

	_float m_fHitL_MinTime = 0.0f;
	_float m_fHitL_RunTime = 0.0f;

	_float m_fHitThrow_MinTime = 0.0f;
	_float m_fHitThrow_RunTime = 0.0f;

	_float m_fHitFall_MinTime = 0.0f;
	_float m_fHitFall_RunTime = 0.0f;
#pragma endregion 속도, 시간 등의 구체적인 정보

	
//////////////////
/* == 사운드 == */
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

