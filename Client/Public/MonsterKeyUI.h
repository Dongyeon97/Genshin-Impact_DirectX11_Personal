#pragma once
#include "IngameUI.h"
class CMonsterKeyUI : public CIngameUI
{
public:
	explicit CMonsterKeyUI() = default;
	virtual ~CMonsterKeyUI() = default;

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
	virtual _vector CalculateToViewPort() override;

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
	_float4 m_vViewPortPos = {};
};

