#include "pch.h"
#include "ArrowRain_Floor.h"

#include "AttackCollider.h"

void CArrowRain_Floor::Initialize()
{
	CPlayerAttackEffect::Initialize();
}

HRESULT CArrowRain_Floor::Initialize(const wstring& _wstrJsonFileName)
{
	return CPlayerAttackEffect::Initialize(_wstrJsonFileName);
}

void CArrowRain_Floor::RecycleBegin()
{
	m_fAttackTimeAcc = 0.f;

	CPlayerAttackEffect::RecycleBegin();
}

_int CArrowRain_Floor::PreUpdate(const _float& _fTimeDelta)
{
	return CPlayerAttackEffect::PreUpdate(_fTimeDelta);
}

_int CArrowRain_Floor::Update(const _float& _fTimeDelta)
{
	m_fAttackTimeAcc += _fTimeDelta;
	if (0.4f < m_fAttackTimeAcc)
	{
		ATTACK_DESC AttackDesc;

		AttackDesc = { L"Player_Ambor", L"Player_Ambor Skill2" + to_wstring(rand()), PYRO, STRONG,	false,	4850.f };

		m_wpAttackCollider.lock()->SetAttackDesc(AttackDesc);

		m_fAttackTimeAcc = 0.f;
	}

	if((m_fLifeTimeAcc + _fTimeDelta) >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		SetActorTranslation_NoNavigation(XMVectorSet(100000.f, 100000.f, 100000.f, 1.f));
		CalculateWorldMatrix_Immediately();
		m_wpAttackCollider.lock()->UpdateSceneComponentTransform();
		m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
		m_wpAttackCollider.lock()->LateUpdate(0.f); // ∞≥ ¿”Ω√
	}

	return CPlayerAttackEffect::Update(_fTimeDelta);
}

_int CArrowRain_Floor::LateUpdate(const _float& _fTimeDelta)
{
	return CPlayerAttackEffect::LateUpdate(_fTimeDelta);
}

void CArrowRain_Floor::Render()
{
	CPlayerAttackEffect::Render();
}

void CArrowRain_Floor::EndPlay()
{
	CPlayerAttackEffect::EndPlay();
}

void CArrowRain_Floor::Release()
{
	CPlayerAttackEffect::Release();
}
