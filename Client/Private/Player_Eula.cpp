#include "pch.h"
#include "Player_Eula.h"

#include "CollisionManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"

#include "Level.h"
#include "Layer.h"

#include "Eula_StateMachine.h"
#include "AnimModel.h"
#include "Animation.h"
#include "Songlue.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "SkillIcon.h"
#include "HPbar.h"
#include "HPbar_Frame.h"

void CPlayer_Eula::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Player_Eula_Proto4");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Player_Eula");

#pragma region 액터
	// UI, Effect
	ReadyEffect();
	ReadySkillIcon();
	ReadyHPbar();

	m_fMaxHp = 19294.0f;
	m_fCurHp = 19294.0f;
	m_fAtk = 3570.0f;
	m_fDef = 200.0f;
#pragma endregion 액터

#pragma region 컴포넌트
	// [ Component - Collider ]
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 70.0f, 0.0f });
	m_wpCollider.lock()->SetColliderExtents({ 35.f, 70.f, 35.f });
	CCollisionManager::GetInstance()->AddPlayerBodyCollider(m_wpCollider);

	// [ Component - SkillCollider ]
	m_wpSkillCollider = CreateActorComponent<CAttackCollider>(L"SkillCollider");
	m_wpSkillCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpSkillCollider.lock()->SetColliderCenter({ 0.0f, 100.0f, 0.0f });
	m_wpSkillCollider.lock()->SetColliderExtents({ 200.f, 100.f, 200.f });
	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpSkillCollider);

	// ================================================================================================
	m_wpWeapon = CreateActorComponent<CSonglue>(L"Weapon");
	m_wpStateMachine = CreateActorComponent<CEula_StateMachine>(L"PlayerStateMachine");
	// ================================================================================================
#pragma endregion 컴포넌트

	CPlayer::Initialize();

#pragma region 컴포넌트
	// [ Component - AnimModel ]
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::STANDBY]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::WALK_STOP]->SetInterpolationDuration(1.0f);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::WALK_STOP]->SetInterpolationRatio(5.0f);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::WALK]->SetIsLoop(true);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::RUN_CYCLE]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::RUN_DOWNHILL]->SetIsLoop(true);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::SPRINT_CYCLE]->SetIsLoop(true);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::HIT_FALLTOGROUND]->SetIsLoop(false);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_IDLE_D]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_L]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_U]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_R]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_D]->SetIsLoop(true);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_NORMAL]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_TURN_L]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_TURN_R]->SetIsLoop(true);

	// [ Component - StateMachine ]
	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());
	dynamic_pointer_cast<CEula_StateMachine>(m_wpStateMachine.lock())->SetWeaponCollider(m_wpWeapon.lock()->GetWeaponCollider());
	dynamic_pointer_cast<CEula_StateMachine>(m_wpStateMachine.lock())->SetSkillCollider(m_wpSkillCollider);
#pragma endregion 컴포넌트
}

_int CPlayer_Eula::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayer::PreUpdate(_fTimeDelta);
}

_int CPlayer_Eula::Update(const _float& _fTimeDelta)
{
	return CPlayer::Update(_fTimeDelta);
}

_int CPlayer_Eula::LateUpdate(const _float& _fTimeDelta)
{
	SelectBoneMatrixToWeapon();

	return CPlayer::LateUpdate(_fTimeDelta);
}

void CPlayer_Eula::Render()
{
	CPlayer::Render();
}

void CPlayer_Eula::EndPlay()
{
	CPlayer::EndPlay();
}

void CPlayer_Eula::Release()
{
	CPlayer::Release();
}

void CPlayer_Eula::SelectBoneMatrixToWeapon()
{
	_ullong PlayerState = m_wpStateMachine.lock()->GetCurState();

	if (PlayerState & PLAYER_STATE::IS_ATTACK)
	{
		// 평타 1,2,3타
		if (PlayerState & PLAYER_STATE::ATTACK_1 ||
			PlayerState & PLAYER_STATE::ATTACK_2 ||
			PlayerState & PLAYER_STATE::ATTACK_3)
		{
			BindWeaponToRightArm();
		}
		// 평타 4,5타
		else if (PlayerState & PLAYER_STATE::ATTACK_4 ||
			PlayerState & PLAYER_STATE::ATTACK_5)
		{
			BindWeaponToLeftArm();
		}
		else if (PlayerState & PLAYER_STATE::ELEMENTAL_BURST)
		{
			_float fBurstRunTime = dynamic_pointer_cast<CEula_StateMachine>(m_wpStateMachine.lock())->GetElementalBurst_RunTime();
			if (3.0f < fBurstRunTime)
				BindWeaponToChest();
			else
				BindWeaponToRightArm();
		}
		else
		{
			BindWeaponToRightArm();
		}
	}
	else
	{
		BindWeaponToChest();
	}
}

void CPlayer_Eula::BindWeaponToChest()
{
	m_wpWeapon.lock()->SetRotation(XMVectorSet(-90.0f, 90.0f, 100.f, 0.0f));

	m_wpWeapon.lock()->SetActorTranslation(XMVectorSet(25.0f, -50.0f, 0.0f, 1.0f));
	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_Chest);
}

void CPlayer_Eula::ReadyEffect()
{
	// 파티클
	for (_uint i = 0; i < 5; ++i)
	{
		ReadyParticle(L"Particle_Eula_NormalAttack_One", i);
		ReadyParticle(L"Particle_Eula_NormalAttack_Two", i);
		ReadyParticle(L"Particle_Feather", i);
		ReadyParticle(L"Particle_DotLight", i);
		ReadyParticle(L"Particle_Twinkle", i);
		ReadyParticle(L"Particle_Twinkle_Cross", i);
		ReadyParticle(L"Particle_Twinkle_Star", i);
	}
	for (_uint i = 0; i < 5; ++i)
	{
		ReadyParticle(L"Particle_Feather_Q", i);
		ReadyParticle(L"Particle_DotLight_Q", i);
		ReadyParticle(L"Particle_Twinkle_Q", i);
		ReadyParticle(L"Particle_Twinkle_Cross_Q", i);
		ReadyParticle(L"Particle_Twinkle_Star_Q", i);
	}

	// 기본 공격
	for (_uint i = 0; i < 5; ++i)
	{
		ReadyEffectMesh(L"Eula_NormalAtk_One", i);
		ReadyEffectMesh(L"Eula_NormalAtk_Two", i);
		ReadyEffectMesh(L"Eula_NormalAtk_Three_One", i);
		ReadyEffectMesh(L"Eula_NormalAtk_Three_Two", i);
		ReadyEffectMesh(L"Eula_NormalAtk_Four", i);
		ReadyEffectMesh(L"Eula_NormalAtk_Five_One", i);
		// ReadyEffectMesh(L"Eula_NormalAtk_Five_Two", i);

		// ReadyEffectMesh(L"Eula_NormalAtk_Five_Two_Horizon", i);
		// ReadyEffectMesh(L"Eula_NormalAtk_Five_Two_Vertical", i);
	}

	// E
	ReadyEffectMesh(L"Eula_Skill_E", 0);

	// Q
	ReadyEffectMesh(L"Eula_Skill_Q_One", 0);
	ReadyEffectMesh(L"Eula_Skill_Q_Two", 0);
}
HRESULT CPlayer_Eula::ReadyHPbar()
{
	shared_ptr<CHPbar> spHPbar = CreateActor<CHPbar>(L"Eula_Hpbar");
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_HPbar");
	spHPbar->SetTextureFileName(L"UI_HPBar_");
	spHPbar->SetSize({ 255.f, 13.f });
	spHPbar->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.95f });
	spHPbar->SetEnable(true);
	m_vecHPbar.push_back(spHPbar);

	shared_ptr<CHPbar_Frame> spHPbar_Frame = CreateActor<CHPbar_Frame>(L"Eula_Hpbar_Frame");
	if (nullptr == spHPbar_Frame)
		return E_FAIL;

	spHPbar_Frame->SetShaderFileName(L"Shader_UI_HPbar_Frame");
	spHPbar_Frame->SetTextureFileName(L"UI_HPBar_");
	spHPbar_Frame->SetSize({ 255.f, 13.f });
	spHPbar_Frame->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.95f });
	spHPbar_Frame->SetEnable(true);
	m_vecHPbar.push_back(spHPbar_Frame);

	return S_OK;
}
HRESULT CPlayer_Eula::ReadySkillIcon()
{
	// E
	shared_ptr<CSkillIcon> spSkillIcon = CreateActor<CSkillIcon>(L"Eula_SkillIcon_E");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_S_Eula_");
	spSkillIcon->SetSize({ 70.f, 70.f });
	spSkillIcon->SetFontSize(0.75f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f - 21.f, Client::g_iWinSizeY * 0.92f - 12.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.83f, Client::g_iWinSizeY * 0.92f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	// Q
	spSkillIcon = CreateActor<CSkillIcon>(L"Eula_SkillIcon_Q");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_E_Eula_");
	spSkillIcon->SetSize({ 120.f, 120.f });
	spSkillIcon->SetFontSize(1.f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f + 98.f, Client::g_iWinSizeY * 0.92f - 30.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.93f, Client::g_iWinSizeY * 0.9f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	return S_OK;
}