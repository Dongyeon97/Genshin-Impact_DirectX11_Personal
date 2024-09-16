
#include "Engine_pch.h"
#include "PlayerProjectileEffect.h"

#include "CollisionManager.h"

#include "AttackCollider.h"

void CPlayerProjectileEffect::Initialize()
{
	CPlayerAttackEffect::Initialize();
}

HRESULT CPlayerProjectileEffect::Initialize(const wstring& _wstrJsonFileName)
{
	return CPlayerAttackEffect::Initialize(_wstrJsonFileName);
}

void CPlayerProjectileEffect::RecycleBegin()
{
	CPlayerAttackEffect::RecycleBegin();
}

_int CPlayerProjectileEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayerAttackEffect::PreUpdate(_fTimeDelta);
}

_int CPlayerProjectileEffect::Update(const _float& _fTimeDelta)
{
	if (m_fLifeTimeAcc + _fTimeDelta >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		return CPlayerAttackEffect::Update(_fTimeDelta);
	}
	else if (m_fLifeTimeAcc > m_EffectMeshDesc.vLifeTime.x)
	{
		ProgressToDirrection(_fTimeDelta);
	}

	return CPlayerAttackEffect::Update(_fTimeDelta);
}

_int CPlayerProjectileEffect::LateUpdate(const _float& _fTimeDelta)
{
	return CPlayerAttackEffect::LateUpdate(_fTimeDelta);
}

void CPlayerProjectileEffect::Render()
{
	CPlayerAttackEffect::Render();
}

void CPlayerProjectileEffect::EndPlay()
{
	CPlayerAttackEffect::EndPlay();
}

void CPlayerProjectileEffect::Release()
{
	CPlayerAttackEffect::Release();
}

void CPlayerProjectileEffect::ProgressToDirrection(const _float& _fTimeDelta)
{
	_vector vShootDir = XMVector3Normalize(XMLoadFloat3(&m_vShootDirrection));

	// 가속도는 현재 임시임, 넣을지 안넣을지 모름
	SetActorTranslation_NoNavigation(GetActorTranslation() + vShootDir * m_fShootSpeed * _fTimeDelta);
}

_int CPlayerProjectileEffect::OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	m_bUse = false;

	SetActorTranslation_NoNavigation(XMVectorSet(100000.f, 100000.f, 100000.f, 1.f));
	CalculateWorldMatrix_Immediately();
	m_wpAttackCollider.lock()->UpdateSceneComponentTransform();
	m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
	m_wpAttackCollider.lock()->LateUpdate(0.f); // 개 임시

	return 0;
}

void CPlayerProjectileEffect::OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}

void CPlayerProjectileEffect::OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}
