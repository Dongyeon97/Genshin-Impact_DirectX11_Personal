#include "pch.h"
#include "Logo_LoadingText2.h"

#include "CameraManager.h"

#include "VIBuffer_Rect.h"

void CLogo_LoadingText2::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_wpRectBuffer = CreateActorComponent<CVIBuffer_Rect>(L"Component_VIBuffer_Rect");
	m_wpRectBuffer.lock()->SetShaderFileInfo(L"Shader_VtxPosTex");
	m_wpRectBuffer.lock()->SetTextureFileInfo(L"Logo_KOR_Pure0", 0, 0);
	m_wpRectBuffer.lock()->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_wpRectBuffer.lock()->SetLevelType(Engine::LEVEL_STATIC);

	m_fSizeX = 512;
	m_fSizeY = 344;
	m_fX = Client::g_iWinSizeX * 0.75f;
	m_fY = Client::g_iWinSizeY * 0.7f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Client::g_iWinSizeX, Client::g_iWinSizeY, 0.f, 1.f));

	SetScale(XMVectorSet(m_fSizeX, m_fSizeY, 1.0f, 0.0f));
	SetActorTranslation_NoNavigation(XMVectorSet(m_fX - Client::g_iWinSizeX * 0.5f, -m_fY + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	CActor::Initialize();
}

_int CLogo_LoadingText2::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CLogo_LoadingText2::Update(const _float& _fDeltaDelta)
{
	return CActor::Update(_fDeltaDelta);
}

_int CLogo_LoadingText2::LateUpdate(const _float& _fDeltaDelta)
{
	return CActor::LateUpdate(_fDeltaDelta);
}

void CLogo_LoadingText2::Render()
{
	BindShaderResources();

	CActor::Render();
}

void CLogo_LoadingText2::EndPlay()
{

	CActor::EndPlay();
}

void CLogo_LoadingText2::Release()
{

	CActor::Release();
}

HRESULT CLogo_LoadingText2::BindShaderResources()
{
	if (FAILED(m_wpRectBuffer.lock()->BindMatrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_wpRectBuffer.lock()->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_wpRectBuffer.lock()->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_wpRectBuffer.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_wpRectBuffer.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpRectBuffer.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}
