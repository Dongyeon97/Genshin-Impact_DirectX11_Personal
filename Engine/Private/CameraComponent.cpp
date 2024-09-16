#include "Engine_pch.h"
#include "CameraComponent.h"

#include "InputManager.h"

#include "Actor.h"

void CCameraComponent::Initialize()
{
	CSceneComponent::Initialize();
}

_int CCameraComponent::PreUpdate(const _float& _fTimeDelta)
{
	return CSceneComponent::PreUpdate(_fTimeDelta);
}

_int CCameraComponent::Update(const _float& _fTimeDelta)
{
	UpdateCameraTransform(_fTimeDelta);

	return CSceneComponent::Update(_fTimeDelta);
}

_int CCameraComponent::LateUpdate(const _float& _fTimeDelta)
{
	return CSceneComponent::LateUpdate(_fTimeDelta);
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::EndPlay()
{
	CSceneComponent::EndPlay();
}

void CCameraComponent::Release()
{

	CSceneComponent::Release();
}

void CCameraComponent::SetCameraProjInfo(_float _fFovY, _float _fNear, _float _fFar, _float _fAspect)
{
	m_fFovY = _fFovY;
	m_fNear = _fNear;
	m_fFar = _fFar;
	m_fAspect = _fAspect;
}

void CCameraComponent::UpdateCameraTransform(const _float& _fTimeDelta)
{
	if (!m_wpOwnerActor.expired())
	{
		_matrix matWorldInverse = XMMatrixInverse(0, XMLoadFloat4x4(&m_matWorld));
		XMStoreFloat4x4(&m_matView, matWorldInverse);
	}
}