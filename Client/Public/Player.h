
#pragma once
#include "Charactor.h"
#include "PlayerStateDefine.h"

BEGIN(Engine)
class CProductionCamera;
class CAttackCollider;
END

class CWeapon;
class CUIActor_Base;
class CSkillIcon;

class CPlayer : public CCharactor
{
public:
	explicit CPlayer() = default;
	virtual ~CPlayer() = default;

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
	virtual void SetActorTranslation(_fvector _vPosition, _int* _iCurCellIndex) override;

	void UpdateCollidersWorldMatrix();
		void RemovePlayerBodyCollider();
		void AddPlayerBodyCollider();

	void SetStandby();

protected:
	void KeyInput(const _float& _fTimeDelta);
	void MouseHold(const _float& _fTimeDelta);
	void BindCoolTimeToSkillIcon();

	void ReadyBoneMatrixToWeapon();
	virtual void SelectBoneMatrixToWeapon() {};
		virtual void BindWeaponToChest();
		void BindWeaponToLeftArm();
		void BindWeaponToRightArm();

	HRESULT ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum);
	HRESULT ReadySpriteEffect(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio);
	HRESULT ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum);

	virtual void ReadyEffect() {};

	virtual HRESULT ReadyHPbar() { return S_OK; }
	virtual HRESULT ReadySkillIcon() { return S_OK; }

private:
	virtual _int OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});
	virtual void OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});
	virtual void OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});

/********************
	Getter / Setter
********************/
public:
	shared_ptr<CAnimModel> GetAnimModel() { return m_wpAnimModel.lock(); }
	shared_ptr<CWeapon> GetWeapon() { return m_wpWeapon.lock(); }

	void SetHPBarEnable(_bool _bEnable);
/********************
	Data Members
********************/
protected:
	// [ ���� ������Ʈ - ���� ]
	weak_ptr<CWeapon> m_wpWeapon;

	// [ ���Ⱑ ������ ������ �� ]
	const _float4x4* m_pSocketMatrix_RightArm = nullptr;
	// [ ���Ⱑ ������ �޼� �� ]
	const _float4x4* m_pSocketMatrix_LeftArm = nullptr;
	// [ ���Ⱑ ������ ���� �� ]
	const _float4x4* m_pSocketMatrix_Chest = nullptr;

	// [ ��ų ���� ���� �ݶ��̴� ]
	weak_ptr<CAttackCollider> m_wpSkillCollider;

protected:
	/* �ӽ� */
	_bool m_bMouseHold = true;

	/* ���ݹ��� ��û ����� */
	REQUEST_DIR m_eRequestDir = REQUEST_DIR::DIR_NONE;

protected:
	// [ ��ų ������ ]
	vector<shared_ptr<CSkillIcon>> m_vecSkillIcons;

	// [ HP �� ]
	vector<shared_ptr<CUIActor_Base>> m_vecHPbar;
};

