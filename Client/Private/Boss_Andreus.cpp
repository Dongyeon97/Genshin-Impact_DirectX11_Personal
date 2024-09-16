#include "pch.h"
#include "Boss_Andreus.h"

#include "RenderManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"
#include "FontManager.h"
#include "InputManager.h"

#include "Level.h"
#include "Layer.h"

#include "ParticleActor.h"
#include "EffectMeshActor.h"
#include "SpriteActor.h"
#include "MovingEffect.h"
#include "MonsterAttackEffect.h"
#include "MonsterProjectileEffect.h"

#include "AnimModel.h"
#include "Animation.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "AndreusStateMachine.h"

void CBoss_Andreus::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Monster_Wolf_LupiBoreas (merge)");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Andreus");

#pragma region 컴포넌트
	m_wpStateMachine = CreateActorComponent<CAndreusStateMachine>(L"AndreusStateMachine");

	// 몬스터의 기본 콜라이더 장착
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 1.0f, 0.0f });
	m_wpCollider.lock()->SetColliderRadius(3.0f);
	CCollisionManager::GetInstance()->AddMonsterBodyCollider(m_wpCollider);

	// AttackCollider
	m_spLeftArm_AttackCollider = make_shared<CAttackCollider>();
	m_spLeftArm_AttackCollider->SetComponentName(L"LeftArm_AttackCollider");
	m_spLeftArm_AttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spLeftArm_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spLeftArm_AttackCollider->SetColliderRadius(3.5f);
	m_spLeftArm_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spLeftArm_AttackCollider->Initialize();

	m_spRightArm_AttackCollider = make_shared<CAttackCollider>();
	m_spRightArm_AttackCollider->SetComponentName(L"RightArm_AttackCollider");
	m_spRightArm_AttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spRightArm_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spRightArm_AttackCollider->SetColliderRadius(3.5f);
	m_spRightArm_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spRightArm_AttackCollider->Initialize();

	m_spTail_AttackCollider = make_shared<CAttackCollider>();
	m_spTail_AttackCollider->SetComponentName(L"Tail_AttackCollider");
	m_spTail_AttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spTail_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spTail_AttackCollider->SetColliderRadius(3.0f);
	m_spTail_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spTail_AttackCollider->Initialize();

	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spLeftArm_AttackCollider);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spRightArm_AttackCollider);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spTail_AttackCollider);
#pragma endregion 컴포넌트

#pragma region 액터
	ReadyEffect();
	ReadyHPbar();

	m_tInfo.m_eMonsterType = MONSTER_TYPE::BOSS;
	m_tInfo.m_eHitType = HIT_TYPE::SUPERARMOR;
	m_tInfo.m_eElementalType = ELEMENTAL_TYPE::CRYO;

	m_fMaxHp = 1000000.0f;
	m_fCurHp = 1000000.0f;
	m_fAtk = 3000.0f;
	m_fDef = 100.0f;
	m_fMoveSpeed = 200.0f;

	m_tInfo.m_fDetectingRadius = 2000.0f;
	// =============Attack============== //
	m_tInfo.m_fAttack1Radius = 900.0f;
	m_tInfo.m_fAttack2Radius = 900.0f;
	m_tInfo.m_fAttack3Radius = 2000.0f;
	m_tInfo.m_fAttack4Radius = 1200.0f;
	m_tInfo.m_fAttack5Radius = 500.0f;

	m_tInfo.m_fAttack1RunTime = 4.0f;
	m_tInfo.m_fAttack2RunTime = 5.5f;
	m_tInfo.m_fAttack3RunTime = 4.0f;
	m_tInfo.m_fAttack4RunTime = 0.0f;
	m_tInfo.m_fAttack5RunTime = 0.0f;

	m_tInfo.m_fAttack1CoolTime = 0.0f;
	m_tInfo.m_fAttack2CoolTime = 0.0f;
	m_tInfo.m_fAttack3CoolTime = 0.0f;
	m_tInfo.m_fAttack4CoolTime = 0.0f;
	m_tInfo.m_fAttack5CoolTime = 0.0f;

	m_tInfo.m_fAttack1MinTime = 8.0f;
	m_tInfo.m_fAttack2MinTime = 10.0f;
	m_tInfo.m_fAttack3MinTime = 12.5f;
	m_tInfo.m_fAttack4MinTime = 17.5f;
	m_tInfo.m_fAttack5MinTime = 17.5f;

	// =============Pattern============== //
	m_tInfo.m_fPattern1Radius = 5000.0f;
	m_tInfo.m_fPattern2Radius = 1500.0f;
	m_tInfo.m_fPattern3Radius = 1500.0f;
	m_tInfo.m_fPattern4Radius = 5000.0f;
	m_tInfo.m_fPattern5Radius = 5000.0f;

	m_tInfo.m_fPattern1RunTime = 0.0f;
	m_tInfo.m_fPattern2RunTime = 0.0f;
	m_tInfo.m_fPattern3RunTime = 0.0f;
	m_tInfo.m_fPattern4RunTime = 0.0f;
	m_tInfo.m_fPattern5RunTime = 0.0f;

	m_tInfo.m_fPattern1CoolTime = 0.0f;
	m_tInfo.m_fPattern2CoolTime = 0.0f;
	m_tInfo.m_fPattern3CoolTime = 0.0f;
	m_tInfo.m_fPattern4CoolTime = 0.0f;
	m_tInfo.m_fPattern5CoolTime = 0.0f;

	m_tInfo.m_fPattern1MinTime = 5.0f;		// 고드름 생성 시간
	m_tInfo.m_fPattern2MinTime = 15.0f;		// 바람 칼날 6갈래
	m_tInfo.m_fPattern3MinTime = 15.0f;		// 아이스 웨이브
	m_tInfo.m_fPattern4MinTime = 45.0f;		// 하울링 - 가만히 서서 하울링 모션 취하면서 전범위 광역 바람공격 -> 못피함
	m_tInfo.m_fPattern5MinTime = 8.0f;		// 꼬마 늑대 소환 시간

	m_tInfo.m_fHit1MinTime = 1.0f;
	m_tInfo.m_fHit1RunTime = 0.0f;

	m_tInfo.m_fHit2MinTime = 1.0f;
	m_tInfo.m_fHit2RunTime = 0.0f;

	m_tInfo.m_fHit3MinTime = 1.0f;
	m_tInfo.m_fHit3RunTime = 0.0f;

	dynamic_pointer_cast<CMonsterStateMachine>(m_wpStateMachine.lock())->SetMonsterInfo(m_tInfo);
#pragma endregion 액터

	CBossMonster::Initialize();

#pragma region 컴포넌트
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::BATTLE_IDLE01]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::BATTLE_IDLE02]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::BATTLE_IDLE03]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::STANDBY]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::WALK]->SetIsLoop(true);
	m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::RUN]->SetIsLoop(true);
	// m_wpAnimModel.lock()->GetAnimVector()[ANDREUS_ANIM::HOWL]->SetIsLoop(true);
	m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::STANDBY);

	m_wpAnimModel.lock()->SetIsRootMotion(false, ANDREUS_ANIM::WALK);
	m_wpAnimModel.lock()->SetIsRootMotion(false, ANDREUS_ANIM::RUN);

	m_wpAnimModel.lock()->SetRootMotionRatio({ 0.0f, 2.45f, 0.0f }, ANDREUS_ANIM::WALK);
	m_wpAnimModel.lock()->SetRootMotionRatio({ 0.0f, 2.45f, 0.0f }, ANDREUS_ANIM::RUN);
	m_wpAnimModel.lock()->SetRootMotionRatio({ 0.2f, 1.0f, 0.2f }, ANDREUS_ANIM::JUMP_BACK);

	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());

	dynamic_pointer_cast<CAndreusStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"LeftArm", m_spLeftArm_AttackCollider);
	dynamic_pointer_cast<CAndreusStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"RightArm", m_spRightArm_AttackCollider);
	dynamic_pointer_cast<CAndreusStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"Tail", m_spTail_AttackCollider);

	ReadyBoneMatrix();
#pragma endregion 컴포넌트
}

_int CBoss_Andreus::PreUpdate(const _float& _fTimeDelta)
{
	return CBossMonster::PreUpdate(_fTimeDelta);
}

_int CBoss_Andreus::Update(const _float& _fTimeDelta)
{
	UpdateColliderTransform();

	if (CInputManager::GetInstance()->IsKeyDown(DIK_P))
		m_fCurHp = m_fMaxHp * 0.1f;

	return CBossMonster::Update(_fTimeDelta);
}

_int CBoss_Andreus::LateUpdate(const _float& _fTimeDelta)
{
	m_spLeftArm_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matLeftArm));
	m_spLeftArm_AttackCollider->LateUpdate(_fTimeDelta);

	m_spRightArm_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matRightArm));
	m_spRightArm_AttackCollider->LateUpdate(_fTimeDelta);

	m_spTail_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matTail));
	m_spTail_AttackCollider->LateUpdate(_fTimeDelta);

#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_spLeftArm_AttackCollider);
	CRenderManager::GetInstance()->AddDebugComponent(m_spRightArm_AttackCollider);
	CRenderManager::GetInstance()->AddDebugComponent(m_spTail_AttackCollider);
#endif // _DEBUG

	return CBossMonster::LateUpdate(_fTimeDelta);
}

void CBoss_Andreus::Render()
{
	CBossMonster::Render();
}

void CBoss_Andreus::EndPlay()
{
	CBossMonster::EndPlay();
}

void CBoss_Andreus::Release()
{
	if (nullptr != m_spLeftArm_AttackCollider)
	{
		m_spLeftArm_AttackCollider->Release();
		m_spLeftArm_AttackCollider.reset();
	}

	if (nullptr != m_spRightArm_AttackCollider)
	{
		m_spRightArm_AttackCollider->Release();
		m_spRightArm_AttackCollider.reset();
	}

	if (nullptr != m_spTail_AttackCollider)
	{
		m_spTail_AttackCollider->Release();
		m_spTail_AttackCollider.reset();
	}

	CBossMonster::Release();
}

void CBoss_Andreus::ReadyBoneMatrix()
{
	m_pSocketMatrix_LeftArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("Bone_L_Hand");
	m_pSocketMatrix_RightArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("Bone_R_Hand");
	m_pSocketMatrix_Tail = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("Bone_Tail_04");
}

void CBoss_Andreus::UpdateColliderTransform()
{
	// 붙어있는 뼈의 행렬을 가져온다.
	_matrix	LeftArm_BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix_LeftArm);
	_matrix	RightArm_BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix_RightArm);
	_matrix Tail_BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix_Tail);

	// 행렬을 정규화 한다.
	for (size_t i = 0; i < 3; i++)
	{
		LeftArm_BoneMatrix.r[i] = XMVector3Normalize(LeftArm_BoneMatrix.r[i]);
		RightArm_BoneMatrix.r[i] = XMVector3Normalize(RightArm_BoneMatrix.r[i]);
		Tail_BoneMatrix.r[i] = XMVector3Normalize(Tail_BoneMatrix.r[i]);
	}

	// 자기 -> 뼈(1차 부모) -> 액터(2차 부모) 순서대로 월드를 구한다.
	XMStoreFloat4x4(&m_matLeftArm, LeftArm_BoneMatrix * XMLoadFloat4x4(&m_matWorld));
	XMStoreFloat4x4(&m_matRightArm, RightArm_BoneMatrix * XMLoadFloat4x4(&m_matWorld));
	XMStoreFloat4x4(&m_matTail, Tail_BoneMatrix * XMLoadFloat4x4(&m_matWorld));
}

void CBoss_Andreus::ReadyEffect()
{
	// 스프라이트
	for (_uint i = 0; i < 20; ++i)
	{
		ReadyParticle(L"Sprite_AttackStar_One", i);
	}
	// 파티클

	for (_uint i = 0; i < 5; ++i)
	{
		ReadyParticle(L"Particle_Andrius_NormalAttack_One", i);
		ReadyParticle(L"Particle_Andrius_Attack_Turn", i);
		ReadyParticle(L"Particle_Andrius_Attack_Turn_Two", i);
		ReadyParticle(L"Particle_Andrius_PaizeChange", i);
		ReadyParticle(L"Particle_Andrius_Attack_Three", i);
	}

	// 파편
	for (_uint i = 0; i < 20; ++i)
	{
		ReadyMovingEffect(L"Andrius_WolfStone", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_One", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_Two", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_Three", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_Four", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_Five", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_WolfStone_Six", i, _float2(50.f, 80.f));
	}

	// Attack1 - 할퀴기
	ReadyEffectMesh(L"Andrius_Attack_One_L", 0);
	ReadyEffectMesh(L"Andrius_Attack_Two_L", 0);
	ReadyEffectMesh(L"Andrius_Attack_One_R", 0);
	ReadyEffectMesh(L"Andrius_Attack_Two_R", 0);

	// Attack2 - 할퀴기
	ReadyEffectMesh(L"Andrius_Attack_Tail_L", 0);
	ReadyEffectMesh(L"Andrius_Attack_Tail_R", 0);

	// Attack3 - 날라올라 내려찍기
	for (_uint i = 0; i < 2; ++i)
	{
		ReadyEffectMesh(L"Andrius_Attack_Three_IceField_Floor", 0);
		ReadyEffectMesh(L"Andrius_Attack_Three_IceField_Niddle", 0);
		ReadyEffectMesh(L"Andrius_Attack_Three_IceField_Aura", 0);
	}

	// Attack4 - 한바퀴 돌기
	ReadyEffectMesh(L"Andrius_Attack_Turn", 0);

	// =============================페이즈 전환===================================
	ReadyEffectMesh(L"Andrius_PaizeChange_AuraFloor", 0);
	ReadyEffectMesh(L"Andrius_PaizeChange_Tonado", 0);
	for (_uint i = 0; i < 3; ++i)
	{
		ReadyEffectMesh(L"Andrius_PaizeChange_Thunder_One", i);
		ReadyEffectMesh(L"Andrius_PaizeChange_Thunder_Two", i);
		ReadyEffectMesh(L"Andrius_PaizeChange_Thunder_Three", i);
	}
	for (_uint i = 0; i < 10; ++i)
	{
		ReadySpriteEffects(L"Sprite_Andrius_Thunder_One", i, 4, 4, 0.1f);
	}
	// ======================================================================

	// Pattern2 - 윈드 블레이드
	for (_uint i = 0; i < 6; ++i)
		ReadyProjectileEffect_OBB(L"Andrius_Pattern_Two_WindBlade", i, _float3(100.f, 200.f, 40.f), _float3(-200.f, 50.f, 0.f));

	// Pattern3 - 아이스 웨이브
	for (_uint i = 0; i < 10; ++i)
		ReadyAttackEffect(L"Andrius_Pattern_Three_IceWave", i, 50.f);

	for(_uint i = 0; i < 10; ++i)
		ReadyEffectMesh(L"Andrius_Pattern_Three_IceFloor", i);

	for (_uint i = 0; i < 10; ++i)
		ReadyParticle(L"Particle_Andrius_Pattern_Three_IceWave", i);

	for (_uint i = 0; i < 20; ++i)
	{
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_One", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_Two", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_Three", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_Four", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_Five", i, _float2(50.f, 80.f));
		ReadyMovingEffect(L"Andrius_Pattern_Three_IceDebris_Six", i, _float2(50.f, 80.f));
	}
}

HRESULT CBoss_Andreus::ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum)
{
	shared_ptr<CParticleActor> spParticleActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->CreateActorToPool<CParticleActor>(_wstrJsonFileName + to_wstring(_iParticleNum));
	if (nullptr == spParticleActor)
	{
		MSG_BOX("Failed to Create Particle : CBoss_Andreus");
		return E_FAIL;
	}

	spParticleActor->SetJsonFileName(_wstrJsonFileName);

	if (FAILED(spParticleActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spParticleActor->GetActorName());
	}

	return S_OK;
}
HRESULT CBoss_Andreus::ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum)
{
	shared_ptr<CEffectMeshActor> spEffectMeshActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CEffectMeshActor>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spEffectMeshActor)
		return E_FAIL;

	spEffectMeshActor->SetJsonFileName(_wstrJsonFileName);

	if (FAILED(spEffectMeshActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spEffectMeshActor->GetActorName());
	}

	return S_OK;
}
HRESULT CBoss_Andreus::ReadyMovingEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float2 _vMinMaxSpeed)
{
	shared_ptr<CMovingEffect> spMovingEffect = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CMovingEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spMovingEffect)
		return E_FAIL;

	spMovingEffect->SetJsonFileName(_wstrJsonFileName);
	spMovingEffect->SetSpeed(_vMinMaxSpeed.x, _vMinMaxSpeed.y);
	spMovingEffect->SetIsGravity(true);
	spMovingEffect->SetGravityValue(120.f);
	spMovingEffect->SetWidth(_float2(-30.f, 30.f));
	spMovingEffect->SetHeight(_float2(100.f, 120.f));
	spMovingEffect->SetDepth(_float2(-30.f, 30.f));

	if (FAILED(spMovingEffect->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spMovingEffect->GetActorName());
	}

	return S_OK;
}
HRESULT CBoss_Andreus::ReadyAttackEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius)
{
	shared_ptr<CMonsterAttackEffect> spAttackEffectActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CMonsterAttackEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spAttackEffectActor)
		return E_FAIL;

	spAttackEffectActor->SetJsonFileName(_wstrJsonFileName);
	spAttackEffectActor->SetColliderRadius(_fRadius);

	if (FAILED(spAttackEffectActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spAttackEffectActor->GetActorName());
	}

	return S_OK;
}
HRESULT CBoss_Andreus::ReadyProjectileEffect_OBB(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float3 _vExtents, _float3 _vCenter)
{
	shared_ptr<CMonsterProjectileEffect> spProjectileEffect = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CMonsterProjectileEffect>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spProjectileEffect)
		return E_FAIL;

	spProjectileEffect->SetJsonFileName(_wstrJsonFileName);
	spProjectileEffect->SetAttackColliderType(COLLIDER_TYPE::OBB);
	spProjectileEffect->SetColliderExtents(_vExtents);
	spProjectileEffect->SetColliderCenter(_vCenter);
	spProjectileEffect->SetColliderOn(false);

	if (FAILED(spProjectileEffect->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spProjectileEffect->GetActorName());
	}

	return S_OK;
}
HRESULT CBoss_Andreus::ReadySpriteEffects(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio)
{
	shared_ptr<CSpriteActor> spSpriteActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->CreateActorToPool<CSpriteActor>(_wstrJsonFileName + to_wstring(_iEffectsNum));
	if (nullptr == spSpriteActor)
	{
		MSG_BOX("Failed to Create HitEffects : CBoss_Andreus");
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
