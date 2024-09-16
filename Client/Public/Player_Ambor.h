#pragma once
#include "Player.h"

class CUIActor_Base;

class CPlayer_Ambor : public CPlayer
{
public:
	explicit CPlayer_Ambor() = default;
	virtual ~CPlayer_Ambor() = default;

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
		HRESULT ReadyProjectileEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);
		HRESULT ReadyPlayerAttackEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);

	virtual HRESULT ReadyHPbar();
	virtual HRESULT ReadySkillIcon();

	HRESULT ReadyArrowRain(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);

/********************
	Getter / Setter
********************/
public:
	

/********************
	Data Members
********************/
private:

};

