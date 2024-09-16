#include "Engine_pch.h"
#include "AttackCollider.h"

void CAttackCollider::Initialize()
{
	CCollider::Initialize();
}

_int CAttackCollider::PreUpdate(const _float& _fTimeDelta)
{
	return CCollider::PreUpdate(_fTimeDelta);
}

_int CAttackCollider::Update(const _float& _fTimeDelta)
{
	return CCollider::Update(_fTimeDelta);
}

_int CAttackCollider::LateUpdate(const _float& _fTimeDelta)
{
	return CCollider::LateUpdate(_fTimeDelta);
}

void CAttackCollider::Render()
{
	CCollider::Render();
}

void CAttackCollider::EndPlay()
{
	CCollider::EndPlay();
}

void CAttackCollider::Release()
{
	CCollider::Release();
}
