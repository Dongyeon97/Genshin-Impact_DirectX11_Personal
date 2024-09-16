#pragma once
#include "Actor.h"

BEGIN(Engine)

class CCameraComponent;

END

class FreeCam final : public CActor
{
public:
	explicit FreeCam() = default;
	virtual ~FreeCam() = default;

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
private:
	void KeyInput(const _float& _fTimeDelta);
	void FocusOnViewport();

/********************
	Data Members
********************/
private:
	weak_ptr<CCameraComponent> m_wpCameraComponent;

	_bool m_bMouseHold = false;
};

