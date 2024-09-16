#pragma once
#include "Player.h"
class CPlayer_Eula : public CPlayer
{
public:
	explicit CPlayer_Eula() = default;
	virtual ~CPlayer_Eula() = default;

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

