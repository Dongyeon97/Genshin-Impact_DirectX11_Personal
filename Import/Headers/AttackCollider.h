#pragma once
#include "ColliderComponent.h"

BEGIN(Engine)

class ENGINE_DLL CAttackCollider : public CCollider
{
public:
	explicit CAttackCollider() = default;
	virtual ~CAttackCollider() = default;

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


/********************
	Getter/Setter
********************/
public:
	// [ ATTACK_DESC ]
	void SetAttackDesc(ATTACK_DESC _AttackDesc) { m_AttackDesc = _AttackDesc; }
	ATTACK_DESC GetAttackDesc() { return m_AttackDesc; }

/********************
	Data Members
********************/
protected:
	// [ ATTACK_DESC ] -> 콜라이더에 담긴 공격정보
	ATTACK_DESC m_AttackDesc = {};
};

END