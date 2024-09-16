#pragma once
#include "MonsterStateMachine.h"
#include "MonsterStateDefine.h"

enum ANDREUS_ANIM
{
	BATTLE_IDLE01,
	BATTLE_IDLE02,
	BATTLE_IDLE03,
	
	CLAW_L,
	CLAW_R,

	DRIFT_L,
	DRIFT_R,

	FIRE, // 아무것도 아님

	FLAP_L,
	FLAP_R,

	HITSHAKE_CLAVICLE_L, // 아무것도 아님
	HITSHAKE_CLAVICLE_R,

	HITSHAKE_HEAD,
	HITSHAKE_SPINE,
	HITSHAKE_THIGH_L,
	HITSHAKE_THIGH_R,

	JUMP,
	JUMP_BACK,
	JUMP_BS,
	JUMP_BS_L,
	JUMP_BS_R,

	RUN,
	RUN_BS,
	
	SCRAM_L,
	SCRAM_R,

	SMASH_L,
	SMASH_R,

	STAMP,
	STAMP_LONG,

	STANDBY,
	
	SWEEP_L,
	SWEEP_R,

	TURN_L,
	TURN_R,

	WALK,
	WALK_TO_R,
	WALK_TO_L,

	HOWL,
	HOWL_AS,
	HOWL_BS,

	LOOK_DOWN,
	LOOK_FORWARD,
	LOOK_LEFT,
	LOOK_RIGHT,
	LOOK_UP,

	SHAKE,
	SHAKE_AS,
	SHAKE_BS
};

class CAndreusStateMachine final : public CMonsterStateMachine
{
public:
	explicit CAndreusStateMachine() = default;
	virtual ~CAndreusStateMachine() = default;

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
#pragma region Default
	// [ 몬스터의 현재 상태에 따른 움직임을 제어 ]
	virtual void ManagementActorState(const _float& _fTimeDelta) override;

	// [ 애님액터로부터 들어온 요청에 대한 조건 처리 ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override;

	// [ 몬스터의 현재 상태에 따른 쿨타임을 제어 ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) override;
#pragma endregion Default

private:
	// [ 페이즈 전환 연출 ]
	void ChangePaize(_uint _iPaizeIndex, _vector _vCamStartPos, _vector _vCamEndPos, _vector _vCamStartAt, _gvector _vCamEndAt, const _float& _fPaizeProductionTime);
		HRESULT DoPattern_Paize1(_float _fMonsterToPlayerDistance);
		HRESULT DoPattern_Paize2(_float _fMonsterToPlayerDistance);
		HRESULT DoPattern_Paize3(_float _fMonsterToPlayerDistance);

	// [ 랜덤한 방향벡터 생성 ]
	_vector MakeRandomDir(_float2 _vWidth, _float2 _vHeight, _float2 _vDepth);
	_float	MakeRandomRadius(_float2 _vRadiusRange);

private:
	// [ 파티클  ]
	void MakeParticle(const wstring& _wstrParticleName, _vector _vOffset);
		void MakeParticle_Attack1(_vector _vOffset);						// 공격1 - 할퀴기
		void MakeParticle_Attack2(_vector _vOffset);						// 공격2 - 꼬리치기
		void MakeParticle_Attack3(_vector _vOffset, _float2 _vLifeTime);	// 패턴3 - 아이스 웨이브

	// [ 이펙트  ]
	void MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset);
		void MakeDebris(const wstring& _wstrEffectMeshName, _vector _vOffset, _float2 _vLifeTime);		// 공격3, 패턴2 - 파편
		void MakeIceWave(const wstring& _wstrEffectMeshName, _vector _vMonLook, _vector _vMonRight);	// 패턴3 - 아이스 웨이브
		void MakeWindBlade(const wstring& _wstrEffectMeshName);											// 패턴2 - 윈드 블레이드

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

private:
	_uint	m_iPaize = 0;
	_bool	m_bProduction = false;
	_bool	m_bHit;
	_float	m_fHitTimeAcc = 0.f;

	// [ Attack3 - 뛰어올라 내려찍기 목표지점, 거리 ]
	_float4 m_vTargetPos = {};
	_float m_fJump_Attack_Distance = 0.0f;

	// [ Attack4 - 백스탭 시간 ]
	_float m_fJump_Back_RunTime = 0.0f;
	_float m_fJump_Back_MinTime = 1.0f;

	// [ 등장 씬 연출 시간 ]
	_float m_fDetectingProductionTime = 6.0f;
	_float m_fDetectingProductionRunTime = 0.0f;

	// [ 페이즈1 연출 시간 ]
	_float m_fPaize1ProductionTime = 6.0f;
	_float m_fPaize1ProductionRunTime = 0.0f;

	// [ 페이즈2 연출 시간 ]
	_float m_fPaize2ProductionTime = 6.0f;
	_float m_fPaize2ProductionRunTime = 0.0f;

	// [ 페이즈3 연출 시간 ]
	_float m_fPaize3ProductionTime = 6.0f;
	_float m_fPaize3ProductionRunTime = 0.0f;

	// [ 걷는 소리 카운트 ]
	_uint m_iWalkSoundCount = 0;

	_bool m_bAttack_Turn_SoundCompleted = false;
	_bool m_bAttack_Jump_SoundCompleted = false;
};

