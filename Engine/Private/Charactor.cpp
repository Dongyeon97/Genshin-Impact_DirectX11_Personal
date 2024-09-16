#include "Engine_pch.h"
#include "Charactor.h"

void CCharactor::Initialize()
{
	CAnimActor::Initialize();
}

_int CCharactor::PreUpdate(const _float& _fTimeDelta)
{
	return CAnimActor::PreUpdate(_fTimeDelta);
}

_int CCharactor::Update(const _float& _fTimeDelta)
{
	return CAnimActor::Update(_fTimeDelta);
}

_int CCharactor::LateUpdate(const _float& _fTimeDelta)
{
	return CAnimActor::LateUpdate(_fTimeDelta);
}

void CCharactor::Render()
{
	CAnimActor::Render();
}

void CCharactor::EndPlay()
{
	CAnimActor::EndPlay();
}

void CCharactor::Release()
{
	CAnimActor::Release();
}