#pragma once
#include "Monster.h"

class CUIActor_Base;

class CBossMonster : public CMonster
{
public:
	explicit CBossMonster() = default;
	virtual ~CBossMonster() = default;

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
	HRESULT ReadyHPbar();

/********************
	Getter / Setter
********************/
public:
	void SetHPBarEnable(_bool _bEnable);

/********************
	Data Members
********************/
protected:
	vector<shared_ptr<CUIActor_Base>> m_vecHPbar;
};
