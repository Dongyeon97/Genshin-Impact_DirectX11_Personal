
#pragma once
#include "IngameUI.h"

class CMonsterHPbar : public CIngameUI
{
public:
	explicit CMonsterHPbar() = default;
	virtual ~CMonsterHPbar() = default;

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
	virtual HRESULT BindShaderResources() override;

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:

};
