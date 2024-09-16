#include "pch.h"
#include "Amos.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CAmos::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"Equip_Bow_Amos");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Bow_Amos");

	XMStoreFloat4(&m_vScale, XMVectorSet(100.0f, 100.0f, 100.0f, 0.0f));

	SetTransformCopy(_float3(- 61.426f, - 143.628f, - 42.564f));
	_vector vTransformCopy = XMLoadFloat3(&m_vTransformCopy);

	vTransformCopy = XMVectorSetX(vTransformCopy, XMVectorGetX(vTransformCopy) - 90.f);
	vTransformCopy = XMVectorSetZ(vTransformCopy, XMVectorGetZ(vTransformCopy) + 90.f);
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

_int CAmos::PreUpdate(const _float& _fTimeDelta)
{
	return CWeapon::PreUpdate(_fTimeDelta);
}

_int CAmos::Update(const _float& _fTimeDelta)
{
	return CWeapon::Update(_fTimeDelta);
}

_int CAmos::LateUpdate(const _float& _fTimeDelta)
{
	return CWeapon::LateUpdate(_fTimeDelta);
}

void CAmos::Render(_uint _iMeshIndex)
{
	CWeapon::Render(_iMeshIndex);
}

void CAmos::EndPlay()
{
	CWeapon::EndPlay();
}

void CAmos::Release()
{
	CWeapon::Release();
}