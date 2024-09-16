#include "Engine_pch.h"
#include "AnimActor.h"

#include "RenderManager.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "PrefabManager.h"

#include "AnimModel.h"
#include "CameraComponent.h"

void CAnimActor::Initialize()
{
	// 모델 데이터로부터 받아온 정보 세팅
	m_eRenderGroup = m_SpawnData.tPrefab.eRenderGroup;
	m_wstrModelFileName = L"";
	for (_uint i = 0; i < MAX_PATH; ++i)
	{
		if (m_SpawnData.tPrefab.szModelFileName[i] == '\0') { break; }
		m_wstrModelFileName += m_SpawnData.tPrefab.szModelFileName[i];
	}

	// Scale / Rotation / Translate
	XMFLOAT3 vScale = m_SpawnData.tPrefab.vScale;
	XMStoreFloat4(&m_vScale, XMVectorSet(vScale.x, vScale.y, vScale.z, 0.0f));

	XMFLOAT3 vRotation = m_SpawnData.tPrefab.vRotation;
	XMStoreFloat4(&m_vRotation, XMVectorSet(vRotation.x, vRotation.y, vRotation.z, 0.0f));

	XMFLOAT3 vTranslate = m_SpawnData.tPrefab.vTranslate;
	XMStoreFloat4(&m_vTranslate, XMVectorSet(vTranslate.x, vTranslate.y, vTranslate.z, 0.0f));

	// 모델 컴포넌트를 생성하고, 모델 데이터를 컴포넌트에 전송
	m_wpAnimModel = CreateActorComponent<CAnimModel>(L"AnimModel");
	m_wpAnimModel.lock()->SetModelFileName(m_wstrModelFileName);
	m_wpAnimModel.lock()->SetModelData(m_ModelData);
	m_wpAnimModel.lock()->SetSpawnData(m_SpawnData);

	m_wpAnimModel.lock()->SetAdditionalPivot(XMLoadFloat4x4(&m_matAdditionalPivot));

	CMeshActor::Initialize();
}

_int CAnimActor::PreUpdate(const _float& _fTimeDelta)
{
	return CMeshActor::PreUpdate(_fTimeDelta);
}

_int CAnimActor::Update(const _float& _fTimeDelta)
{
	return CMeshActor::Update(_fTimeDelta);
}

_int CAnimActor::LateUpdate(const _float& _fTimeDelta)
{
	// CRenderManager::GetInstance()->AddObjectToRenderGroup(RENDER_SHADOW, dynamic_pointer_cast<CActor>(shared_from_this()));

	return CMeshActor::LateUpdate(_fTimeDelta);
}

void CAnimActor::Render()
{
	if (FAILED(BindShaderResources()))
	{
		MSG_BOX("Failed to BindShaderResources : CAnimActor");
		return;
	}

	_uint iNumMeshes = m_wpAnimModel.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_wpAnimModel.lock()->BindMaterialShaderResource(i, TEXTURE_TYPE::DIFFUSE, "g_DiffuseTexture")))
		{
			MSG_BOX("Failed to BindMaterialShaderResource : CAnimActor");
			return;
		}

		if (FAILED(m_wpAnimModel.lock()->BindBoneMatrices("g_BoneMatrices", i)))
		{
			MSG_BOX("Failed to BindBoneMatrices : CAnimActor");
			return;
		}

		if (FAILED(m_wpAnimModel.lock()->BeginShader(0)))
		{
			MSG_BOX("Failed to BeginShader : CAnimActor");
			return;
		}

		m_wpAnimModel.lock()->Render(i);
	}

	CMeshActor::Render();
}

void CAnimActor::Render_Shadow()
{
	if (BindShaderResources_Shadow())
	{
		MSG_BOX("Failed to BindShaderResources : CAnimActor");
		return;
	}

	_uint iNumMeshes = m_wpAnimModel.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_wpAnimModel.lock()->BindBoneMatrices("g_BoneMatrices", i)))
		{
			MSG_BOX("Failed to BindBoneMatrices : CAnimActor");
			return;
		}

		if (FAILED(m_wpAnimModel.lock()->BeginShader(1)))
		{
			MSG_BOX("Failed to BeginShader : CAnimActor");
			return;
		}

		m_wpAnimModel.lock()->Render(i);
	}

	// CMeshActor::Render();
}

void CAnimActor::EndPlay()
{
	CMeshActor::EndPlay();
}

void CAnimActor::Release()
{
	CMeshActor::Release();
}

HRESULT CAnimActor::BindShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpAnimModel.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpAnimModel.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpAnimModel.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	_float fFar = CCameraManager::GetInstance()->GetCurCamera()->GetFar();
	m_wpAnimModel.lock()->BindRawValue("g_fFar", &fFar, sizeof(_float));

	return S_OK;
}

HRESULT CAnimActor::BindShaderResources_Shadow()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(2000.f, 2000.f, 2000.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), (_float)1280.0f / 720.0f, 0.1f, 1000000.f));

	m_wpAnimModel.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpAnimModel.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpAnimModel.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	return S_OK;
}
