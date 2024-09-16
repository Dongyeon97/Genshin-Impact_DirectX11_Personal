#include "pch.h"
#include "Player_Ambor.h"

#include "CollisionManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"

#include "Level.h"
#include "Layer.h"
#include "SpriteActor.h"
#include "ArrowRain_Floor.h"

#include "Ambor_StateMachine.h"
#include "AnimModel.h"
#include "Animation.h"
#include "Amos.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "PlayerAttackEffect.h"
#include "PlayerProjectileEffect.h"

#include "SkillIcon.h"
#include "HPbar.h"
#include "HPbar_Frame.h"

void CPlayer_Ambor::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Player_Ambor_Proto1");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Player_Ambor");

#pragma region 액터
	// UI, Effect
	ReadyEffect();
	ReadySkillIcon();
	ReadyHPbar();

	m_fMaxHp = 17338.0f;
	m_fCurHp = 17338.0f;
	m_fAtk = 1438.f;
	m_fDef = 100.0f;
#pragma endregion 액터

#pragma region 컴포넌트
	// [ Component - Collider ]
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 70.0f, 0.0f });
	m_wpCollider.lock()->SetColliderExtents({ 35.f, 70.f, 35.f });
	CCollisionManager::GetInstance()->AddPlayerBodyCollider(m_wpCollider);

	//// [ Component - SkillCollider ]
	//m_wpSkillCollider = CreateActorComponent<CAttackCollider>(L"SkillCollider");
	//m_wpSkillCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	//m_wpSkillCollider.lock()->SetColliderCenter({ 0.0f, 100.0f, 100.0f });
	//m_wpSkillCollider.lock()->SetColliderExtents({ 200.f, 100.f, 200.f });
	//CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpSkillCollider);

	// ================================================================================================
	m_wpWeapon = CreateActorComponent<CAmos>(L"Weapon");
	m_wpStateMachine = CreateActorComponent<CAmbor_StateMachine>(L"PlayerStateMachine");
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
	dynamic_pointer_cast<CAmbor_StateMachine>(m_wpStateMachine.lock())->SetWeaponCollider(m_wpWeapon.lock()->GetWeaponCollider());
	// dynamic_pointer_cast<CAmbor_StateMachine>(m_wpStateMachine.lock())->SetSkillCollider(m_wpSkillCollider);
#pragma endregion 컴포넌트
}

_int CPlayer_Ambor::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayer::PreUpdate(_fTimeDelta);
}

_int CPlayer_Ambor::Update(const _float& _fTimeDelta)
{
	return CPlayer::Update(_fTimeDelta);
}

_int CPlayer_Ambor::LateUpdate(const _float& _fTimeDelta)
{
	SelectBoneMatrixToWeapon();

	return CPlayer::LateUpdate(_fTimeDelta);
}

void CPlayer_Ambor::Render()
{
	CPlayer::Render();
}

void CPlayer_Ambor::EndPlay()
{
	CPlayer::EndPlay();
}

void CPlayer_Ambor::Release()
{
	CPlayer::Release();
}

void CPlayer_Ambor::SelectBoneMatrixToWeapon()
{
	_ullong PlayerState = m_wpStateMachine.lock()->GetCurState();

	if (PlayerState & PLAYER_STATE::IS_ATTACK)
	{
		// 평타 1,2,3,4,5타
		if (PlayerState & PLAYER_STATE::IS_NORMALATTACK)
		{
			BindWeaponToLeftArm();
		}
		else if (PlayerState & PLAYER_STATE::ELEMENTAL_CLICK)
		{
			BindWeaponToChest();
		}
		else
		{
			BindWeaponToChest();
		}
	}
	else
	{
		BindWeaponToChest();
	}
}

HRESULT CPlayer_Ambor::ReadyProjectileEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
{
	shared_ptr<CPlayerProjectileEffect> spProjectileEffect = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CPlayerProjectileEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spProjectileEffect)
		return E_FAIL;

	spProjectileEffect->SetJsonFileName(_wstrJsonFileName);
	spProjectileEffect->SetAttackColliderType(COLLIDER_TYPE::SPHERE);
	spProjectileEffect->SetColliderRadius(_fRadius);
	spProjectileEffect->SetColliderCenter(_vCenter);
	spProjectileEffect->SetColliderOn(false);

	if (FAILED(spProjectileEffect->Initialize(_wstrJsonFileName)))
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spProjectileEffect->GetActorName());
	
	return S_OK;
}
HRESULT CPlayer_Ambor::ReadyPlayerAttackEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
{
	shared_ptr<CPlayerAttackEffect> spAttackEffect = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CPlayerAttackEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spAttackEffect)
		return E_FAIL;

	spAttackEffect->SetJsonFileName(_wstrJsonFileName);
	spAttackEffect->SetAttackColliderType(COLLIDER_TYPE::SPHERE);
	spAttackEffect->SetColliderRadius(_fRadius);
	spAttackEffect->SetColliderCenter(_vCenter);
	spAttackEffect->SetColliderOn(false);

	if (FAILED(spAttackEffect->Initialize(_wstrJsonFileName)))
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spAttackEffect->GetActorName());

	return S_OK;
}

void CPlayer_Ambor::ReadyEffect()
{
	// 스프라이트
	for (_uint i = 0; i < 5; ++i)
	{
		ReadySpriteEffect(L"Sprite_Ambor_AmosAttack", i, 3, 3, 0.02f);
		ReadySpriteEffect(L"Sprite_Ambor_FireAttack", i, 2, 2, 0.08f);
		ReadySpriteEffect(L"Sprite_Ambor_NormalAttack_BlackCut", i, 4, 2, 0.06f);
	}

	// 파티클
	for (_uint i = 0; i < 5; ++i)
	{
		ReadyParticle(L"Particle_Ambor_NormalAttack_One", i);
		ReadyParticle(L"Particle_Ambor_NormalAttack_Two", i);
	}

	// E
	ReadyEffectMesh(L"Ambor_Skill_E_Aura_One", 0);
	ReadyEffectMesh(L"Ambor_Skill_E_Aura_Two", 0);
	for (_uint i = 0; i < 3; ++i)
	{
		ReadySpriteEffect(L"Sprite_Ambor_NormalAttack_Fire", i, 2, 2, 0.05f);
	}

	// Q
	ReadyParticle(L"Particle_Ambor_Skill_Q", 0);
	ReadyArrowRain(L"Ambor_Skill_Q_Floor_One", 0, 25.f, _float3(0.f, 0.f, 0.f));
	ReadyArrowRain(L"Ambor_Skill_Q_Floor_Two", 0, 25.f, _float3(0.f, 0.f, 0.f));
	ReadyArrowRain(L"Ambor_Skill_Q_Floor_Three", 0, 25.f, _float3(0.f, 0.f, 0.f));

	// 투사체
	for (_uint i = 0; i < 10; ++i)
	{
		ReadyProjectileEffect_Sphere(L"Ambor_NormalAtk_One", i, 4.f, _float3(0.f, 0.f, 0.f));
		ReadyProjectileEffect_Sphere(L"Ambor_NormalAtk_Fire", i, 4.f, _float3(0.f, 0.f, 0.f));
	}
}
HRESULT CPlayer_Ambor::ReadyHPbar()
{
	shared_ptr<CHPbar> spHPbar = CreateActor<CHPbar>(L"Ambor_Hpbar");
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_HPbar");
	spHPbar->SetTextureFileName(L"UI_HPBar_");
	spHPbar->SetSize({ 255.f, 13.f });
	spHPbar->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.95f });
	spHPbar->SetEnable(true);
	m_vecHPbar.push_back(spHPbar);

	shared_ptr<CHPbar_Frame> spHPbar_Frame = CreateActor<CHPbar_Frame>(L"Ambor_Hpbar_Frame");
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
HRESULT CPlayer_Ambor::ReadySkillIcon()
{
	// E
	shared_ptr<CSkillIcon> spSkillIcon = CreateActor<CSkillIcon>(L"Ambor_SkillIcon_E");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_S_Ambor_");
	spSkillIcon->SetSize({ 70.f, 70.f });
	spSkillIcon->SetFontSize(0.75f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f - 21.f, Client::g_iWinSizeY * 0.92f - 12.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.83f, Client::g_iWinSizeY * 0.92f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	// Q
	spSkillIcon = CreateActor<CSkillIcon>(L"Ambor_SkillIcon_Q");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_E_Ambor_HD");
	spSkillIcon->SetSize({ 120.f, 120.f });
	spSkillIcon->SetFontSize(1.f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f + 98.f, Client::g_iWinSizeY * 0.92f - 30.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.93f, Client::g_iWinSizeY * 0.9f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	return S_OK;
}
HRESULT CPlayer_Ambor::ReadyArrowRain(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
{
	shared_ptr<CArrowRain_Floor> spArrowRain = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CArrowRain_Floor>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spArrowRain)
		return E_FAIL;

	spArrowRain->SetJsonFileName(_wstrJsonFileName);
	spArrowRain->SetAttackColliderType(COLLIDER_TYPE::SPHERE);
	spArrowRain->SetColliderRadius(_fRadius);
	spArrowRain->SetColliderCenter(_vCenter);
	spArrowRain->SetColliderOn(false);

	ATTACK_DESC AttackDesc;
	AttackDesc = { L"Player_Ambor", L"Player_Ambor Skill2", PYRO, STRONG,	false,	1438.f * 2.0f };
	spArrowRain->SetAttackDesc(AttackDesc);

	if (FAILED(spArrowRain->Initialize(_wstrJsonFileName)))
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spArrowRain->GetActorName());

	return S_OK;
}
