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
	// [ 공격 콜라이더 정보를 스테이트 머신에 세팅 ]
	void AddAttackCollider(const wstring& _wstrColTag, weak_ptr<CAttackCollider> _wpAtackCollider) 
	{ 
		m_mapAttackColliders.emplace(_wstrColTag, _wpAtackCollider.lock());
	}

protected:
	// [ 몬스터가 플레이어를 바라보게 함 ]
	_float CaculateMonsterRotation(_vector _vMonsterLook, _vector _vMonsterToPlayer);

	// [ 피격 이펙트 ]
	void MakeHitEffect();

	// [ 데미지 폰트 ]
	void MakeDamageFont(ATTACK_DESC _AttackDesc, _vector _vOffset);

	// [ 원소 반응 폰트 ]
	void MakeElementalReactionFont(const wstring& _wstrReactionName, ATTACK_DESC _AttackDesc, _vector _vOffset);

	// [ 피격 데미지 판정 ]
	virtual _uint Hitted(ATTACK_DESC _AttackDesc, _float _fDamage, _vector _vOffset);

	// [ 피격 반응 산출 ]
	const wstring CalculateElementalReaction(ELEMENTAL_TYPE _Src, ATTACK_DESC _Dst);

private:
	// [ 매 프레임 액터의 상태를 관리 ]
	virtual void ManagementActorState(const _float& _fTimeDelta) {};

	// [ 요청에 대한 조건 처리 ]
	virtual void RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection = REQUEST_DIR::DIR_NONE, ATTACK_DESC _AttackDesc = {}) override {};

	// [ 현재 상태에 따른 쿨타임을 제어 ]
	virtual void ManagementActorCoolTime(const _float& _fTimeDelta) = 0;

/********************
	Getter / Setter
********************/
public:
	// [ 몬스터 정보 세팅 ]
	void SetMonsterInfo(MONSTER_INFO _tMonsterInfo) { m_tInfo = _tMonsterInfo; }

	// [ 모든 공격 콜라이더 on ]
	void SetAllAttackColliderOn();

	// [ 모든 공격 콜라이더 off ]
	void SetAllAttackColliderOff();

	// [ 공격 콜라이더 태그로 찾아서 on/off ]
	void SetAttackColliderEnable(const wstring& _wstrColTag, _bool _bEnable);

	// [ 공격 콜라이더 태그로 찾아서 ATTACK_DESC 세팅 ]
	void SetAttackDescToAttackCollider(const wstring& _wstrColTag, ATTACK_DESC _AttackDesc);

/********************
	Data Members
********************/
protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	// [ 몬스터의 정보 ]
	MONSTER_INFO m_tInfo = {};

	// [ 공격 콜라이더들 ]
	map<wstring, shared_ptr<CAttackCollider>> m_mapAttackColliders;

	// [ 초기 스폰 지점 ]
	_float3 m_vOriginSpawnPoint = {};

protected:
	// [ 몬스터 패턴 ATTACK_DESC ]
	ATTACK_DESC m_Pattern1_Desc = {};
	ATTACK_DESC m_Pattern2_Desc = {};
	ATTACK_DESC m_Pattern3_Desc = {};
	ATTACK_DESC m_Pattern4_Desc = {};
	ATTACK_DESC m_Pattern5_Desc = {};
};

