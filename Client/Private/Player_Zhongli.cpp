#include "pch.h"
#include "Player_Zhongli.h"

#include "CollisionManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"

#include "Level.h"
#include "Layer.h"

#include "Jusin.h"
#include "Shield.h"

#include "Zhongli_StateMachine.h"
#include "AnimModel.h"
#include "Animation.h"
#include "Kunwu.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "SkillIcon.h"
#include "HPbar.h"
#include "HPbar_Frame.h"

void CPlayer_Zhongli::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Player_Zhongli_Proto1");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Player_Zhongli");

#pragma region 액터
	// UI, Effect
	ReadyEffect();
	ReadySkillIcon();
	ReadyHPbar();

	m_fMaxHp = 43254.0f;
	m_fCurHp = 43254.0f;
	m_fAtk = 2470.0f;
	m_fDef = 200.0f;
#pragma endregion 액터

#pragma region 컴포넌트
	// [ Component - Collider ]
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 70.0f, 0.0f });
	m_wpCollider.lock()->SetColliderExtents({ 70.f, 70.f, 70.f });
	CCollisionManager::GetInstance()->AddPlayerBodyCollider(m_wpCollider);

	// [ Component - SkillCollider ]
	m_wpSkillCollider = CreateActorComponent<CAttackCollider>(L"SkillCollider");
	m_wpSkillCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpSkillCollider.lock()->SetColliderCenter({ 0.0f, 100.0f, 0.0f });
	m_wpSkillCollider.lock()->SetColliderExtents({ 400.f, 100.f, 400.f });
	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpSkillCollider);

	// ================================================================================================
	m_wpWeapon = CreateActorComponent<CKunwu>(L"Weapon");
	m_wpStateMachine = CreateActorComponent<CZhongli_StateMachine>(L"PlayerStateMachine");
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
	dynamic_pointer_cast<CZhongli_StateMachine>(m_wpStateMachine.lock())->SetWeaponCollider(m_wpWeapon.lock()->GetWeaponCollider());
	dynamic_pointer_cast<CZhongli_StateMachine>(m_wpStateMachine.lock())->SetSkillCollider(m_wpSkillCollider);
#pragma endregion 컴포넌트
}

_int CPlayer_Zhongli::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayer::PreUpdate(_fTimeDelta);
}

_int CPlayer_Zhongli::Update(const _float& _fTimeDelta)
{
	return CPlayer::Update(_fTimeDelta);
}

_int CPlayer_Zhongli::LateUpdate(const _float& _fTimeDelta)
{
	SelectBoneMatrixToWeapon();

	return CPlayer::LateUpdate(_fTimeDelta);
}

void CPlayer_Zhongli::Render()
{
	CPlayer::Render();
}

void CPlayer_Zhongli::EndPlay()
{
	CPlayer::EndPlay();
}

void CPlayer_Zhongli::Release()
{
	CPlayer::Release();
}

void CPlayer_Zhongli::SelectBoneMatrixToWeapon()
{
	_ullong PlayerState = m_wpStateMachine.lock()->GetCurState();

	if (PlayerState & PLAYER_STATE::IS_ATTACK)
	{
		// 평타 1,4,5타
		if (PlayerState & PLAYER_STATE::ATTACK_1 ||
			PlayerState & PLAYER_STATE::ATTACK_4 ||
			PlayerState & PLAYER_STATE::ATTACK_5)
		{
			m_wpWeapon.lock()->SetIsRender(false);
		}
		// 평타 2,3타
		else if (PlayerState & PLAYER_STATE::ATTACK_2 ||
			PlayerState & PLAYER_STATE::ATTACK_3)
		{
			BindWeaponToRightArm();
		}
		else if (PlayerState & PLAYER_STATE::IS_SKILL)
		{
			m_wpWeapon.lock()->SetIsRender(false);
		}
	}
	else
	{
		BindWeaponToChest();
		m_wpWeapon.lock()->SetIsRender(true);
	}
}

void CPlayer_Zhongli::ReadyEffect()
{
	// 파티클
	ReadySpriteEffect(L"SprtieParticle_Zhongli_Jusin_One", 0, 6, 6, 0.0025f);

	// E
	ReadyShield(L"Zhongli_Skill_E_ShieldCylinder", 0);
	ReadyShield(L"Zhongli_Skill_E_ShieldRing_One", 0);
	ReadyShield(L"Zhongli_Skill_E_ShieldRing_Two", 0);
	ReadyEffectMesh(L"Zhongli_Skill_E_ShockWave", 0);
	ReadyEffectMesh(L"Zhongli_Skill_E_Floor", 0);

	// Q
	ReadyJusin(L"Zhongli_Skill_Q_Jusin", 0, 100.f, _float3(0.f, 0.f, 0.f));

	ReadyEffectMesh(L"Zhongli_Skill_Q_ShockWave", 0);
	ReadyEffectMesh(L"Zhongli_Skill_Q_ShockWave_Two", 0);
	ReadyEffectMesh(L"Zhongli_Skill_Q_Floor", 0);
}
HRESULT CPlayer_Zhongli::ReadyShield(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum)
{
	shared_ptr<CShield> spShield = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CShield>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spShield)
		return E_FAIL;

	spShield->SetJsonFileName(_wstrJsonFileName);
	spShield->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

	if (FAILED(spShield->Initialize(_wstrJsonFileName)))
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spShield->GetActorName());

	return S_OK;
}
HRESULT CPlayer_Zhongli::ReadyJusin(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
{
	shared_ptr<CJusin> spJusin = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CJusin>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spJusin)
		return E_FAIL;

	spJusin->SetJsonFileName(_wstrJsonFileName);
	spJusin->SetAttackColliderType(COLLIDER_TYPE::SPHERE);
	spJusin->SetColliderRadius(_fRadius);
	spJusin->SetColliderCenter(_vCenter);
	spJusin->SetColliderOn(false);
	spJusin->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

	if (FAILED(spJusin->Initialize(_wstrJsonFileName)))
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spJusin->GetActorName());

	return S_OK;
}
HRESULT CPlayer_Zhongli::ReadyHPbar()
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
HRESULT CPlayer_Zhongli::ReadySkillIcon()
{
	// E
	shared_ptr<CSkillIcon> spSkillIcon = CreateActor<CSkillIcon>(L"Zhongli_SkillIcon_E");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_S_Zhongli_");
	spSkillIcon->SetSize({ 70.f, 70.f });
	spSkillIcon->SetFontSize(0.75f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f - 21.f, Client::g_iWinSizeY * 0.92f - 12.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.83f, Client::g_iWinSizeY * 0.92f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	// Q
	spSkillIcon = CreateActor<CSkillIcon>(L"Zhongli_SkillIcon_Q");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_E_Zhongli_");
	spSkillIcon->SetSize({ 120.f, 120.f });
	spSkillIcon->SetFontSize(1.f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f + 98.f, Client::g_iWinSizeY * 0.92f - 30.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.93f, Client::g_iWinSizeY * 0.9f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	return S_OK;
}