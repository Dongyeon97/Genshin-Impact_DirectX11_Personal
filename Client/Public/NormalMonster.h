#pragma once
#include "Monster.h"

class CIngameUI;

class CNormalMonster : public CMonster
{
public:
	explicit CNormalMonster() = default;
	virtual ~CNormalMonster() = default;

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
protected:
	HRESULT ReadyHPbar(_float _fOffsetY);

/********************
	Getter / Setter
********************/
public:
	void SetHPBarEnable(_bool _bEnable);

/********************
	Data Members
********************/
protected:
	vector<shared_ptr<CIngameUI>> m_vecHPbar;
};

