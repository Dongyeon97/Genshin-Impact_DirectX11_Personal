#include "pch.h"
#include "Songlue.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CSonglue::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Weapon_Songlue");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Songlue");

	XMStoreFloat4(&m_vScale, XMVectorSet(100.0f, 100.0f, 100.0f, 0.0f));
	
	SetTransformCopy(_float3(-60.8f, -35.9f, 43.6f));
	_vector vTransformCopy = XMLoadFloat3(&m_vTransformCopy);

	vTransformCopy = XMVectorSetX(vTransformCopy, XMVectorGetX(vTransformCopy) - 90.f);
	vTransformCopy = XMVectorSetZ(vTransformCopy, XMVectorGetZ(vTransformCopy) + 90.f);
	XMStoreFloat4(&m_vRotation, vTransformCopy);

	// [ Component - Collider ]
	m_spAttackCollider = make_shared<CAttackCollider>();
	m_spAttackCollider->SetComponentName(L"WeaponAttackCollider");
	m_spAttackCollider->SetColliderType(COLLIDER_TYPE::OBB);
	m_spAttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.5f });
	m_spAttackCollider->SetColliderExtents({ 0.4f, 0.4f, 1.0f });
	CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_spAttackCollider);
	m_spAttackCollider->SetOwnerActor(m_wpOwnerActor.lock());
	m_spAttackCollider->Initialize();

	CWeapon::Initialize();
}

_int CSonglue::PreUpdate(const _float& _fTimeDelta)
{
	return CWeapon::PreUpdate(_fTimeDelta);
}

_int CSonglue::Update(const _float& _fTimeDelta)
{
	return CWeapon::Update(_fTimeDelta);
}

_int CSonglue::LateUpdate(const _float& _fTimeDelta)
{
	return CWeapon::LateUpdate(_fTimeDelta);
}

void CSonglue::Render(_uint _iMeshIndex)
{
	CWeapon::Render(_iMeshIndex);
}

void CSonglue::EndPlay()
{
	CWeapon::EndPlay();
}

void CSonglue::Release()
{
	CWeapon::Release();
}