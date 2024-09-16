#include "Engine_pch.h"
#include "MonsterAttackEffect.h"

#include "RenderManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CMonsterAttackEffect::Initialize()
{
	m_wpAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpAttackCollider.lock()->SetColliderType(m_eColliderType);
	m_wpAttackCollider.lock()->SetColliderCenter(m_vCenter);
	m_wpAttackCollider.lock()->SetColliderRadius(m_fRadius);
	m_wpAttackCollider.lock()->SetColliderExtents(m_vExtents);

	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_wpAttackCollider);

	return CEffectMeshActor::Initialize();
}

HRESULT CMonsterAttackEffect::Initialize(const wstring& _wstrJsonFileName)
{
	m_wpAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpAttackCollider.lock()->SetColliderType(m_eColliderType);
	m_wpAttackCollider.lock()->SetColliderCenter(m_vCenter);
	m_wpAttackCollider.lock()->SetColliderRadius(m_fRadius);
	m_wpAttackCollider.lock()->SetColliderExtents(m_vExtents);

	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_wpAttackCollider);

	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMonsterAttackEffect::RecycleBegin()
{
	if (!m_wpAttackCollider.expired())
		CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_wpAttackCollider);

	CEffectMeshActor::RecycleBegin();
}

_int CMonsterAttackEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMonsterAttackEffect::Update(const _float& _fTimeDelta)
{
	if ((m_fLifeTimeAcc + _fTimeDelta) >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		if(!m_wpAttackCollider.expired())
			m_wpAttackCollider.lock()->SetIsCollideOn(false);

		CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_wpAttackCollider);
	}
	else if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.x)
	{
		m_wpAttackCollider.lock()->SetIsCollideOn(true);
	}

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMonsterAttackEffect::LateUpdate(const _float& _fTimeDelta)
{
	m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_wpAttackCollider.lock());
#endif

	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMonsterAttackEffect::Render()
{
	CEffectMeshActor::Render();
}

void CMonsterAttackEffect::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMonsterAttackEffect::SetAttackDesc(ATTACK_DESC _AttackDesc) 
{
	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetAttackDesc(_AttackDesc);
}

ATTACK_DESC CMonsterAttackEffect::GetAttackDesc() 
{
	return m_wpAttackCollider.lock()->GetAttackDesc();
}

void CMonsterAttackEffect::SetColliderOn(_bool _bColliderOn) 
{
	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetIsCollideOn(_bColliderOn);
}

_bool CMonsterAttackEffect::GetColliderOn() 
{
	return m_wpAttackCollider.lock()->GetISCollideOn();
}

void CMonsterAttackEffect::Release()
{
	if (!m_wpAttackCollider.expired())
	{
		m_wpAttackCollider.lock()->Release();
		m_wpAttackCollider.reset();
	}

	CEffectMeshActor::Release();
}
