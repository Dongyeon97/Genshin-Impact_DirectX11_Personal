#pragma once
#include "Player.h"

class CUIActor_Base;

class CPlayer_Nahida : public CPlayer
{
public:
	explicit CPlayer_Nahida() = default;
	virtual ~CPlayer_Nahida() = default;

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
	virtual void BindWeaponToChest();

	virtual void ReadyEffect();
		HRESULT ReadyProjectileEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);
		HRESULT ReadyPlayerAttackEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter);

		HRESULT ReadyEffect_Skill_E(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum);
		HRESULT ReadyEffect_Skill_E_Seal(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum);

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
	weak_ptr<CAttackCollider> m_wpNormalAttackCollider;
};

