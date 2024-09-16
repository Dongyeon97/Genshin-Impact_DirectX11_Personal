#pragma once
#include "StateMachine.h"
#include "Monster.h"

BEGIN(Engine)
class CCharactor;
class CAttackCollider;
END

class CMonsterStateMachine : public CStateMachine
{
public:
	explicit CMonsterStateMachine() = default;
	virtual ~CMonsterStateMachine() = default;

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
	// [ ���� �ݶ��̴� ������ ������Ʈ �ӽſ� ���� ]
	void AddAttackCollider(const wstring& _wstrColTag, weak_ptr<CAttackCollider> _wpAtackCollider) 
	{ 
		m_mapAttackColliders.emplace(_wstrColTag, _wpAtackCollider.lock());
	}

protected:
	// [ ���Ͱ� �÷��̾ �ٶ󺸰� �� ]
	_float CaculateMonsterRotation(_vector _vMonsterLook, _vector _vMonsterToPlayer);

	// [ �ǰ� ����Ʈ ]
	void MakeHitEffect();

	// [ ������ ��Ʈ ]
	void MakeDamageFont(ATTACK_DESC _AttackDesc, _vector _vOffset);

	// [ ���� ���� ��Ʈ ]
	void MakeElementalReactionFont(const wstring& _wstrReactionName, ATTACK_DESC _AttackDesc, _vector _vOffset);

	// [ �ǰ� ������ ���� ]
	virtual _uint Hitted(ATTACK_DESC _AttackDesc, _float _fDamage, _vector _vOffset);

	// [ �ǰ� ���� ���� ]
	const wstring CalculateElementalReaction(ELEMENTAL_TYPE _Src, ATTACK_DESC _Dst);

private:
	// [ �� ������ ������ ���¸� ���� ]
	virtual void ManagementActorState(const _float& _fTimeDelta) {};

	// [ ��û�� ���� ���� ó�� ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override {};

	// [ ���� ���¿� ���� ��Ÿ���� ���� ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) = 0;

/********************
	Getter / Setter
********************/
public:
	// [ ���� ���� ���� ]
	void SetMonsterInfo(MONSTER_INFO _tMonsterInfo) { m_tInfo = _tMonsterInfo; }

	// [ ��� ���� �ݶ��̴� on ]
	void SetAllAttackColliderOn();

	// [ ��� ���� �ݶ��̴� off ]
	void SetAllAttackColliderOff();

	// [ ���� �ݶ��̴� �±׷� ã�Ƽ� on/off ]
	void SetAttackColliderEnable(const wstring& _wstrColTag, _bool _bEnable);

	// [ ���� �ݶ��̴� �±׷� ã�Ƽ� ATTACK_DESC ���� ]
	void SetAttackDescToAttackCollider(const wstring& _wstrColTag, ATTACK_DESC _AttackDesc);

/********************
	Data Members
********************/
protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	// [ ������ ���� ]
	MONSTER_INFO m_tInfo = {};

	// [ ���� �ݶ��̴��� ]
	map<wstring, shared_ptr<CAttackCollider>> m_mapAttackColliders;

	// [ �ʱ� ���� ���� ]
	_float3 m_vOriginSpawnPoint = {};

protected:
	// [ ���� ���� ATTACK_DESC ]
	ATTACK_DESC m_Pattern1_Desc = {};
	ATTACK_DESC m_Pattern2_Desc = {};
	ATTACK_DESC m_Pattern3_Desc = {};
	ATTACK_DESC m_Pattern4_Desc = {};
	ATTACK_DESC m_Pattern5_Desc = {};
};

