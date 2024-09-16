#include "pch.h"
#include "Test_Object_1.h"

#include "InputManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "LightManager.h"

#include "NonAnimModel.h"

void CTest_Object_1::Initialize()
{
	m_eRenderGroup = RENDER_BLEND;

	_matrix			PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	m_wpNonAnimModel = CreateActorComponent<CNonAnimModel>(L"NonAnimModel");
	m_wpNonAnimModel.lock()->SetActorTranslation(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	m_wpNonAnimModel.lock()->SetScale(XMVectorSet(250.0f, 250.0f, 250.0f, 0.0f));
	m_wpNonAnimModel.lock()->SetShaderFileInfo(L"Shader_VtxMesh");
	m_wpNonAnimModel.lock()->SetVertexInfo(VTXMESH::Elements, VTXMESH::iNumElements);
	m_wpNonAnimModel.lock()->SetModelFileInfo(MODEL_NONANIM, L"Test_Object_1", PivotMatrix);
	m_wpNonAnimModel.lock()->SetLevelType(Engine::LEVEL_DYNAMIC);

	CActor::Initialize();
}

_int CTest_Object_1::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CTest_Object_1::Update(const _float& _fTimeDelta)
{

	return CActor::Update(_fTimeDelta);
}

_int CTest_Object_1::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CTest_Object_1::Render()
{
	if (FAILED(BindShaderResources()))
		return;

	_uint iNumMeshes = m_wpNonAnimModel.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_wpNonAnimModel.lock()->BindMaterialShaderResource(i, TEXTURE_TYPE::DIFFUSE, "g_DiffuseTexture")))
			return;

		if (FAILED(m_wpNonAnimModel.lock()->BeginShader(0)))
			return;

		m_wpNonAnimModel.lock()->Render(i);
	}

	CActor::Render();
}

void CTest_Object_1::EndPlay()
{
	CActor::EndPlay();
}

void CTest_Object_1::Release()
{
	CActor::Release();
}

HRESULT CTest_Object_1::BindShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());
	XMStoreFloat4x4(&WorldMatrix, m_wpNonAnimModel.lock()->GetWorldMatrix());

	m_wpNonAnimModel.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpNonAnimModel.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpNonAnimModel.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	return S_OK; 
}
