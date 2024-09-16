#include "pch.h"
#include "Player_Nahida.h"

#include "RenderManager.h"
#include "CollisionManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"

#include "Level.h"
#include "Layer.h"
#include "SpriteActor.h"
#include "CaptureEffect.h"
#include "Seal.h"

#include "Nahida_StateMachine.h"
#include "AnimModel.h"
#include "Animation.h"
#include "Catalyst.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "PlayerAttackEffect.h"
#include "PlayerProjectileEffect.h"

#include "SkillIcon.h"
#include "HPbar.h"
#include "HPbar_Frame.h"

void CPlayer_Nahida::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Avatar_Loli_Catalyst_Nahida_Proto1");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Player_Nahida");

#pragma region 액터
	ReadyEffect();
	ReadySkillIcon();
	ReadyHPbar();

	m_fMaxHp = 15972.0f;
	m_fCurHp = 15972.0f;
	m_fAtk = 2438.f;
	m_fDef = 100.0f;
#pragma endregion 액터

#pragma region 컴포넌트
	// [ Component - Collider ]
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::AABB);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 70.0f, 0.0f });
	m_wpCollider.lock()->SetColliderExtents({ 35.f, 70.f, 35.f });
	CCollisionManager::GetInstance()->AddPlayerBodyCollider(m_wpCollider);

	// [ Component - AttackCollider ]
	m_wpNormalAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpNormalAttackCollider.lock()->SetColliderType(COLLIDER_TYPE::OBB);
	m_wpNormalAttackCollider.lock()->SetColliderCenter({ 0.0f, 100.0f, 200.0f });
	m_wpNormalAttackCollider.lock()->SetColliderExtents({ 50.f, 50.f, 250.f });
	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpNormalAttackCollider);

	// [ Component - SkillCollider ]
	m_wpSkillCollider = CreateActorComponent<CAttackCollider>(L"SkillCollider");
	m_wpSkillCollider.lock()->SetColliderType(COLLIDER_TYPE::OBB);
	m_wpSkillCollider.lock()->SetColliderCenter({ 0.0f, 100.0f, 800.0f });
	m_wpSkillCollider.lock()->SetColliderExtents({ 150.f, 150.f, 800.f });
	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpSkillCollider);

	// ================================================================================================
	m_wpWeapon = CreateActorComponent<CCatalyst>(L"Weapon");
	m_wpStateMachine = CreateActorComponent<CNahida_StateMachine>(L"PlayerStateMachine");
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
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::CLIMB_D]->SetIsLoop(false);

	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_NORMAL]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_TURN_L]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANIM::FLY_TURN_R]->SetIsLoop(true);

	// [ Component - StateMachine ]
	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());
	dynamic_pointer_cast<CNahida_StateMachine>(m_wpStateMachine.lock())->SetWeaponCollider(m_wpNormalAttackCollider);
	dynamic_pointer_cast<CNahida_StateMachine>(m_wpStateMachine.lock())->SetSkillCollider(m_wpSkillCollider);
#pragma endregion 컴포넌트
}

_int CPlayer_Nahida::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayer::PreUpdate(_fTimeDelta);
}

_int CPlayer_Nahida::Update(const _float& _fTimeDelta)
{
	return CPlayer::Update(_fTimeDelta);
}

_int CPlayer_Nahida::LateUpdate(const _float& _fTimeDelta)
{
	SelectBoneMatrixToWeapon();

	if (!m_wpNormalAttackCollider.expired())
		m_wpNormalAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

//#ifdef _DEBUG
//	if (!m_wpNormalAttackCollider.expired())
//		CRenderManager::GetInstance()->AddDebugComponent(m_wpCollider.lock());
//#endif // _DEBUG

	return CPlayer::LateUpdate(_fTimeDelta);
}

void CPlayer_Nahida::Render()
{
	CPlayer::Render();
}

void CPlayer_Nahida::EndPlay()
{
	CPlayer::EndPlay();
}

void CPlayer_Nahida::Release()
{
	CPlayer::Release();
}

void CPlayer_Nahida::SelectBoneMatrixToWeapon()
{
	_ullong PlayerState = m_wpStateMachine.lock()->GetCurState();

	if (PlayerState & PLAYER_STATE::IS_ATTACK)
	{
		m_wpWeapon.lock()->SetIsRender(false);
	}
	else
	{
		m_wpWeapon.lock()->SetIsRender(true);
	}

	BindWeaponToChest();
}

void CPlayer_Nahida::BindWeaponToChest()
{
	m_wpWeapon.lock()->SetRotation(XMVectorSet(-90.0f, 0.0f, 0.f, 0.0f));

	m_wpWeapon.lock()->SetActorTranslation(XMVectorSet(25.0f, -50.0f, 0.0f, 1.0f));
	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_Chest);
}

HRESULT CPlayer_Nahida::ReadyProjectileEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
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
HRESULT CPlayer_Nahida::ReadyPlayerAttackEffect_Sphere(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius, _float3 _vCenter)
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

HRESULT CPlayer_Nahida::ReadyEffect_Skill_E(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum)
{
	shared_ptr<CCaptureEffect> spCaptureEffect = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CCaptureEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spCaptureEffect)
		return E_FAIL;

	spCaptureEffect->SetJsonFileName(_wstrJsonFileName);
	spCaptureEffect->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

	if (FAILED(spCaptureEffect->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spCaptureEffect->GetActorName());
	}

	return S_OK;
}

HRESULT CPlayer_Nahida::ReadyEffect_Skill_E_Seal(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum)
{
	shared_ptr<CSeal> spSeal = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CSeal>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spSeal)
		return E_FAIL;

	spSeal->SetJsonFileName(_wstrJsonFileName);
	// spSeal->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

	if (FAILED(spSeal->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spSeal->GetActorName());
	}

	return S_OK;
}

void CPlayer_Nahida::ReadyEffect()
{
	// 파티클
	for (_uint i = 0; i < 5; ++i)
	{
		ReadyParticle(L"Particle_Nahida_NormalAttack_One", i);
	}

	// 기본 공격
	for (_uint i = 0; i < 10; ++i)
	{
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Two", i);
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Three", i);
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Four", i);
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Seven", i);

		ReadyEffectMesh(L"Nahida_NormalAttack_Floor", i);
	}
	for (_uint i = 0; i < 20; ++i)
	{
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_One", i);
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Five", i);
		ReadyEffectMesh(L"Nahida_NormalAttack_Flower_Six", i);

		ReadyEffect_Skill_E_Seal(L"Nahida_Skill_E_Seal", i);
	}

	// E
	ReadyEffect_Skill_E(L"Nahida_Skill_E_Frame", 0);
	ReadyEffect_Skill_E(L"Nahida_Skill_E_Frame_One", 0);
	ReadyEffect_Skill_E(L"Nahida_Skill_E_Frame_Two_FlowerDeco", 0);
	// ReadyEffect_Skill_E(L"Nahida_Skill_E_RotateCard", 0);

	// Q
	for (_uint i = 0; i < 2; ++i)
	{
		ReadyEffectMesh(L"Nahida_Skill_Q_Palace", 0);
	}
}
HRESULT CPlayer_Nahida::ReadyHPbar()
{
	shared_ptr<CHPbar> spHPbar = CreateActor<CHPbar>(L"Nahida_Hpbar");
	if (nullptr == spHPbar)
		return E_FAIL;

	spHPbar->SetShaderFileName(L"Shader_UI_HPbar");
	spHPbar->SetTextureFileName(L"UI_HPBar_");
	spHPbar->SetSize({ 255.f, 13.f });
	spHPbar->SetPosition({ Client::g_iWinSizeX * 0.5f, Client::g_iWinSizeY * 0.95f });
	spHPbar->SetEnable(true);
	m_vecHPbar.push_back(spHPbar);

	shared_ptr<CHPbar_Frame> spHPbar_Frame = CreateActor<CHPbar_Frame>(L"Nahida_Hpbar_Frame");
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
HRESULT CPlayer_Nahida::ReadySkillIcon()
{
	// E
	shared_ptr<CSkillIcon> spSkillIcon = CreateActor<CSkillIcon>(L"Nahida_SkillIcon_E");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_S_Nahida_");
	spSkillIcon->SetSize({ 70.f, 70.f });
	spSkillIcon->SetFontSize(0.75f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f - 21.f, Client::g_iWinSizeY * 0.92f - 12.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.83f, Client::g_iWinSizeY * 0.92f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	// Q
	spSkillIcon = CreateActor<CSkillIcon>(L"Nahida_SkillIcon_Q");
	if (nullptr == spSkillIcon)
	{
		MSG_BOX("Failed to Create SkillIcon : CPlayer");
		return E_FAIL;
	}

	spSkillIcon->SetShaderFileName(L"Shader_UI_SkillIcon");
	spSkillIcon->SetTextureFileName(L"Skill_E_Nahida_");
	spSkillIcon->SetSize({ 120.f, 120.f });
	spSkillIcon->SetFontSize(1.f);
	spSkillIcon->SetFontPosition({ Client::g_iWinSizeX * 0.83f + 98.f, Client::g_iWinSizeY * 0.92f - 30.f });
	spSkillIcon->SetPosition({ Client::g_iWinSizeX * 0.93f, Client::g_iWinSizeY * 0.9f });
	spSkillIcon->SetEnable(true);
	m_vecSkillIcons.push_back(spSkillIcon);

	return S_OK;
}
