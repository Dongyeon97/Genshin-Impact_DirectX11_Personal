#pragma once
#include "ActorComponent.h"

/* StateMachine은 'AnimActor'의 상태를 제어하고 애니메이션을 변경합니다. */
/* 상속받는 스테이트 머신들이 범용적으로 사용할 수 있도록, _uint 플래그를 사용합니다. */

BEGIN(Engine)

enum REQUEST_DIR
{
	FOWARD,
	FOWARD_LEFT,
	LEFT,
	BACK_LEFT,
	BACK,
	BACK_RIGHT,
	RIGHT,
	FOWARD_RIGHT,
	DIR_NONE
};

class CAnimModel;

class ENGINE_DLL CStateMachine abstract : public CActorComponent
{
public:
	explicit CStateMachine() = default;
	virtual ~CStateMachine() = default;

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
	// [ 매 프레임 액터의 상태를 관리 ]
	virtual void ManagementActorState(const _float& _fTimeDelta) = 0;

	// [ 요청에 대한 조건 처리 ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) = 0;

	// [ 피격 데미지 판정 ]
	virtual _uint Hitted(ATTACK_DESC _AttackDesc, _float _fDamage);

/********************
	Getter/Setter
********************/
public:
	// [ 애님 모델 지정 ]
	void SetAnimModel(shared_ptr<CAnimModel> _spAnimModel) { m_wpAnimModel = _spAnimModel; }

	// [ 현재 상태 ]
	void SetCurState(_ullong _iCurState) { m_iCurrState = _iCurState; }
	_ullong GetCurState() { return m_iCurrState; };

/********************
	Data Members
********************/
protected:
	// [ 주인 Actor의 AnimModel ] -> 첫 생성시 지정 필수
	weak_ptr<CAnimModel> m_wpAnimModel;

	// [ 현재 상태 ]
	_ullong m_iCurrState = 0; // STANDBY나 IDLE은 0으로 고정합니다.

	// [ 요청받은 행동 방향 ]
	REQUEST_DIR m_eDirrection = REQUEST_DIR::DIR_NONE;

	// [ 이전 프레임 방향 ]
	_float m_fPrevAngle = 0.0f;

protected:
	// [ ATTACK_DESC ]
	ATTACK_DESC m_Attack1_Desc = {};
	ATTACK_DESC m_Attack2_Desc = {};
	ATTACK_DESC m_Attack3_Desc = {};
	ATTACK_DESC m_Attack4_Desc = {};
	ATTACK_DESC m_Attack5_Desc = {};

	ATTACK_DESC m_Skill1_Desc = {};
	ATTACK_DESC m_Skill2_Desc = {};

	// [ 피격 받은 ATTACK_DESC ]
	ATTACK_DESC m_HittedAttack_Desc = {};

	// [ 현재 자신에게 뭍어있는 원소 ]
	queue<ELEMENTAL_TYPE> m_queElemental;
};

END