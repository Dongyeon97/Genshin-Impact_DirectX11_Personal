#pragma once
#include "Actor.h"

BEGIN(Engine)
class CNavigation;
END

class CStage3_Navi final : public CActor
{
public:
	explicit CStage3_Navi() = default;
	virtual ~CStage3_Navi() = default;

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
	weak_ptr<CNavigation> m_wpNavigation;
};

