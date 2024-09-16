#include "Engine_pch.h"
#include "MeshActor.h"

#include "PrefabManager.h"

void CMeshActor::Initialize()
{
	CActor::Initialize();
}

_int CMeshActor::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CMeshActor::Update(const _float& _fTimeDelta)
{
	return CActor::Update(_fTimeDelta);
}

_int CMeshActor::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CMeshActor::Render()
{
	CActor::Render();
}

void CMeshActor::Render_Shadow()
{

}

void CMeshActor::EndPlay()
{
	CActor::EndPlay();
}

void CMeshActor::Release()
{
	CActor::Release();
}