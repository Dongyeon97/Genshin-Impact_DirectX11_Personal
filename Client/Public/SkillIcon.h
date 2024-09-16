#pragma once
#include "UIActor_Base.h"

class CSkillIcon : public CUIActor_Base
{
public:
	explicit CSkillIcon() = default;
	virtual ~CSkillIcon() = default;

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
	void SetFontPosition(_float2 _vPosition) { m_vFontPosition = _vPosition; }
	void SetSkillCoolTime(_float2 _vCoolTime) { m_vSkillCoolTime = _vCoolTime; };
	void SetFontSize(_float _fSize) { m_fCoolTimeFontSize = _fSize; }

/********************
	Data Members
********************/
private:
	_float2 m_vSkillCoolTime = { 1.f, 1.f };

	_float m_fCoolTimeFontSize = 1.f;

	_float2 m_vFontPosition = {};

	_bool m_bCoolTimeProgressed = false;
};

