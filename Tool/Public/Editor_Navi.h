#pragma once
#include "Actor.h"

BEGIN(Engine)

class CNavigation;

END

class CEditor_Navi final : public CActor
{
public:
	explicit CEditor_Navi() = default;
	virtual ~CEditor_Navi() = default;

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

public:
	shared_ptr<CNavigation> GetNavigation() { return m_wpNavigation.lock(); }

/********************
	Data Members
********************/
private:
	weak_ptr<CNavigation> m_wpNavigation;
};
