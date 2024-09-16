#include "pch.h"
#include "UserInstance.h"

#include "ObjectManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "NavigationManager.h"
#include "FontManager.h"

#include "Level.h"
#include "Layer.h"

#include "Player_Eula.h"
#include "Player_Ambor.h"
#include "Player_Nahida.h"
#include "Player_Zhongli.h"
#include "SpriteActor.h"
#include "SidePortrait.h"
#include "SkillIcon.h"
#include "HPbar.h"
#include "HPbar_Frame.h"
#include "DamageFont.h"
#include "ElementalReaction.h"

#include "Navigation.h"
#include "ProductionCamera.h"
#include "PlayerStateMachine.h"

void CUserInstance::Initialize()
{
	if (FAILED(ReadyCamera()))
	{
		MSG_BOX("ī�޶� ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyCharactors()))
	{
		MSG_BOX("ĳ���� ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyPortraitFrames()))
	{
		MSG_BOX("ĳ���� �ʻ�ȭ ������ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyPortraitLongFrames()))
	{
		MSG_BOX("ĳ���� �ʻ�ȭ �� ������ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyPortraits()))
	{
		MSG_BOX("ĳ���� �ʻ�ȭ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyEffects()))
	{
		MSG_BOX("�ǰ� ����Ʈ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadySkillFrame()))
	{
		MSG_BOX("��ų ������ ������ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyDamageFont()))
	{
		MSG_BOX("������ ��Ʈ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	if (FAILED(ReadyElementalReactionFont()))
	{
		MSG_BOX("���ҹ��� ��Ʈ ������ �����Ͽ����ϴ�. : CUserInstance");
		return;
	}

	CActor::Initialize();

	ReadySideHPbar(L"����", 0);
	ReadySideHPbar(L"����", 1);
	ReadySideHPbar(L"������", 2);
	ReadySideHPbar(L"����", 3);

	// ���� ĳ���ͷ� �������� ���� ĳ������ �浹ü�� ���ܽ�ŵ�ϴ�. -> �����־ ���� ��������
	for (auto& player : m_vecCharactors)
	{
		if (player == m_vecCharactors[m_iCurCharactorIndex])
			continue;

		player->RemovePlayerBodyCollider();
	}
}

_int CUserInstance::PreUpdate(const _float& _fTimeDelta)
{
	for (auto& PortraitLongFrame : m_vecPortraitLongFrames)
		PortraitLongFrame->PreUpdate(_fTimeDelta);

	for (auto& PortraitFrame : m_vecPortraitFrames)
		PortraitFrame->PreUpdate(_fTimeDelta);

	for (auto& Portrait : m_vecPortraits)
		Portrait->PreUpdate(_fTimeDelta);

	for (auto& SkillFrame : m_vecSkillFrames)
		SkillFrame->PreUpdate(_fTimeDelta);

	for (auto& HPbarFrame : m_vecSideHPbarFrames)
		HPbarFrame->PreUpdate(_fTimeDelta);

	for (auto& HPbar : m_vecSideHPbar)
		HPbar->PreUpdate(_fTimeDelta);

	if (nullptr != m_vecCharactors[m_iCurCharactorIndex])
		m_vecCharactors[m_iCurCharactorIndex]->PreUpdate(_fTimeDelta);

	if (!m_wpProductionCamera.expired())
		m_wpProductionCamera.lock()->PreUpdate(_fTimeDelta);

	if (!m_wpShoulderViewCamera.expired())
		m_wpShoulderViewCamera.lock()->PreUpdate(_fTimeDelta);

	return S_OK;
}

_int CUserInstance::Update(const _float& _fTimeDelta)
{
	KeyInput();
	ManagementPlayerCoolTime(_fTimeDelta);

	if(m_bPartyHeal)
		ManagementPartyHeal(_fTimeDelta);

	if (m_bShield)
		ManagementShield(_fTimeDelta);

	for (auto& PortraitLongFrame : m_vecPortraitLongFrames)
		PortraitLongFrame->Update(_fTimeDelta);

	for (auto& PortraitFrame : m_vecPortraitFrames)
		PortraitFrame->Update(_fTimeDelta);

	for (auto& Portrait : m_vecPortraits)
		Portrait->Update(_fTimeDelta);

	for (auto& SkillFrame : m_vecSkillFrames)
		SkillFrame->Update(_fTimeDelta);

	for (auto& HPbarFrame : m_vecSideHPbarFrames)
		HPbarFrame->Update(_fTimeDelta);

	for (auto& HPbar : m_vecSideHPbar)
		HPbar->Update(_fTimeDelta);

	if (nullptr != m_vecCharactors[m_iCurCharactorIndex])
		m_vecCharactors[m_iCurCharactorIndex]->Update(_fTimeDelta);

	// �ٷ� ������ ���ŵ� ���� �÷��̾��� ���� ��Ʈ������ UserInstance�� �����Ѵ�.
	UpdateUserWorldMatrix();

	if (!m_wpProductionCamera.expired())
		m_wpProductionCamera.lock()->Update(_fTimeDelta);

	if (!m_wpShoulderViewCamera.expired())
		m_wpShoulderViewCamera.lock()->Update(_fTimeDelta);

	return S_OK;
}

_int CUserInstance::LateUpdate(const _float& _fTimeDelta)
{
	for (auto& PortraitLongFrame : m_vecPortraitLongFrames)
		PortraitLongFrame->LateUpdate(_fTimeDelta);

	for (auto& PortraitFrame : m_vecPortraitFrames)
		PortraitFrame->LateUpdate(_fTimeDelta);

	for (auto& Portrait : m_vecPortraits)
		Portrait->LateUpdate(_fTimeDelta);

	for (auto& SkillFrame : m_vecSkillFrames)
		SkillFrame->LateUpdate(_fTimeDelta);

	for (auto& HPbarFrame : m_vecSideHPbarFrames)
		HPbarFrame->LateUpdate(_fTimeDelta);

	for (auto& HPbar : m_vecSideHPbar)
		HPbar->LateUpdate(_fTimeDelta);

	if (nullptr != m_vecCharactors[m_iCurCharactorIndex])
		m_vecCharactors[m_iCurCharactorIndex]->LateUpdate(_fTimeDelta);

	if (!m_wpProductionCamera.expired())
		m_wpProductionCamera.lock()->LateUpdate(_fTimeDelta);

	if (!m_wpShoulderViewCamera.expired())
		m_wpShoulderViewCamera.lock()->LateUpdate(_fTimeDelta);

	return S_OK;
}

void CUserInstance::Render()
{
	for (auto& PortraitLongFrame : m_vecPortraitLongFrames)
		PortraitLongFrame->Render();

	for (auto& PortraitFrame : m_vecPortraitFrames)
		PortraitFrame->Render();

	for (auto& Portrait : m_vecPortraits)
		Portrait->Render();

	for (auto& SkillFrame : m_vecSkillFrames)
		SkillFrame->Render();

	for (auto& HPbarFrame : m_vecSideHPbarFrames)
		HPbarFrame->Render();

	for (auto& HPbar : m_vecSideHPbar)
		HPbar->Render();

	if (nullptr != m_vecCharactors[m_iCurCharactorIndex])
		m_vecCharactors[m_iCurCharactorIndex]->Render();

	//if (!m_wpProductionCamera.expired())
	//	m_wpProductionCamera.lock()->Render();
}

void CUserInstance::EndPlay()
{
	for (auto& PortraitLongFrame : m_vecPortraitLongFrames)
		PortraitLongFrame->EndPlay();

	for (auto& PortraitFrame : m_vecPortraitFrames)
		PortraitFrame->EndPlay();

	for (auto& Portrait : m_vecPortraits)
		Portrait->EndPlay();

	for (auto& SkillFrame : m_vecSkillFrames)
		SkillFrame->EndPlay();

	for (auto& HPbarFrame : m_vecSideHPbarFrames)
		HPbarFrame->EndPlay();

	for (auto& HPbar : m_vecSideHPbar)
		HPbar->EndPlay();

	if (nullptr != m_vecCharactors[m_iCurCharactorIndex])
		m_vecCharactors[m_iCurCharactorIndex]->EndPlay();

	if (!m_wpProductionCamera.expired())
		m_wpProductionCamera.lock()->EndPlay();

	if (!m_wpShoulderViewCamera.expired())
		m_wpShoulderViewCamera.lock()->EndPlay();
}

void CUserInstance::Release()
{
	CActor::Release();
}

HRESULT CUserInstance::ReadyCamera()
{
	// [ Component - 3��Ī ī�޶� ]
	m_wpProductionCamera = CreateActorComponent<CProductionCamera>(L"ThridPersonCameraComponent");
	if (m_wpProductionCamera.expired())
		return E_FAIL;

	m_wpProductionCamera.lock()->SetFar(2000000.0f);
	m_wpProductionCamera.lock()->SetRadius(200.0f);
	m_wpProductionCamera.lock()->SetActorTranslation(XMVectorSet(0.0f, 200.0f, 200.0f, 0.0f));
	CCameraManager::GetInstance()->AddCamera(L"Player_Production_Camera", m_wpProductionCamera.lock());
	CCameraManager::GetInstance()->SetCurCamera(L"Player_Production_Camera");

	// [ Component - ����� ī�޶� ]
	m_wpShoulderViewCamera = CreateActorComponent<CCameraComponent>(L"ShoulderViewCameraComponent");
	if (m_wpShoulderViewCamera.expired())
		return E_FAIL;

	m_wpShoulderViewCamera.lock()->SetFar(2000000.f);
	m_wpShoulderViewCamera.lock()->SetActorTranslation(XMVectorSet(100.0f, 200.0f, -250.0f, 0.0f));
	CCameraManager::GetInstance()->AddCamera(L"Player_ShoulderView_Camera", m_wpShoulderViewCamera.lock());

	return S_OK;
}
HRESULT CUserInstance::ReadyCharactors()
{
	/* �÷��̾� */
	shared_ptr<CPlayer> spEula = CreateActor<CPlayer_Eula>(L"Player_Eula");
	if (nullptr == spEula)
	{
		MSG_BOX("���� ĳ���� ������ �����Ͽ����ϴ�. : CUserInstance");
		return E_FAIL;
	}
	shared_ptr<CPlayer> spAmbor = CreateActor<CPlayer_Ambor>(L"Player_Ambor");
	if (nullptr == spAmbor)
	{
		MSG_BOX("���� ĳ���� ������ �����Ͽ����ϴ�. : CUserInstance");
		return E_FAIL;
	}
	shared_ptr<CPlayer> spNahida = CreateActor<CPlayer_Nahida>(L"Player_Nahida");
	if (nullptr == spNahida)
	{
		MSG_BOX("������ ĳ���� ������ �����Ͽ����ϴ�. : CUserInstance");
		return E_FAIL;
	}
	shared_ptr<CPlayer> spZhongli = CreateActor<CPlayer_Zhongli>(L"Player_Zhongli");
	if (nullptr == spZhongli)
	{
		MSG_BOX("���� ĳ���� ������ �����Ͽ����ϴ�. : CUserInstance");
		return E_FAIL;
	}

	CObjectManager::GetInstance()->SetCurPlayer(spEula);
	m_wpProductionCamera.lock()->SetOwnerActor(spEula);

	// ��Ƽ�� �ִ� 4��
	m_vecCharactors.reserve(4);
	m_vecCharactors.push_back(spEula);
	m_vecCharactors.push_back(spAmbor);
	m_vecCharactors.push_back(spNahida);
	m_vecCharactors.push_back(spZhongli);

	return S_OK;
}
HRESULT CUserInstance::ReadyPortraitLongFrames()
{
	for (_uint i = 0; i < 4; ++i)
	{
		shared_ptr<CSidePortrait> spSidePortrait = CreateActor<CSidePortrait>(L"SideLongFrame" + to_wstring(i));
		if (nullptr == spSidePortrait)
			return E_FAIL;
		spSidePortrait->SetShaderFileName(L"Shader_UI_PortraitFrame");
		spSidePortrait->SetTextureFileName(L"UI_Img_FungusFighter_Title");
		spSidePortrait->SetSize({ 166.f, 56.f });
		spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.965f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f) });
		spSidePortrait->SetEnable(true);
		m_vecPortraitLongFrames.push_back(spSidePortrait);
	}

	return S_OK;
}
HRESULT CUserInstance::ReadyPortraitFrames()
{
	m_vecPortraitFrames.reserve(8);

	for (_uint i = 0; i < 4; ++i)
	{
		shared_ptr<CSidePortrait> spSidePortrait = CreateActor<CSidePortrait>(L"SideFrame" + to_wstring(i));
		if (nullptr == spSidePortrait)
			return E_FAIL;
		spSidePortrait->SetShaderFileName(L"Shader_UI_PortraitFrame");
		spSidePortrait->SetTextureFileName(L"UI_Main_AvatarSideBg");
		spSidePortrait->SetSize({ 40.f, 60.f });
		spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.918f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f)});
		spSidePortrait->SetEnable(true);
		m_vecPortraitFrames.push_back(spSidePortrait);
	}

	return S_OK;
}
HRESULT CUserInstance::ReadyPortraits()
{
	m_vecPortraits.reserve(4);

	/* ĳ���� �ʻ�ȭ */
	// 1
	shared_ptr<CSidePortrait> spSidePortrait = CreateActor<CSidePortrait>(L"Eula_SidePortrait");
	if (nullptr == spSidePortrait)
		return E_FAIL;
	spSidePortrait->SetShaderFileName(L"Shader_UIBase");
	spSidePortrait->SetTextureFileName(L"UI_AvatarIcon_Side_Eula");
	spSidePortrait->SetSize({ 90.f, 90.f });
	spSidePortrait->SetName(L"����");
	spSidePortrait->SetFontPosition({ Client::g_iWinSizeX * 0.92f + 30.f, Client::g_iWinSizeY * 0.22f });
	spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.92f, Client::g_iWinSizeY * 0.22f });
	spSidePortrait->SetEnable(true);
	m_vecPortraits.push_back(spSidePortrait);

	// 2
	spSidePortrait = CreateActor<CSidePortrait>(L"Ambor_SidePortrait");
	if (nullptr == spSidePortrait)
		return E_FAIL;
	spSidePortrait->SetShaderFileName(L"Shader_UIBase");
	spSidePortrait->SetTextureFileName(L"UI_AvatarIcon_Side_Ambor");
	spSidePortrait->SetSize({ 90.f, 90.f });
	spSidePortrait->SetName(L"����");
	spSidePortrait->SetFontPosition({ Client::g_iWinSizeX * 0.92f + 30.f, Client::g_iWinSizeY * 0.37f });
	spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.92f, Client::g_iWinSizeY * 0.37f });
	spSidePortrait->SetEnable(true);
	m_vecPortraits.push_back(spSidePortrait);

	// 3
	spSidePortrait = CreateActor<CSidePortrait>(L"Nahida_SidePortrait");
	if (nullptr == spSidePortrait)
		return E_FAIL;
	spSidePortrait->SetShaderFileName(L"Shader_UIBase");
	spSidePortrait->SetTextureFileName(L"UI_AvatarIcon_Side_Nahida");
	spSidePortrait->SetSize({ 90.f, 90.f });
	spSidePortrait->SetName(L"������");
	spSidePortrait->SetFontPosition({ Client::g_iWinSizeX * 0.92f + 30.f, Client::g_iWinSizeY * 0.52f });
	spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.92f, Client::g_iWinSizeY * 0.52f });
	spSidePortrait->SetEnable(true);
	m_vecPortraits.push_back(spSidePortrait);

	// 4
	spSidePortrait = CreateActor<CSidePortrait>(L"Zhongli_SidePortrait");
	if (nullptr == spSidePortrait)
		return E_FAIL;
	spSidePortrait->SetShaderFileName(L"Shader_UIBase");
	spSidePortrait->SetTextureFileName(L"UI_AvatarIcon_Side_Zhongli");
	spSidePortrait->SetSize({ 90.f, 90.f });
	spSidePortrait->SetName(L"����");
	spSidePortrait->SetFontPosition({ Client::g_iWinSizeX * 0.92f + 30.f, Client::g_iWinSizeY * 0.67f });
	spSidePortrait->SetPosition({ Client::g_iWinSizeX * 0.92f, Client::g_iWinSizeY * 0.67f });
	spSidePortrait->SetEnable(true);
	m_vecPortraits.push_back(spSidePortrait);

	return S_OK;
}
HRESULT CUserInstance::ReadySideHPbar(const wstring& _wstrCharactorName, _uint _iIndex)
{
	if (m_vecCharactors.size() - 1 < _iIndex)
		return E_FAIL;

	shared_ptr<CHPbar> spHPbar = CreateActor<CHPbar>(_wstrCharactorName + L"_HPbar");
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_HPbar");
	spHPbar->SetTextureFileName(L"UI_HPBar_");
	spHPbar->SetSize({ 45.f, 4.0f });
	spHPbar->SetPosition({ Client::g_iWinSizeX * 0.92f + 57.f, Client::g_iWinSizeY * (0.27f + (0.15f * _float(_iIndex))) });
	spHPbar->SetEnable(true);
	spHPbar->SetOwnerActor(m_vecCharactors[_iIndex]);
	spHPbar->Initialize();
	m_vecSideHPbar.push_back(spHPbar);

	shared_ptr<CHPbar_Frame> spHPbar_Frame = CreateActor<CHPbar_Frame>(_wstrCharactorName + L"_HPbar_Frame");
	if (nullptr == spHPbar_Frame)
		return E_FAIL;

	spHPbar_Frame->SetShaderFileName(L"Shader_UI_HPbar_Frame");
	spHPbar_Frame->SetTextureFileName(L"UI_HPBar_");
	spHPbar_Frame->SetSize({ 45.f, 4.0f });
	spHPbar_Frame->SetPosition({ Client::g_iWinSizeX * 0.92f + 57.f, Client::g_iWinSizeY * (0.27f + (0.15f * _float(_iIndex))) });
	spHPbar_Frame->SetEnable(true);
	spHPbar_Frame->SetOwnerActor(m_vecCharactors[_iIndex]);
	spHPbar_Frame->Initialize();
	m_vecSideHPbarFrames.push_back(spHPbar_Frame);

	return S_OK;
}
HRESULT CUserInstance::ReadySkillFrame()
{
	// E
	shared_ptr<CSkillIcon> spSkillIcon = CreateActor<CSkillIcon>(L"SkillFrame_E");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillFrame : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIconFrame");
	spSkillIcon->SetTextureFileName(L"UI_InLevel_SkillIconBg_E");
	spSkillIcon->SetSize({ 80.f, 80.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.83f, Client::g_iWinSizeY * 0.92f });
	spSkillIcon->SetEnable(true);
	m_vecSkillFrames.push_back(spSkillIcon);

	// Q
	spSkillIcon = CreateActor<CSkillIcon>(L"SkillFrame_Q");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillFrame : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIconFrame");
	spSkillIcon->SetTextureFileName(L"UI_InLevel_SkillIconBg_Q_Two");
	spSkillIcon->SetSize({ 120.f, 120.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.93f, Client::g_iWinSizeY * 0.9f });
	spSkillIcon->SetEnable(true);
	m_vecSkillFrames.push_back(spSkillIcon);

	return S_OK;
}
HRESULT CUserInstance::ReadyEffects()
{
#pragma region �ǰ�
	for (_uint i = 0; i < 3; ++i)
	{
		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_One", i, 3, 3, 0.03f)))
			return E_FAIL;

		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Two", i, 2, 2, 0.05f)))
			return E_FAIL;

		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Three", i, 2, 2, 0.05f)))
			return E_FAIL;

		// if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Four", i, 3, 3, 0.03f)))
			// return E_FAIL;

		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Five", i, 2, 2, 0.03f)))
			return E_FAIL;

		// if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Six", i, 2, 2, 0.03f)))
			// return E_FAIL;

		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Seven", i, 2, 2, 0.03f)))
			return E_FAIL;

		if (FAILED(ReadySpriteEffects(L"Sprite_Hit_Eight", i, 2, 2, 0.03f)))
			return E_FAIL;
	}

	return S_OK;
#pragma endregion �ǰ�
}
HRESULT CUserInstance::ReadySpriteEffects(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio)
{
	shared_ptr<CSpriteActor> spSpriteActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->CreateActorToPool<CSpriteActor>(_wstrJsonFileName + to_wstring(_iEffectsNum));
	if (nullptr == spSpriteActor)
	{
		MSG_BOX("Failed to Create HitEffects : CUserInstance");
		return E_FAIL;
	}

	spSpriteActor->SetJsonFileName(_wstrJsonFileName);
	spSpriteActor->SetRow(_iRow);
	spSpriteActor->SetCol(_iCol);
	spSpriteActor->SetSpeedRatio(_fSpeedRatio);

	if (FAILED(spSpriteActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spSpriteActor->GetActorName());
	}

	return S_OK;
}
HRESULT CUserInstance::ReadyDamageFont()
{
	for (_uint i = 0; i < 100; ++i)
	{
		shared_ptr<CDamageFont> spDamageFont = GetLevel().lock()->GetLayer(LAYER::LAYER_UI)->CreateActorToPool<CDamageFont>(L"DamageFont" + to_wstring(i));
		if (nullptr == spDamageFont)
		{
			MSG_BOX("Failed to Create DamagaFont : CUserInstance");
			return E_FAIL;
		}

		spDamageFont->Initialize();
	}

	return S_OK;
}

HRESULT CUserInstance::ReadyElementalReactionFont()
{
	for (_uint i = 0; i < 10; ++i)
	{
		shared_ptr<CElementalReaction> spElementalReactionFont = GetLevel().lock()->GetLayer(LAYER::LAYER_UI)->CreateActorToPool<CElementalReaction>(L"ElementalReactionFont" + to_wstring(i));
		if (nullptr == spElementalReactionFont)
		{
			MSG_BOX("Failed to Create ElementalReactionFont : CUserInstance");
			return E_FAIL;
		}

		spElementalReactionFont->Initialize();
	}

	return S_OK;
}

void CUserInstance::RequestPartyHeal(_float _fHealTime, _float _fHealTick, _float _fHealValue)
{
	m_bPartyHeal = true;

	m_fPartyHealMinTime = _fHealTime;
	m_fPartyHealTick = _fHealTick;
	m_fPartyHealValue = _fHealValue;
}
void CUserInstance::ManagementPartyHeal(const _float& _fTimeDelta)
{
	m_fPartyHealTimeAcc += _fTimeDelta;
	if (m_fPartyHealTimeAcc >= m_fPartyHealMinTime)
	{
		m_fPartyHealTimeAcc = 0.f;
		m_fPartyHealTick = 0.f;

		m_bPartyHeal = false;
		return;
	}

	m_fPartyHealTickAcc += _fTimeDelta;
	if (m_fPartyHealTickAcc >= m_fPartyHealTick)
	{
		for (auto& Player : m_vecCharactors)
		{
			Player->SetCurHp(Player->GetCurHp() + m_fPartyHealValue);

			if (Player->GetCurHp() > Player->GetMaxHp())
				Player->SetCurHp(Player->GetMaxHp());
		}

		m_fPartyHealTickAcc = 0.f;
	}
}

void CUserInstance::RequestShield(_float _fShieldTime)
{
	m_bShield = true;

	m_fShieldMinTime = _fShieldTime;
}

void CUserInstance::ManagementShield(const _float& _fTimeDelta)
{
	m_fShieldTimeAcc += _fTimeDelta;
	if (m_fShieldTimeAcc >= m_fShieldMinTime)
	{
		m_fShieldTimeAcc = 0.f;

		m_bShield = false;
		return;
	}
}

void CUserInstance::KeyInput()
{
	if (CInputManager::GetInstance()->IsKeyDown(DIK_1))
	{
		if (FAILED(ChangeChractor(0)))
		{
			MSG_BOX("ĳ���� ��ü�� �����߽��ϴ�. : CUserInstance");
			return;
		}
	}
	if (CInputManager::GetInstance()->IsKeyDown(DIK_2))
	{
		if (FAILED(ChangeChractor(1)))
		{
			MSG_BOX("ĳ���� ��ü�� �����߽��ϴ�. : CUserInstance");
			return;
		}
	}
	if (CInputManager::GetInstance()->IsKeyDown(DIK_3))
	{
		if (FAILED(ChangeChractor(2)))
		{
			MSG_BOX("ĳ���� ��ü�� �����߽��ϴ�. : CUserInstance");
			return;
		}
	}
	if (CInputManager::GetInstance()->IsKeyDown(DIK_4))
	{
		if (FAILED(ChangeChractor(3)))
		{
			MSG_BOX("ĳ���� ��ü�� �����߽��ϴ�. : CUserInstance");
			return;
		}
	}
}
void CUserInstance::ManagementPlayerCoolTime(const _float& _fTimeDelta)
{
	for (auto& Player : m_vecCharactors)
	{
		shared_ptr<CPlayerStateMachine> spPlayerStateMachine = dynamic_pointer_cast<CPlayerStateMachine>(Player->GetStateMachine());
		spPlayerStateMachine->ManagementActorCoolTime(_fTimeDelta);
	}
}

HRESULT CUserInstance::ChangeChractor(_uint _iIndex)
{
	if (m_vecCharactors.empty())
		return E_FAIL;

	if (m_vecCharactors.size() - 1 < _iIndex)
		return E_FAIL;

	if (nullptr == m_vecCharactors[_iIndex] ||
		nullptr == m_vecCharactors[m_iCurCharactorIndex])
		return E_FAIL;

	// ���� ĳ������ ��������� �ٲ� ĳ������ ���� ��ķ� ���������ش�.
	m_vecCharactors[_iIndex]->SetWorldMatrix(m_vecCharactors[m_iCurCharactorIndex]->GetWorldMatrix());

	// ���� ĳ������ �浹ü�� CollisionManager���� �����Ѵ�.
	// �ٲ� ĳ������ �浹ü�� CollisionManager�� �߰��Ѵ�.
	m_vecCharactors[m_iCurCharactorIndex]->RemovePlayerBodyCollider();
	m_vecCharactors[_iIndex]->AddPlayerBodyCollider();

	// ���� ĳ���Ϳ� �ٲ� ĳ������ �ִϸ��̼��� Standby�� �ʱ�ȭ �Ѵ�.
	m_vecCharactors[m_iCurCharactorIndex]->SetStandby();
	m_vecCharactors[_iIndex]->SetStandby();

	// �ٲ� ĳ������ SRT���� �׺���̼� �ε����� ���Ž����ش�.
	UpdateSRT(_iIndex);
	UpdateNavigationCellIndex(_iIndex);

	// ī�޶��� ������ �ٲ��ش�.
	m_wpProductionCamera.lock()->SetOwnerActor(m_vecCharactors[_iIndex]);

	// �ʻ�ȭ ũ�� �ٲ��ִ� �۾�
	ChangePortraitSizeAndPos(_iIndex);

	// ObjectManager���� ���� ĳ���͸� �ٲ� ĳ���ͷ� ��ü���ش�.
	CObjectManager::GetInstance()->SetCurPlayer(m_vecCharactors[_iIndex]);

	// ���� ĳ���� �ε����� ��ü���ش�.
	m_iCurCharactorIndex = _iIndex;

	return S_OK;
}
void CUserInstance::ChangePortraitSizeAndPos(_uint _iIndex)
{
	if (m_vecPortraitLongFrames.size() - 1 < _iIndex || 
		m_vecPortraitFrames.size() - 1 < _iIndex ||
		m_vecPortraits.size() - 1 < _iIndex)
		return;

	if (m_vecCharactors.size() - 1 < _iIndex)
		return;

	for (_uint i = 0; i < 4; ++i)
	{
		if (i == _iIndex)
		{
			m_vecPortraitLongFrames[i]->SetSize(_float2(123.f, 42.f));
			m_vecPortraitLongFrames[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.965f + 20.f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f)));

			m_vecPortraitFrames[i]->SetSize(_float2(30.f, 45.f));
			m_vecPortraitFrames[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.918f + 20.f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f)));

			m_vecPortraits[i]->SetSize(_float2(80.0f, 80.0f));
			m_vecPortraits[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.905f + 20.f, Client::g_iWinSizeY * (0.22f + _float(i) * 0.15f)));
			m_vecPortraits[i]->SetFontPosition(_float2(Client::g_iWinSizeX * 0.905f + 65.f, Client::g_iWinSizeY * (0.22f + _float(i) * 0.15f) + 5.f));

			if (i < m_vecCharactors.size())
			{
				m_vecSideHPbar[i]->SetEnable(false);
				m_vecSideHPbarFrames[i]->SetEnable(false);
			}
		}
		else
		{
			m_vecPortraitLongFrames[i]->SetSize(_float2(166.f, 56.f));
			m_vecPortraitLongFrames[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.98f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f)));

			m_vecPortraitFrames[i]->SetSize(_float2(40.f, 60.f));
			m_vecPortraitFrames[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.933f, Client::g_iWinSizeY * (0.245f + _float(i) * 0.15f)));

			m_vecPortraits[i]->SetSize(_float2(90.f, 90.f));
			m_vecPortraits[i]->SetPosition(_float2(Client::g_iWinSizeX * 0.92f, Client::g_iWinSizeY * (0.22f + _float(i) * 0.15f)));
			m_vecPortraits[i]->SetFontPosition(_float2(Client::g_iWinSizeX * 0.92f + 30.f, Client::g_iWinSizeY * (0.22f + _float(i) * 0.15f)));

			if (i < m_vecCharactors.size())
			{
				m_vecSideHPbar[i]->SetEnable(true);
				m_vecSideHPbarFrames[i]->SetEnable(true);
			}
		}
	}
}

void CUserInstance::UpdateNavigationCellIndex(_uint _iIndex)
{
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();
	m_vecCharactors[_iIndex]->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(m_vecCharactors[_iIndex]->GetActorTranslation()));
}
void CUserInstance::UpdateSRT(_uint _iIndex)
{
	m_vecCharactors[_iIndex]->SetScale(m_vecCharactors[m_iCurCharactorIndex]->GetScale());
	m_vecCharactors[_iIndex]->SetRotation(m_vecCharactors[m_iCurCharactorIndex]->GetRotation());
	m_vecCharactors[_iIndex]->SetActorTranslation_NoNavigation(m_vecCharactors[m_iCurCharactorIndex]->GetActorTranslation());
}

void CUserInstance::UpdateUserWorldMatrix()
{
	if (nullptr == m_vecCharactors[m_iCurCharactorIndex])
		return;

	XMStoreFloat4(&m_vScale, m_vecCharactors[m_iCurCharactorIndex]->GetScale());

	XMStoreFloat4(&m_vRotation, m_vecCharactors[m_iCurCharactorIndex]->GetRotation());
	XMStoreFloat4x4(&m_matAdditionalRotation, m_vecCharactors[m_iCurCharactorIndex]->GetAdditionalRotationMatrix());
	XMStoreFloat4x4(&m_matMouseRotate, m_vecCharactors[m_iCurCharactorIndex]->GetMouseRotateMatrix());
	XMStoreFloat4x4(&m_matTotalRotation, m_vecCharactors[m_iCurCharactorIndex]->GetTotalRotationMatrix());

	XMStoreFloat4(&m_vTranslate, m_vecCharactors[m_iCurCharactorIndex]->GetActorTranslation());

	UpdateActorTransform();
}

void CUserInstance::SetCurCharactor(_uint _iIndex)
{
	if (0 > _iIndex || 3 < _iIndex)
		return;

	if (m_vecCharactors.empty())
		return;

	if (nullptr == m_vecCharactors[_iIndex])
		return;

	m_iCurCharactorIndex = _iIndex;
}
shared_ptr<CPlayer> CUserInstance::GetCurCharactor()
{
	if (m_vecCharactors.empty())
		return nullptr;

	if (nullptr == m_vecCharactors[m_iCurCharactorIndex])
		return nullptr;

	return m_vecCharactors[m_iCurCharactorIndex];
}
