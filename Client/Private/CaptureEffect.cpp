#include "pch.h"
#include "CaptureEffect.h"

void CCaptureEffect::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CCaptureEffect::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CCaptureEffect::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CCaptureEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CCaptureEffect::Update(const _float& _fTimeDelta)
{
	if (!m_wpOwnerActor.expired())
	{
		_vector vTotalRotation = XMLoadFloat4(&m_vOriginRotation) + m_wpOwnerActor.lock()->GetRotation();

		XMStoreFloat4(&m_vRotation, vTotalRotation);
		XMStoreFloat4x4(&m_matAdditionalRotation, m_wpOwnerActor.lock()->GetAdditionalRotationMatrix());
		XMStoreFloat4x4(&m_matMouseRotate, m_wpOwnerActor.lock()->GetMouseRotateMatrix());
		// XMStoreFloat4(&m_vTranslate, XMLoadFloat4(&m_vOffset));
	}

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CCaptureEffect::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CCaptureEffect::Render()
{
	CEffectMeshActor::Render();
}

void CCaptureEffect::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CCaptureEffect::Release()
{
	CEffectMeshActor::Release();
}
