#include "pch.h"
#include "Catalyst.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CCatalyst::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Equip_Catalyst_Morax");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Bow_Amos");

	XMStoreFloat4(&m_vScale, XMVectorSet(100.0f, 100.0f, 100.0f, 0.0f));

	SetTransformCopy(_float3(0.f, 90.f, 0.f));
	_vector vTransformCopy = XMLoadFloat3(&m_vTransformCopy);

	vTransformCopy = XMVectorSetX(vTransformCopy, XMVectorGetX(vTransformCopy));
	vTransformCopy = XMVectorSetZ(vTransformCopy, XMVectorGetZ(vTransformCopy));
	XMStoreFloat4(&m_vRotation, vTransformCopy);

	//// [ Component - Collider ]
	//m_spAttackCollider = make_shared<CAttackCollider>();
	//m_spAttackCollider->SetComponentName(L"WeaponAttackCollider");
	//m_spAttackCollider->SetColliderType(COLLIDER_TYPE::OBB);
	//m_spAttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	//m_spAttackCollider->SetColliderExtents({ 1.0f, 1.0f, 1.0f });
	//CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_spAttackCollider);
	//m_spAttackCollider->Initialize();

	CWeapon::Initialize();
}

_int CCatalyst::PreUpdate(const _float& _fTimeDelta)
{
	return CWeapon::PreUpdate(_fTimeDelta);
}

_int CCatalyst::Update(const _float& _fTimeDelta)
{
	return CWeapon::Update(_fTimeDelta);
}

_int CCatalyst::LateUpdate(const _float& _fTimeDelta)
{
	return CWeapon::LateUpdate(_fTimeDelta);
}

void CCatalyst::Render(_uint _iMeshIndex)
{
	CWeapon::Render(_iMeshIndex);
}

void CCatalyst::EndPlay()
{
	CWeapon::EndPlay();
}

void CCatalyst::Release()
{
	CWeapon::Release();
}