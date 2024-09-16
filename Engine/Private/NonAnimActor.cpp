#include "Engine_pch.h"
#include "NonAnimActor.h"

#include "CameraManager.h"
#include "LightManager.h"
#include "PrefabManager.h"

#include "NonAnimModel.h"
#include "CameraComponent.h"
#include "Mesh.h"

void CNonAnimActor::Initialize()
{
	// 모델 데이터로 받아온 정보 세팅
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
	m_wpNonAnimModel = CreateActorComponent<CNonAnimModel>(L"NonAnimComponent");
	m_wpNonAnimModel.lock()->SetModelFileName(m_wstrModelFileName);
	m_wpNonAnimModel.lock()->SetModelData(m_ModelData);
	m_wpNonAnimModel.lock()->SetSpawnData(m_SpawnData);

	CMeshActor::Initialize();

	SetVertexPositionToWorld(); // 모델의 정점들에 액터의 월드매트릭스를 한번 곱해 초기화합니다.(툴에서 마우스 피킹용)
}

_int CNonAnimActor::PreUpdate(const _float& _fTimeDelta)
{
	return CMeshActor::PreUpdate(_fTimeDelta);
}

_int CNonAnimActor::Update(const _float& _fTimeDelta)
{

	return CMeshActor::Update(_fTimeDelta);
}

_int CNonAnimActor::LateUpdate(const _float& _fTimeDelta)
{

	return CMeshActor::LateUpdate(_fTimeDelta);
}

void CNonAnimActor::Render()
{
	BindShaderResources();

	_uint iNumMeshes = m_wpNonAnimModel.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_wpNonAnimModel.lock()->BindMaterialShaderResource(i, TEXTURE_TYPE::DIFFUSE, "g_DiffuseTexture")))
			return;

		if (FAILED(m_wpNonAnimModel.lock()->BindMaterialShaderResource(i, TEXTURE_TYPE::NORMALS, "g_NormalTexture")))
			return;

		if (FAILED(m_wpNonAnimModel.lock()->BeginShader(0)))
			return;

		m_wpNonAnimModel.lock()->Render(i);
	}

	CMeshActor::Render();
}

void CNonAnimActor::EndPlay()
{
	CMeshActor::EndPlay();
}

void CNonAnimActor::Release()
{

	CMeshActor::Release();
}

HRESULT CNonAnimActor::BindShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpNonAnimModel.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpNonAnimModel.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpNonAnimModel.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	_float fFar = CCameraManager::GetInstance()->GetCurCamera()->GetFar();
	m_wpNonAnimModel.lock()->BindRawValue("g_fFar", &fFar, sizeof(_float));

	return S_OK;
}

void CNonAnimActor::SetVertexPositionToWorld()
{
	vector<shared_ptr<CMesh>>& vecMeshs = m_wpNonAnimModel.lock()->GetVecMeshs();
	_uint iNumMeshs = m_wpNonAnimModel.lock()->GetNumMeshes();

	// 메시들의 정점들을 순회하며 액터의 월드매트릭스를 곱해줍니다.
	for (auto& mesh : vecMeshs)
	{
		vector<_float3>& vecVertexPoses = mesh->GetVecVertexPos();

		for (auto& Vertex : vecVertexPoses)
		{
			XMStoreFloat3(&Vertex, XMVector3Transform(XMLoadFloat3(&Vertex), XMLoadFloat4x4(&m_matWorld)));
		}
	}
}
