#include "Engine_pch.h"
#include "MonsterProjectileEffect.h"

void CMonsterProjectileEffect::Initialize()
{
	CMonsterAttackEffect::Initialize();
}

HRESULT CMonsterProjectileEffect::Initialize(const wstring& _wstrJsonFileName)
{
	return CMonsterAttackEffect::Initialize(_wstrJsonFileName);
}

void CMonsterProjectileEffect::RecycleBegin()
{
	CMonsterAttackEffect::RecycleBegin();
}

_int CMonsterProjectileEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CMonsterAttackEffect::PreUpdate(_fTimeDelta);
}

_int CMonsterProjectileEffect::Update(const _float& _fTimeDelta)
{
	if (m_fLifeTimeAcc + _fTimeDelta >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		return CMonsterAttackEffect::Update(_fTimeDelta);
	}
	else if (m_fLifeTimeAcc > m_EffectMeshDesc.vLifeTime.x)
	{
		ProgressToDirrection(_fTimeDelta);
	}

	return CMonsterAttackEffect::Update(_fTimeDelta);
}

_int CMonsterProjectileEffect::LateUpdate(const _float& _fTimeDelta)
{
	return CMonsterAttackEffect::LateUpdate(_fTimeDelta);
}

void CMonsterProjectileEffect::Render()
{
	CMonsterAttackEffect::Render();
}

void CMonsterProjectileEffect::EndPlay()
{
	CMonsterAttackEffect::EndPlay();
}

void CMonsterProjectileEffect::Release()
{
	CMonsterAttackEffect::Release();
}

void CMonsterProjectileEffect::ProgressToDirrection(const _float& _fTimeDelta)
{
	_vector vShootDir = XMVector3Normalize(XMLoadFloat3(&m_vShootDirrection));

	// 가속도는 현재 임시임, 넣을지 안넣을지 모름
	SetActorTranslation_NoNavigation(GetActorTranslation() + vShootDir * m_fShootSpeed * _fTimeDelta);
}