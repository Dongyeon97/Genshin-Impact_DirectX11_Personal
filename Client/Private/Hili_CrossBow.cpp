#include "pch.h"
#include "Hili_CrossBow.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"

#include "AttackCollider.h"

void CHili_CrossBow::Initialize()
{
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(L"MonEquip_Crossbow_Ice_01 (merge)");
	m_SpawnData = CSpawnManager::GetInstance()->FindSpawnData("CrossBow");

	XMStoreFloat4(&m_vScale, XMVectorSet(100.0f, 100.0f, 100.0f, 0.0f));

	_vector vTransformCopy = { 0.0f, 90.0f, 0.0f};
	XMStoreFloat4(&m_vRotation, vTransformCopy);

	// [ Component - Collider ]
	m_spAttackCollider = make_shared<CAttackCollider>();
	m_spAttackCollider->SetComponentName(L"WeaponAttackCollider");
	m_spAttackCollider->SetColliderType(COLLIDER_TYPE::SPHERE);
	m_spAttackCollider->SetColliderCenter({ 0.0f, 0.0f, 0.0f });
	m_spAttackCollider->SetColliderRadius(0.5f);
	// CCollisionManager::GetInstance()->AddPlayerAttackCollider(m_spAttackCollider);
	m_spAttackCollider->Initialize();

	CWeapon::Initialize();
}

_int CHili_CrossBow::PreUpdate(const _float& _fTimeDelta)
{
	return CWeapon::PreUpdate(_fTimeDelta);
}

_int CHili_CrossBow::Update(const _float& _fTimeDelta)
{
	return CWeapon::Update(_fTimeDelta);
}

_int CHili_CrossBow::LateUpdate(const _float& _fTimeDelta)
{
	return CWeapon::LateUpdate(_fTimeDelta);
}

void CHili_CrossBow::Render(_uint _iMeshIndex)
{
	CWeapon::Render(_iMeshIndex);
}

void CHili_CrossBow::EndPlay()
{
	CWeapon::EndPlay();
}

void CHili_CrossBow::Release()
{
	CWeapon::Release();
}