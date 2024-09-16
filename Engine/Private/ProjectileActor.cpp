#include "Engine_pch.h"
#include "ProjectileActor.h"

#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "LightManager.h"

#include "Projectile.h"
#include "AttackCollider.h"

void CProjectileActor::Initialize()
{
	// 얘 생성될 때, 꼭 밖에서 SetModelFileName 해주자
	// 이 외에도 발사 관련된 정보도 꼭 밖에서 세팅해주자.
	m_ModelData = *CPrefabManager::GetInstance()->GetModelData(m_wstrModelFileName);
	m_SpawnData.tPrefab.eModelType = MODEL_TYPE::MODEL_NONANIM;
	// m_SpawnData.tPrefab.szActorName;
	m_SpawnData.tPrefab.eRenderGroup = RENDER_GROUP::RENDER_NONBLEND;
	m_SpawnData.tPrefab.eMonsterVariation = MONSTER_VARIATION::NONE_MONSTER;

	// [ Projectile ]
	m_wpProjectile = CreateActorComponent<CProjectile>(L"ProjectileComponent");
	m_wpProjectile.lock()->SetModelFileName(m_wstrModelFileName);
	m_wpProjectile.lock()->SetModelData(m_ModelData);
	m_wpProjectile.lock()->SetSpawnData(m_SpawnData);
	m_wpProjectile.lock()->SetShaderFileInfo(L"Shader_VtxMesh");

	// [ AttackCollider ]
	m_wpAttackCollider = CreateActorComponent<CAttackCollider>(L"AttackCollider");
	m_wpAttackCollider.lock()->SetColliderType(m_eColliderType);
	m_wpAttackCollider.lock()->SetColliderCenter(m_vCenter);
	m_wpAttackCollider.lock()->SetColliderRadius(m_fRadius);
	m_wpAttackCollider.lock()->SetColliderExtents(m_vExtents);
	m_wpAttackCollider.lock()->SetColliderRadians(m_vRadians);
	m_wpAttackCollider.lock()->SetAttackDesc(m_AttackDesc);
	CCollisionManager::GetInstance()->AddMonsterAttackCollider(m_wpAttackCollider);

	CMeshActor::Initialize();
}

_int CProjectileActor::PreUpdate(const _float& _fTimeDelta)
{
	return CMeshActor::PreUpdate(_fTimeDelta);
}

_int CProjectileActor::Update(const _float& _fTimeDelta)
{
	ProgressToDirrection(_fTimeDelta);

	return CMeshActor::Update(_fTimeDelta);
}

_int CProjectileActor::LateUpdate(const _float& _fTimeDelta)
{
	m_wpAttackCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

	if (CheckDelete())
	{
		CCollisionManager::GetInstance()->RemoveMonsterAttackCollider(m_wpAttackCollider);

		return -1; // 자기 자신 삭제 요청
	}

	return CMeshActor::LateUpdate(_fTimeDelta);
}

void CProjectileActor::Render()
{
	BindShaderResources();

	_uint iNumMeshes = m_wpProjectile.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_wpProjectile.lock()->BindMaterialShaderResource(i, TEXTURE_TYPE::DIFFUSE, "g_DiffuseTexture")))
			return;

		if (FAILED(m_wpProjectile.lock()->BeginShader(0)))
			return;

		m_wpProjectile.lock()->Render(i);
	}

	CMeshActor::Render();

#ifdef _DEBUG
	m_wpAttackCollider.lock()->Render();
#endif
}

void CProjectileActor::EndPlay()
{
	CMeshActor::EndPlay();
}

void CProjectileActor::Release()
{
	CMeshActor::Release();
}

HRESULT CProjectileActor::BindShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpProjectile.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpProjectile.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpProjectile.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	_float4 vCamPosition;
	XMStoreFloat4(&vCamPosition, CCameraManager::GetInstance()->GetCamPosition());

	if (FAILED(m_wpProjectile.lock()->BindRawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

void CProjectileActor::ProgressToDirrection(const _float& _fTimeDelta)
{
	m_fShootTime += _fTimeDelta;

	// 가속도는 현재 임시임, 넣을지 안넣을지 모름
	SetActorTranslation_NoNavigation(GetActorTranslation() + XMLoadFloat3(&m_vShootDirrection) * m_fShootSpeed * m_fAcceleration);
}

_bool CProjectileActor::CheckDelete()
{
	if (m_fShootTime >= m_fLifeTime)
		return true;

	return false;
}
