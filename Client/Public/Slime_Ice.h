#pragma once
#include "NormalMonster.h"

BEGIN(Engine)
class CAttackCollider;
END

class CSlime_Ice final : public CNormalMonster
{
public:
	explicit CSlime_Ice() = default;
	virtual ~CSlime_Ice() = default;

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
	// [ ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spAttackCollider;

};

