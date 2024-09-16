#include "Engine_pch.h"
#include "Projectile.h"

void CProjectile::Initialize()
{
	CNonAnimModel::Initialize();
}

_int CProjectile::PreUpdate(const _float& _fTimeDelta)
{
	return CNonAnimModel::PreUpdate(_fTimeDelta);
}

_int CProjectile::Update(const _float& _fTimeDelta)
{
	return CNonAnimModel::Update(_fTimeDelta);
}

_int CProjectile::LateUpdate(const _float& _fTimeDelta)
{
	return CNonAnimModel::LateUpdate(_fTimeDelta);
}

void CProjectile::Render(_uint _iMeshIndex)
{
	CNonAnimModel::Render(_iMeshIndex);
}

void CProjectile::EndPlay()
{
	CNonAnimModel::EndPlay();
}

void CProjectile::Release()
{
	CNonAnimModel::Release();
}