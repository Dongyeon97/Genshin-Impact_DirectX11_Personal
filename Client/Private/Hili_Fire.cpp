#include "pch.h"
#include "Hili_Fire.h"

#include "RenderManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"
#include "NavigationManager.h"

#include "AnimModel.h"
#include "Animation.h"
#include "ColliderComponent.h"
#include "Hili_Club.h"
#include "Hili_Fire_StateMachine.h"
#include "Navigation.h"

void CHili_Fire::Initialize()
{
	// 프리팹 매니저에서 모델 프리팹 검색, 있으면 데이터를 받아옴
	// m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Monster_Hili_Fire_Proto1");
	// m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Hili_Fire");

	m_wpStateMachine = CreateActorComponent<CHili_Fire_StateMachine>(L"Hili_Fire_StateMachine");

	// 몬스터의 기본 콜라이더 장착
	m_wpCollider = CreateActorComponent<CCollider>(L"ColliderComponent");
	m_wpCollider.lock()->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_wpCollider.lock()->SetColliderCenter({ 0.0f, 0.0f, 0.5f });
	m_wpCollider.lock()->SetColliderRadius(100.0f);
	CCollisionManager::GetInstance()->AddMonsterBodyCollider(m_wpCollider);

	m_wpClub = CreateActorComponent<CHili_Club>(L"Weapon");
	m_wpClub.lock()->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

#pragma region 몬스터 정보
	m_tInfo.m_eMonsterType = MONSTER_TYPE::COMMON;
	m_tInfo.m_eHitType = HIT_TYPE::KNOCKBACKED;
	m_tInfo.m_eElementalType = ELEMENTAL_TYPE::PYRO;

	ReadyHPbar(150.f);

	m_fMaxHp = 60000.0f;
	m_fCurHp = 60000.0f;
	m_fAtk = 1000.0f;
	m_fDef = 150.0f;
	m_fMoveSpeed = 150.0f;

	m_tInfo.m_fDetectingRadius = 750.0f;
	// =============Attack============== //
	m_tInfo.m_fAttack1Radius = 400.0f;
	m_tInfo.m_fAttack2Radius = 1000.0f;
	m_tInfo.m_fAttack3Radius = 0.0f;
	m_tInfo.m_fAttack4Radius = 0.0f;

	m_tInfo.m_fAttack1RunTime = 2.0f;	// Attack 진행 시간(내가 임의로 정할거임, 기본 애니메이션들이 너무 다 길게 잡혀있음.)
	m_tInfo.m_fAttack2RunTime = 9.0f;
	m_tInfo.m_fAttack3RunTime = 0.0f;
	m_tInfo.m_fAttack4RunTime = 0.0f;

	m_tInfo.m_fAttack1CoolTime = 0.0f;	// Attack을 사용하고 나서 델타타임 만큼 누적되는 시간
	m_tInfo.m_fAttack2CoolTime = 0.0f;
	m_tInfo.m_fAttack3CoolTime = 0.0f;
	m_tInfo.m_fAttack4CoolTime = 0.0f;

	m_tInfo.m_fAttack1MinTime = 5.0f;	// Attack을 사용하기 까지의 최소 시간
	m_tInfo.m_fAttack2MinTime = 20.0f;
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

	// [ Component - Parts ]
	ReadyBoneMatrixToWeapon();

	m_wpAnimModel.lock()->GetAnimVector()[HILI_FIRE_ANIM::WALK_FORWARD]->SetIsLoop(true);
	m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::IDLE);

	m_wpStateMachine.lock()->SetAnimModel(m_wpAnimModel.lock());
	dynamic_pointer_cast<CHili_Fire_StateMachine>(m_wpStateMachine.lock())->AddAttackCollider(L"AttackCollider", m_wpClub.lock()->GetWeaponCollider());
}

_int CHili_Fire::PreUpdate(const _float& _fTimeDelta)
{
	SetActorTranslation(GetActorTranslation(), &m_iCurNaviCellIndex);

	return CNormalMonster::PreUpdate(_fTimeDelta);
}

_int CHili_Fire::Update(const _float& _fTimeDelta)
{
	return CNormalMonster::Update(_fTimeDelta);
}

_int CHili_Fire::LateUpdate(const _float& _fTimeDelta)
{
	return CNormalMonster::LateUpdate(_fTimeDelta);
}

void CHili_Fire::Render()
{
	CNormalMonster::Render();

#ifdef _DEBUG
	m_wpClub.lock()->Render(0);
#endif
}

void CHili_Fire::EndPlay()
{
	CNormalMonster::EndPlay();
}

void CHili_Fire::Release()
{
	CCollisionManager::GetInstance()->RemoveMonsterBodyCollider(m_wpCollider);

	if (!m_wpCollider.expired())
	{
		m_wpCollider.reset();
	}

	CNormalMonster::Release();
}

void CHili_Fire::SetActorTranslation(_fvector _vPosition, _int* _iCurNaviCellIndex)
{
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	_float fPlaneY = 0.0f;

	if (nullptr == spNavi)
	{
		XMStoreFloat4(&m_vTranslate, _vPosition);
		return;
	}
	else if (true == spNavi->IsMove(_vPosition, _iCurNaviCellIndex, &fPlaneY))
	{
		_float4 vFinalPosition;
		XMStoreFloat4(&vFinalPosition, _vPosition);

		vFinalPosition.y = fPlaneY + 60.f;

		XMStoreFloat4(&m_vTranslate, XMLoadFloat4(&vFinalPosition));
	}
}

void CHili_Fire::ReadyBoneMatrixToWeapon()
{
	m_pSocketMatrix_Arm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("Bip001_R_Hand");

	m_wpClub.lock()->SetSocketMatrix(m_pSocketMatrix_Arm);
}
