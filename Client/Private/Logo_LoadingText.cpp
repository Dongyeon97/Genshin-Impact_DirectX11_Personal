#include "pch.h"
#include "Logo_LoadingText.h"

#include "CameraManager.h"

#include "VIBuffer_Rect.h"

void CLogo_LoadingText::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_wpRectBuffer = CreateActorComponent<CVIBuffer_Rect>(L"Component_VIBuffer_Rect");
	m_wpRectBuffer.lock()->SetShaderFileInfo(L"Shader_VtxPosTex");
	m_wpRectBuffer.lock()->SetTextureFileInfo(L"Logo_KOR", 0, 0);
	m_wpRectBuffer.lock()->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_wpRectBuffer.lock()->SetLevelType(Engine::LEVEL_STATIC);

	m_fSizeX = 512;
	m_fSizeY = 512;
	m_fX = Client::g_iWinSizeX * 0.7f;
	m_fY = Client::g_iWinSizeY * 0.3f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Client::g_iWinSizeX, Client::g_iWinSizeY, 0.f, 1.f));

	SetScale(XMVectorSet(m_fSizeX, m_fSizeY, 1.0f, 0.0f));
	SetActorTranslation_NoNavigation(XMVectorSet(m_fX - Client::g_iWinSizeX * 0.5f, -m_fY + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	CActor::Initialize();
}

_int CLogo_LoadingText::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CLogo_LoadingText::Update(const _float& _fDeltaDelta)
{
	return CActor::Update(_fDeltaDelta);
}

_int CLogo_LoadingText::LateUpdate(const _float& _fDeltaDelta)
{
	return CActor::LateUpdate(_fDeltaDelta);
}

void CLogo_LoadingText::Render()
{
	BindShaderResources();

	CActor::Render();
}

void CLogo_LoadingText::EndPlay()
{

	CActor::EndPlay();
}

void CLogo_LoadingText::Release()
{

	CActor::Release();
}

HRESULT CLogo_LoadingText::BindShaderResources()
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
