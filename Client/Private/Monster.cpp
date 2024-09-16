#include "pch.h"
#include "Monster.h"

#include "RenderManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"

#include "Level.h"
#include "Layer.h"
#include "Player.h"
#include "Seal.h"

#include "ColliderComponent.h"

_uint CMonster::g_iNumMonster = 0;

void CMonster::Initialize()
{
	CCharactor::Initialize();
}

_int CMonster::PreUpdate(const _float& _fTimeDelta)
{
	return CCharactor::PreUpdate(_fTimeDelta);
}

_int CMonster::Update(const _float& _fTimeDelta)
{
	if (m_bNahida_Marked)
	{
		m_fMarkingTimeAcc += _fTimeDelta;
		if (m_fMarkingMinTime < m_fMarkingTimeAcc)
		{
			m_bNahida_Marked = false;
			m_fMarkingTimeAcc = 0.f;
		}
	}

	return CCharactor::Update(_fTimeDelta);
}

_int CMonster::LateUpdate(const _float& _fTimeDelta)
{
	// ������ Update�� ���� ��, LateUpdate ������ �浹ü �����Ʈ���� ������Ʈ
	m_wpCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
	m_wpCollider.lock()->LateUpdate(_fTimeDelta);

	if (m_wpStateMachine.lock()->GetCurState() & MONSTER_STATE::IS_DIE)
	{
		m_bUse = false;
	}

//#ifdef _DEBUG
//	CRenderManager::GetInstance()->AddDebugComponent(m_wpCollider.lock());
//#endif

	return CCharactor::LateUpdate(_fTimeDelta);
}

void CMonster::Render()
{
	CCharactor::Render();
}

void CMonster::EndPlay()
{
	CCharactor::EndPlay();
}

void CMonster::Release()
{
	CCollisionManager::GetInstance()->RemoveMonsterBodyCollider(m_wpCollider.lock());

	CCharactor::Release();
}

void CMonster::RemoveMonsterCollider()
{
	CCollisionManager::GetInstance()->RemoveMonsterBodyCollider(m_wpCollider);
}

HRESULT CMonster::MakeSeal(const wstring& _wstrEffectName)
{
	shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetStaticLevel()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectName);

	if (nullptr == spActor)
		return E_FAIL;

	spActor = dynamic_pointer_cast<CSeal>(spActor);
	spActor->SetOwnerActor(dynamic_pointer_cast<CActor>(shared_from_this()));

	return S_OK;
}

_int CMonster::OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	if (!m_bNahida_Marked && ELEMENTAL_TYPE::DENDRO == AttackDesc.eElementalType)
	{
		MakeSeal(L"Nahida_Skill_E_Seal");
		m_bNahida_Marked = true;
	}

	switch (AttackDesc.eKnockbackType)
	{
		// 1. �� ����(����)
	case KNOCKBACK_TYPE::WEAK:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT1, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
		// 2. �� ����(��ħ)
	case KNOCKBACK_TYPE::STRONG:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT2, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
		// 3. �ٿ� ����
	case KNOCKBACK_TYPE::POWERFUL:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT3, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	}
	return 0;
}

void CMonster::OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	switch (AttackDesc.eKnockbackType)
	{
		// 1. �� ����(����)
	case KNOCKBACK_TYPE::WEAK:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT1, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	// 2. �� ����(��ħ)
	case KNOCKBACK_TYPE::STRONG:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT2, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	// 3. �ٿ� ����
	case KNOCKBACK_TYPE::POWERFUL:
	{
		m_wpStateMachine.lock()->RequestChangeState(MONSTER_STATE::HIT3, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	}
}

void CMonster::OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}