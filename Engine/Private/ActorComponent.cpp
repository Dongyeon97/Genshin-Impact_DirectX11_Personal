#include "Engine_pch.h"
#include "ActorComponent.h"

#include "Level.h"
#include "Layer.h"
#include "Actor.h"

void CActorComponent::Initialize()
{
}

_int CActorComponent::PreUpdate(const _float& _fTimeDelta)
{
	return S_OK;
}

_int CActorComponent::Update(const _float& _fTimeDelta)
{
	return S_OK;
}

_int CActorComponent::LateUpdate(const _float& _fTimeDelta)
{
	return S_OK;
}

void CActorComponent::Render()
{
}

void CActorComponent::EndPlay()
{
}

void CActorComponent::Release()
{
}
