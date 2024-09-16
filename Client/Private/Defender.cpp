#include "pch.h"
#include "Defender.h"

#include "RenderManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AnimModel.h"
#include "Animation.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "DefenderStateMachine.h"

#include "MonsterHPbar.h"
#include "MonsterHPbar_Frame.h"

void CDefender::Initialize()
{
	// m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Cs_Monster_Konungmathr_None (merge)");
	// m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Defender");

	m_wpStateMachine = CreateActorComponent<CDefenderStateMachine>(L"DefenderStateMachine");

	// 몬스터의 기본 콜라이더 장착
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 1.0f, 0.0f });
	m_wpCollider.lock()->SetColliderRadius(1.5f);
	CCollisionManager::GetInstance()->AddMonsterBodyCollider(m_wpCollider);

	// AttackCollider
	m_spLeftArm_AttackCollider = make_shared<CAttackCollider>();
	m_spLeftArm_AttackCollider->SetComponentName(L"LeftArm_AttackCollider");
	m_spLeftArm_AttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spLeftArm_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spLeftArm_AttackCollider->SetColliderRadius(1.5f);
	m_spLeftArm_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spLeftArm_AttackCollider->Initialize();

	m_spRightArm_AttackCollider = make_shared<CAttackCollider>();
	m_spRightArm_AttackCollider->SetComponentName(L"RightArm_AttackCollider");
	m_spRightArm_AttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spRightArm_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spRightArm_AttackCollider->SetColliderRadius(1.5f);
	m_spRightArm_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spRightArm_AttackCollider->Initialize();

	m_spLeg_AttackCollider = make_shared<CAttackCollider>();
	m_spLeg_AttackCollider->SetComponentName(L"Leg_AttackCollider");
	m_spLeg_AttackCollider->SetColliderType(COLLIDER_TYPE::OBB);
	m_spLeg_AttackCollider->SetColliderCenter({ 0.0f, 0.0f, 1.0f });
	m_spLeg_AttackCollider->SetColliderExtents({ 2.0f, 1.0f, 2.0f });
	m_spLeg_AttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spLeg_AttackCollider->Initialize();

	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spLeftArm_AttackCollider);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spRightArm_AttackCollider);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spLeg_AttackCollider);

#pragma region 몬스터 정보
	m_tInfo.m_eMonsterType		= MONSTER_TYPE::MIDDLE_BOSS;
	m_tInfo.m_eHitType			= HIT_TYPE::SUPERARMOR;
	m_tInfo.m_eElementalType	= ELEMENTAL_TYPE::NONE_ELEMENTAL;

	ReadyHPbar(500.f);

	m_fMaxHp = 100000.0f;
	m_fCurHp = 100000.0f;
	m_fAtk = 2000.0f;
	m_fDef = 500.0f;
	m_fMoveSpeed = 300.0f;

	m_tInfo.m_fDetectingRadius = 800.0f;
	// =============Attack============== //
	m_tInfo.m_fAttack1Radius = 400.0f;
	m_tInfo.m_fAttack2Radius = 700.0f;
	m_tInfo.m_fAttack3Radius = 700.0f;
	m_tInfo.m_fAttack4Radius = 1000.0f;

	m_tInfo.m_fAttack1RunTime = 4.0f;	// Attack 진행 시간(내가 임의로 정할거임, 기본 애니메이션들이 너무 다 길게 잡혀있음.)
	m_tInfo.m_fAttack2RunTime = 5.5f;
	m_tInfo.m_fAttack3RunTime = 4.0f;
	m_tInfo.m_fAttack4RunTime = 0.0f;

	m_tInfo.m_fAttack1CoolTime = 0.0f;	// Attack을 사용하고 나서 델타타임 만큼 누적되는 시간
	m_tInfo.m_fAttack2CoolTime = 0.0f;
	m_tInfo.m_fAttack3CoolTime = 0.0f;
	m_tInfo.m_fAttack4CoolTime = 0.0f;

	m_tInfo.m_fAttack1MinTime = 5.0f;	// Attack을 사용하기 까지의 최소 시간
	m_tInfo.m_fAttack2MinTime = 15.0f;
	m_tInfo.m_fAttack3MinTime = 20.0f;
	m_tInfo.m_fAttack4MinTime = 25.0f;

	// =============Pattern============== //
	m_tInfo.m_fPattern1Radius = 500.0f;
	m_tInfo.m_fPattern2Radius = 500.0f;
	m_tInfo.m_fPattern3Radius = 500.0f;
	m_tInfo.m_fPattern4Radius = 500.0f;	

	m_tInfo.m_fPattern1RunTime = 0.0f;	// Pattern 진행 시간
	m_tInfo.m_fPattern2RunTime = 0.0f;
	m_tInfo.m_fPattern3RunTime = 0.0f;
	m_tInfo.m_fPattern4RunTime = 0.0f;

	m_tInfo.m_fPattern1CoolTime = 5.0f;	// Pattern을 사용하고 나서 델타타임 만큼 누적되는 시간
	m_tInfo.m_fPattern2CoolTime = 6.0f;	
	m_tInfo.m_fPattern3CoolTime = 7.0f;	
	m_tInfo.m_fPattern4CoolTime = 8.0f;	

	m_tInfo.m_fPattern1MinTime = 5.0f;;	// Pattern을 사용하기 까지의 최소 시간
	m_tInfo.m_fPattern2MinTime = 15.0f;	
	m_tInfo.m_fPattern3MinTime = 20.0f;	
	m_tInfo.m_fPattern4MinTime = 25.0f;	

	m_tInfo.m_fHit1MinTime = 1.0f;				// HIT1상태 지속시간
	m_tInfo.m_fHit1RunTime = 0.0f;				// HIT1상태 진행시간
				    
	m_tInfo.m_fHit2MinTime = 1.0f;				// HIT2상태 지속시간
	m_tInfo.m_fHit2RunTime = 0.0f;				// HIT2상태 지속시간
				    
	m_tInfo.m_fHit3MinTime = 1.0f;				// HIT3상태 진행시간
	m_tInfo.m_fHit3RunTime = 0.0f;				// HIT3상태 진행시간

	dynamic_pointer_cast<CMonsterStateMachine>(m_wpStateMachine.lock())->SetMonsterInfo(m_tInfo);
#pragma endregion 몬스터 정보

	CNormalMonster::Initialize();

	m_wpAnimModel.lock()->GetAnimVector()[DEFENDER_ANIM::WALK_FORWARD]->SetIsLoop(true);
	m_wpAnimModel.lock()->SetAnimationIndex(DEFENDER_ANIM::IDLE);

	for (_uint i = 0; i < m_wpAnimModel.lock()->GetNumAnimations(); ++i)
	{
		m_wpAnimModel.lock()->SetIsRootMotion(false, i);
		m_wpAnimModel.lock()->SetRootMotionRatio({ 0.0f, 2.1f, 0.0f }, i);
	}

	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());
	dynamic_pointer_cast<CDefenderStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"LeftArm", m_spLeftArm_AttackCollider);
	dynamic_pointer_cast<CDefenderStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"RightArm", m_spRightArm_AttackCollider);
	dynamic_pointer_cast<CDefenderStateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"Leg", m_spLeg_AttackCollider);

	ReadyBoneMatrixToArm();
}

_int CDefender::PreUpdate(const _float& _fTimeDelta)
{
	return CNormalMonster::PreUpdate(_fTimeDelta);
}

_int CDefender::Update(const _float& _fTimeDelta)
{
	UpdateArmColliderTransform();

	return CNormalMonster::Update(_fTimeDelta);
}

_int CDefender::LateUpdate(const _float& _fTimeDelta)
{
	m_spLeftArm_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matLeftArm));
	m_spLeftArm_AttackCollider->LateUpdate(_fTimeDelta);

	m_spRightArm_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matRightArm));
	m_spRightArm_AttackCollider->LateUpdate(_fTimeDelta);

	m_spLeg_AttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
	m_spLeg_AttackCollider->LateUpdate(_fTimeDelta);

	if (m_wpStateMachine.lock()->GetCurState() & MONSTER_STATE::IS_DIE)
	{
		CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_spLeftArm_AttackCollider);
		CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_spRightArm_AttackCollider);
		CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_spLeg_AttackCollider);
	}

#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_spLeftArm_AttackCollider);
	CRenderManager::GetInstance()->AddDebugComponent(m_spRightArm_AttackCollider);
	CRenderManager::GetInstance()->AddDebugComponent(m_spLeg_AttackCollider);
#endif

	return CNormalMonster::LateUpdate(_fTimeDelta);
}

void CDefender::Render()
{
	CNormalMonster::Render();
}

void CDefender::EndPlay()
{
	CNormalMonster::EndPlay();
}

void CDefender::Release()
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

	if (nullptr != m_spLeg_AttackCollider)
	{
		m_spLeg_AttackCollider->Release();
		m_spLeg_AttackCollider.reset();
	}

	CNormalMonster::Release();
}

void CDefender::ReadyBoneMatrixToArm()
{
	m_pSocketMatrix_LeftArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("HitBox_LeftArm");
	m_pSocketMatrix_RightArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("HitBox_RightArm");
}

void CDefender::UpdateArmColliderTransform()
{
	// 붙어있는 뼈의 행렬을 가져온다.
	_matrix	LeftArm_BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix_LeftArm);
	_matrix	RightArm_BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix_RightArm);

	// 행렬을 정규화 한다.
	for (size_t i = 0; i < 3; i++)
	{
		LeftArm_BoneMatrix.r[i] = XMVector3Normalize(LeftArm_BoneMatrix.r[i]);
		RightArm_BoneMatrix.r[i] = XMVector3Normalize(RightArm_BoneMatrix.r[i]);
	}

	// 자기 -> 뼈(1차 부모) -> 액터(2차 부모) 순서대로 월드를 구한다.
	XMStoreFloat4x4(&m_matLeftArm, LeftArm_BoneMatrix * XMLoadFloat4x4(&m_matWorld));
	XMStoreFloat4x4(&m_matRightArm, RightArm_BoneMatrix * XMLoadFloat4x4(&m_matWorld));
}
