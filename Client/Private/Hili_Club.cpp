#include "pch.h"
#include "Hili_Club.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CHili_Club::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"MonEquip_WoodenClub_01");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("Torch");

	XMStoreFloat4(&m_vScale, XMVectorSet(100.0f, 100.0f, 100.0f, 0.0f));

	_vector vTransformCopy = { 0.0f, 90.0f, 0.0f };
	XMStoreFloat4(&m_vRotation, vTransformCopy);

	// [ Component - Collider ]
	m_spAttackCollider = make_shared<CAttackCollider>();
	m_spAttackCollider->SetComponentName(L"WeaponAttackCollider");
	m_spAttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spAttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spAttackCollider->SetColliderRadius(0.75f);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_spAttackCollider);
	m_spAttackCollider->SetOwnerActor(GetOwnerActor().lock());
	m_spAttackCollider->Initialize();

	CWeapon::Initialize();
}

_int CHili_Club::PreUpdate(const _float& _fTimeDelta)
{
	return CWeapon::PreUpdate(_fTimeDelta);
}

_int CHili_Club::Update(const _float& _fTimeDelta)
{
	return CWeapon::Update(_fTimeDelta);
}

_int CHili_Club::LateUpdate(const _float& _fTimeDelta)
{
	return CWeapon::LateUpdate(_fTimeDelta);
}

void CHili_Club::Render(_uint _iMeshIndex)
{
	CWeapon::Render(_iMeshIndex);
}

void CHili_Club::EndPlay()
{
	CWeapon::EndPlay();
}

void CHili_Club::Release()
{
	CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_spAttackCollider);

	CWeapon::Release();
}