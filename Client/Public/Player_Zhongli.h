#pragma once
#include "Player.h"

class CPlayer_Zhongli : public CPlayer
{
public:
	explicit CPlayer_Zhongli() = default;
	virtual ~CPlayer_Zhongli() = default;

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
	virtual void SelectBoneMatrixToWeapon();

	virtual void ReadyEffect();
	HRESULT ReadyShield(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum);
	HRESULT ReadyJusin(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);

	virtual HRESULT ReadyHPbar();
	virtual HRESULT ReadySkillIcon();

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:
	
};

