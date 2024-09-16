#pragma once
#include "ActorComponent.h"

/* StateMachine�� 'AnimActor'�� ���¸� �����ϰ� �ִϸ��̼��� �����մϴ�. */
/* ��ӹ޴� ������Ʈ �ӽŵ��� ���������� ����� �� �ֵ���, _uint �÷��׸� ����մϴ�. */

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
	// [ �� ������ ������ ���¸� ���� ]
	virtual void ManagementActorState(const _float& _fTimeDelta) = 0;

	// [ ��û�� ���� ���� ó�� ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) = 0;

	// [ �ǰ� ������ ���� ]
	virtual _uint Hitted(ATTACK_DESC _AttackDesc, _float _fDamage);

/********************
	Getter/Setter
********************/
public:
	// [ �ִ� �� ���� ]
	void SetAnimModel(shared_ptr<CAnimModel> _spAnimModel) { m_wpAnimModel = _spAnimModel; }

	// [ ���� ���� ]
	void SetCurState(_ullong _iCurState) { m_iCurrState = _iCurState; }
	_ullong GetCurState() { return m_iCurrState; };

/********************
	Data Members
********************/
protected:
	// [ ���� Actor�� AnimModel ] -> ù ������ ���� �ʼ�
	weak_ptr<CAnimModel> m_wpAnimModel;

	// [ ���� ���� ]
	_ullong m_iCurrState = 0; // STANDBY�� IDLE�� 0���� �����մϴ�.

	// [ ��û���� �ൿ ���� ]
	REQUEST_DIR m_eDirrection = REQUEST_DIR::DIR_NONE;

	// [ ���� ������ ���� ]
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

	// [ �ǰ� ���� ATTACK_DESC ]
	ATTACK_DESC m_HittedAttack_Desc = {};

	// [ ���� �ڽſ��� �����ִ� ���� ]
	queue<ELEMENTAL_TYPE> m_queElemental;
};

END