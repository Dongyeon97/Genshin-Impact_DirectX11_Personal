#include "pch.h"
#include "Shield.h"

#include "ObjectManager.h"

#include "UserInstance.h"

void CShield::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CShield::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CShield::RecycleBegin()
{
	// 유저 인스턴스에 무적 요청
	dynamic_pointer_cast<CUserInstance>(m_wpOwnerActor.lock()->GetOwnerActor().lock())->RequestShield(m_EffectMeshDesc.vLifeTime.y);

	CEffectMeshActor::RecycleBegin();
}

void CShield::UpdateCurPlayerPos()
{
	shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurPlayer();

	XMStoreFloat4(&m_vTranslate, spActor->GetActorTranslation() + XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));
}

_int CShield::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CShield::Update(const _float& _fTimeDelta)
{
	UpdateCurPlayerPos();

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CShield::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CShield::Render()
{
	CEffectMeshActor::Render();
}

void CShield::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CShield::Release()
{
	CEffectMeshActor::Release();
}
