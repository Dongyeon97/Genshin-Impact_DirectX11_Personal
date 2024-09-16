#pragma once
#include "Actor.h"

BEGIN(Engine)
class CCameraComponent;
class CProductionCamera;
END

class CPlayer;
class CSidePortrait;
class CSkillIcon;
class CUIActor_Base;

class CUserInstance : public CActor
{
public:
	explicit CUserInstance() = default;
	virtual ~CUserInstance() = default;

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
	HRESULT ReadyCamera();
	HRESULT ReadyCharactors();
	HRESULT ReadyPortraitLongFrames();
	HRESULT ReadyPortraitFrames();
	HRESULT ReadyPortraits();
	HRESULT ReadySideHPbar(const wstring& _wstrCharactorName, _uint _iIndex);
	HRESULT ReadySkillFrame();
	HRESULT ReadyEffects();
		HRESULT ReadySpriteEffects(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio);
	HRESULT ReadyDamageFont();
	HRESULT ReadyElementalReactionFont();

public:
	void RequestPartyHeal(_float _fHealTime, _float _fHealTick, _float _fHealValue);
	void ManagementPartyHeal(const _float& _fTimeDelta);

public:
	void RequestShield(_float _fShieldTime);
	void ManagementShield(const _float& _fTimeDelta);

private:
	void KeyInput();
	void ManagementPlayerCoolTime(const _float& _fTimeDelta);

private:
	HRESULT ChangeChractor(_uint _iIndex);
	void ChangePortraitSizeAndPos(_uint _iIndex);

private:
	void UpdateNavigationCellIndex(_uint _iIndex);
	void UpdateSRT(_uint _iIndex);
	void UpdateUserWorldMatrix();
/********************
	Getter / Setter
********************/
public:
	void SetCurCharactor(_uint _iIndex);
	shared_ptr<CPlayer> GetCurCharactor();

	shared_ptr<CUserInstance> GetUserInstance() { return dynamic_pointer_cast<CUserInstance>(shared_from_this()); }

	_bool GetIsShield() { return m_bShield; }
/********************
	Data Members
********************/
protected:
	// [ 3인칭 연출 카메라 ]
	weak_ptr<CProductionCamera> m_wpProductionCamera;

	// [ 숄더뷰 카메라 ]
	weak_ptr<CCameraComponent> m_wpShoulderViewCamera;

	// [ 캐릭터들을 담아두는 컨테이너 ]
	vector<shared_ptr<CPlayer>> m_vecCharactors;

	// [ 현재 캐릭터 인덱스 ]
	_uint m_iCurCharactorIndex = 0;

	// [ 캐릭터 초상화 프레임 ]
	vector<shared_ptr<CSidePortrait>> m_vecPortraitFrames;

	// [ 캐릭터 초상화 긴 프레임 ]
	vector<shared_ptr<CSidePortrait>> m_vecPortraitLongFrames;

	// [ 캐릭터 초상화 ]
	vector<shared_ptr<CSidePortrait>> m_vecPortraits;

	// [ 캐릭터 HP바 ]
	vector<shared_ptr<CUIActor_Base>> m_vecSideHPbar;

	// [ 캐릭터 HP바 프레임 ]
	vector<shared_ptr<CUIActor_Base>> m_vecSideHPbarFrames;

	// [ 스킬 아이콘 프레임 ]
	vector<shared_ptr<CSkillIcon>> m_vecSkillFrames;

private:
	_bool m_bPartyHeal = false;

	_float m_fPartyHealMinTime = 0.f;
	_float m_fPartyHealTimeAcc = 0.f;

	_float m_fPartyHealTick = 0.f;
	_float m_fPartyHealTickAcc = 0.f;

	_float m_fPartyHealValue = 0.f;

private:
	_bool m_bShield = false;

	_float m_fShieldMinTime = 0.f;
	_float m_fShieldTimeAcc = 0.f;
};

