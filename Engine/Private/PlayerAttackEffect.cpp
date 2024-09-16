#include "Engine_pch.h"
#include "PlayerAttackEffect.h"

#include "RenderManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CPlayerAttackEffect::Initialize()
{
	m_wpAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpAttackCollider.lock()->SetColliderType(m_eColliderType);
	m_wpAttackCollider.lock()->SetColliderCenter(m_vCenter);
	m_wpAttackCollider.lock()->SetColliderRadius(m_fRadius);
	m_wpAttackCollider.lock()->SetColliderExtents(m_vExtents);

	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpAttackCollider);

	return CEffectMeshActor::Initialize();
}

HRESULT CPlayerAttackEffect::Initialize(const wstring& _wstrJsonFileName)
{
	m_wpAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpAttackCollider.lock()->SetColliderType(m_eColliderType);
	m_wpAttackCollider.lock()->SetColliderCenter(m_vCenter);
	m_wpAttackCollider.lock()->SetColliderRadius(m_fRadius);
	m_wpAttackCollider.lock()->SetColliderExtents(m_vExtents);

	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpAttackCollider);

	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CPlayerAttackEffect::RecycleBegin()
{
	if (!m_wpAttackCollider.expired())
	{
		// CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_wpAttackCollider);
		m_wpAttackCollider.lock()->SetIsCollideOn(true);
	}

	CEffectMeshActor::RecycleBegin();
}

_int CPlayerAttackEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CPlayerAttackEffect::Update(const _float& _fTimeDelta)
{
	if ((m_fLifeTimeAcc + _fTimeDelta) >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		if (!m_wpAttackCollider.expired())
			m_wpAttackCollider.lock()->SetIsCollideOn(false);

		// CCollisionManager::GetInstance()->RemovePlayerAttackCollider(m_wpAttackCollider);
	}
	else if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.x)
	{
		m_wpAttackCollider.lock()->SetIsCollideOn(true);
	}

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CPlayerAttackEffect::LateUpdate(const _float& _fTimeDelta)
{
	m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_wpAttackCollider.lock());
#endif

	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CPlayerAttackEffect::Render()
{
	CEffectMeshActor::Render();
}

void CPlayerAttackEffect::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CPlayerAttackEffect::SetAttackDesc(ATTACK_DESC _AttackDesc)
{
	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetAttackDesc(_AttackDesc);
}

ATTACK_DESC CPlayerAttackEffect::GetAttackDesc()
{
	return m_wpAttackCollider.lock()->GetAttackDesc();
}

void CPlayerAttackEffect::SetColliderOn(_bool _bColliderOn)
{
	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetIsCollideOn(_bColliderOn);
}

_bool CPlayerAttackEffect::GetColliderOn()
{
	return m_wpAttackCollider.lock()->GetISCollideOn();
}

void CPlayerAttackEffect::Release()
{
	if (!m_wpAttackCollider.expired())
	{
		m_wpAttackCollider.lock()->Release();
		m_wpAttackCollider.reset();
	}

	CEffectMeshActor::Release();
}
