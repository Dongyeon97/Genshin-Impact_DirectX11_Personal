#include "pch.h"
#include "Skybox.h"

#include "CameraManager.h"

#include "VIBuffer_Cube.h"

void CSkybox::Initialize()
{
	m_eRenderGroup = RENDER_PRIORITY;

	SetScale(XMVectorSet(10000.f, 10000.f, 10000.f, 0.0f));

	m_wpCubeBuffer = CreateActorComponent<CVIBuffer_Cube>(L"VIBuffer_Cube");
	m_wpCubeBuffer.lock()->SetShaderFileInfo(L"Shader_VtxCube");
	m_wpCubeBuffer.lock()->SetTextureFileInfo(L"Sky_Red", 0, 0);
	m_wpCubeBuffer.lock()->SetVertexInfo(VTXCUBE::Elements, VTXCUBE::iNumElements);
	m_wpCubeBuffer.lock()->SetLevelType(Engine::LEVEL_STATIC);

	CActor::Initialize();
}

_int CSkybox::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CSkybox::Update(const _float& _fTimeDelta)
{
	return CActor::Update(_fTimeDelta);
}

_int CSkybox::LateUpdate(const _float& _fTimeDelta)
{
	_vector vCamPosition = CCameraManager::GetInstance()->GetCamPosition();

	SetActorTranslation_NoNavigation(vCamPosition);

	return CActor::LateUpdate(_fTimeDelta);
}

void CSkybox::Render()
{
	BindShaderResources();

	CActor::Render();
}

void CSkybox::EndPlay()
{
	CActor::EndPlay();
}

void CSkybox::Release()
{
	CActor::Release();
}

HRESULT CSkybox::BindShaderResources()
{
	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpCubeBuffer.lock()->BindMatrix("g_WorldMatrix", &m_matWorld);
	m_wpCubeBuffer.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpCubeBuffer.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	if (FAILED(m_wpCubeBuffer.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_wpCubeBuffer.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpCubeBuffer.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}
