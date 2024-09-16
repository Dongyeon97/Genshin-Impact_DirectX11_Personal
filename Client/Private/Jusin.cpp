#include "pch.h"
#include "Jusin.h"

#include "CameraManager.h"

#include "Level.h"
#include "Layer.h"
#include "SpriteActor.h"

#include "AttackCollider.h"
#include "CameraComponent.h"
#include "ProductionCamera.h"

void CJusin::Initialize()
{
	CPlayerProjectileEffect::Initialize();
}

HRESULT CJusin::Initialize(const wstring& _wstrJsonFileName)
{
	return CPlayerProjectileEffect::Initialize(_wstrJsonFileName);
}

void CJusin::RecycleBegin()
{
	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetIsCollideOn(false);

	m_bExplosionReserve = false;

	CPlayerProjectileEffect::RecycleBegin();
}

_int CJusin::PreUpdate(const _float& _fTimeDelta)
{
	if (m_bExplosionReserve)
		m_bUse = false;

	return CPlayerProjectileEffect::PreUpdate(_fTimeDelta);
}

_int CJusin::Update(const _float& _fTimeDelta)
{
	CPlayerProjectileEffect::Update(_fTimeDelta);

	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetIsCollideOn(false);

	if (m_fLifeTimeAcc > 1.75f)
		m_wpAttackCollider.lock()->SetIsCollideOn(true);

	return 0;
}

_int CJusin::LateUpdate(const _float& _fTimeDelta)
{
	if (m_vTranslate.y < m_vExplosionPos.y)
		Explosion();

	return CPlayerProjectileEffect::LateUpdate(_fTimeDelta);
}

void CJusin::Render()
{
	CPlayerProjectileEffect::Render();
}

void CJusin::EndPlay()
{
	CPlayerProjectileEffect::EndPlay();
}

void CJusin::Release()
{
	CPlayerProjectileEffect::Release();
}

void CJusin::Explosion()
{
	// m_bUse = false;
	m_bExplosionReserve = true;
	m_bUse = true;

	if (!m_wpAttackCollider.expired())
		m_wpAttackCollider.lock()->SetIsCollideOn(true);

	// 파티클 생성
	MakeParticle(L"SprtieParticle_Zhongli_Jusin_One", XMVectorSet(0.f, 0.f, 0.f, 0.f));

	// 이펙트 생성
	MakeEffect(L"Zhongli_Skill_Q_Floor", XMVectorSet(0.f, 3.f, 0.f, 0.f));
	MakeEffect(L"Zhongli_Skill_Q_ShockWave", XMVectorSet(0.f, 3.f, 0.f, 0.f));
	MakeEffect(L"Zhongli_Skill_Q_ShockWave_Two", XMVectorSet(0.f, 3.f, 0.f, 0.f));

	SetActorTranslation_NoNavigation(XMVectorSet(100000.f, 100000.f, 100000.f, 1.f));
	CalculateWorldMatrix_Immediately();
	m_wpAttackCollider.lock()->UpdateSceneComponentTransform();
	m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
	m_wpAttackCollider.lock()->LateUpdate(0.f); // 개 임시

	// 카메라 셰이킹 요청
	shared_ptr<CCameraComponent> spCam = CCameraManager::GetInstance()->GetCurCamera();
	if (nullptr == spCam)
		return;

	shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(spCam);

	spProductionCam->BeginShaking(0.f, 0.7f, 2.f);
}

void CJusin::MakeParticle(const wstring& _wstrParticleName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(_wstrParticleName);
	if (nullptr != spActor)
		spActor->SetActorTranslation_NoNavigation(XMLoadFloat4(&m_vTranslate) + _vOffset);
}

void CJusin::MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);
	if (nullptr == spActor)
		return;

	_float3 vRotation = dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vRotation;

	spActor->SetRotation(XMLoadFloat3(&vRotation));
	spActor->SetActorTranslation_NoNavigation(XMLoadFloat4(&m_vTranslate) + _vOffset);
}

_int CJusin::OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	return 0;
}