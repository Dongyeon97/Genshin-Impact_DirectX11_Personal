#pragma once
#include "NormalMonster.h"

BEGIN(Engine)
class CAttackCollider;
END

class CSlime_Fire final : public CNormalMonster
{
public:
	explicit CSlime_Fire() = default;
	virtual ~CSlime_Fire() = default;

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
	Data Members
********************/
private:
	// [ 공격 콜라이더 ]
	shared_ptr<CAttackCollider> m_spAttackCollider;

};

