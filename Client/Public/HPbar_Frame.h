#pragma once
#include "UIActor_Base.h"

class CHPbar_Frame : public CUIActor_Base
{
public:
	explicit CHPbar_Frame() = default;
	virtual ~CHPbar_Frame() = default;

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
	virtual HRESULT BindShaderResources() override;


/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:

};
