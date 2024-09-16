#include "pch.h"
#include "Slime_Ice.h"

#include "RenderManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AnimModel.h"
#include "Animation.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"
#include "Slime_Ice_StateMachine.h"

void CSlime_Ice::Initialize()
{
	// 프리팹 매니저에서 모델 프리팹 검색, 있으면 데이터를 받아옴
	// m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Monster_Slime_Ice_02 (merge)");
	// m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Slime_Ice");

	m_wpStateMachine = CreateActorComponent<CSlime_Ice_StateMachine>(L"Slime_Ice_StateMachine");

	// 몬스터의 기본 콜라이더 장착
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 0.0f, 0.25f });
	m_wpCollider.lock()->SetColliderRadius(0.5f);
	CCollisionManager::GetInstance()->AddMonsterBodyCollider(m_wpCollider);

	m_spAttackCollider = make_shared<CAttackCollider>();
	m_spAttackCollider->SetComponentName(L"AttackCollider");
	m_spAttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spAttackCollider->SetColliderCenter({ 0.0f, 1.0f, 0.0f });
	m_spAttackCollider->SetColliderRadius(0.5f);
	m_spAttackCollider->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));
	m_spAttackCollider->Initialize();
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spAttackCollider);

	_matrix matAddi = XMMatrixRotationX(XMConvertToRadians(90.0f));
	matAddi = matAddi * XMMatrixRotationZ(XMConvertToRadians(-180.0f));

	SetAdditionalPivot(matAddi);

#pragma region 몬스터 정보
	m_tInfo.m_eMonsterType = MONSTER_TYPE::COMMON;
	m_tInfo.m_eHitType = HIT_TYPE::KNOCKBACKED;
	m_tInfo.m_eElementalType = ELEMENTAL_TYPE::CRYO;

	ReadyHPbar(100.f);

	m_fMaxHp = 50000.0f;
	m_fCurHp = 50000.0f;
	m_fAtk = 800.0f;
	m_fDef = 100.0f;
	m_fMoveSpeed = 150.0f;

	m_tInfo.m_fDetectingRadius = 750.0f;
	// =============Attack============== //
	m_tInfo.m_fAttack1Radius = 150.0f;
	m_tInfo.m_fAttack2Radius = 150.0f;
	m_tInfo.m_fAttack3Radius = 0.0f;
	m_tInfo.m_fAttack4Radius = 0.0f;

	m_tInfo.m_fAttack1RunTime = 1.5f;	// Attack 진행 시간(내가 임의로 정할거임, 기본 애니메이션들이 너무 다 길게 잡혀있음.)
	m_tInfo.m_fAttack2RunTime = 2.0f;
	m_tInfo.m_fAttack3RunTime = 0.0f;
	m_tInfo.m_fAttack4RunTime = 0.0f;

	m_tInfo.m_fAttack1CoolTime = 0.0f;	// Attack을 사용하고 나서 델타타임 만큼 누적되는 시간
	m_tInfo.m_fAttack2CoolTime = 0.0f;
	m_tInfo.m_fAttack3CoolTime = 0.0f;
	m_tInfo.m_fAttack4CoolTime = 0.0f;

	m_tInfo.m_fAttack1MinTime = 5.0f;	// Attack을 사용하기 까지의 최소 시간
	m_tInfo.m_fAttack2MinTime = 10.0f;
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

	m_tInfo.m_fHit3MinTime = 3.0f;				// HIT3상태 진행시간
	m_tInfo.m_fHit3RunTime = 0.0f;				// HIT3상태 진행시간

	dynamic_pointer_cast<CMonsterStateMachine>(m_wpStateMachine.lock())->SetMonsterInfo(m_tInfo);
#pragma endregion 몬스터 정보

	CNormalMonster::Initialize();

	m_wpAnimModel.lock()->GetAnimVector()[SLIME_ICE_ANIM::WALK_FORWARD]->SetIsLoop(true);
	m_wpAnimModel.lock()->SetAnimationIndex(SLIME_ICE_ANIM::IDLE);

	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());
	dynamic_pointer_cast<CSlime_Ice_StateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"AttackCollider", m_spAttackCollider);
}

_int CSlime_Ice::PreUpdate(const _float& _fTimeDelta)
{
	return CNormalMonster::PreUpdate(_fTimeDelta);
}

_int CSlime_Ice::Update(const _float& _fTimeDelta)
{
	return CNormalMonster::Update(_fTimeDelta);
}

_int CSlime_Ice::LateUpdate(const _float& _fTimeDelta)
{
	m_spAttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
	m_spAttackCollider->LateUpdate(_fTimeDelta);

#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_spAttackCollider);
#endif

	return CNormalMonster::LateUpdate(_fTimeDelta);
}

void CSlime_Ice::Render()
{
	CNormalMonster::Render();
}

void CSlime_Ice::EndPlay()
{
	CNormalMonster::EndPlay();
}

void CSlime_Ice::Release()
{
	CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_spAttackCollider);

	if (nullptr != m_spAttackCollider)
	{
		m_spAttackCollider->Release();
		m_spAttackCollider.reset();
	}

	CNormalMonster::Release();
}