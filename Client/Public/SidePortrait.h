#pragma once
#include "UIActor_Base.h"

class CSidePortrait : public CUIActor_Base
{
public:
	explicit CSidePortrait() = default;
	virtual ~CSidePortrait() = default;

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
	void SetName(const wstring& _wstrName) { m_wstrCharactorName = _wstrName; }
	void SetFontPosition(_float2 _vPosition) { m_vFontPosition = _vPosition; }

/********************
	Data Members
********************/
private:
	wstring m_wstrCharactorName = L"";

	_float2 m_vFontPosition = {};
};

