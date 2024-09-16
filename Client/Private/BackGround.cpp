#include "pch.h"
#include "BackGround.h"

#include "CameraManager.h"

#include "VIBuffer_Rect.h"

void CBackGround::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_wpRectBuffer = CreateActorComponent<CVIBuffer_Rect>(L"Component_VIBuffer_Rect");
	m_wpRectBuffer.lock()->SetShaderFileInfo(L"Shader_VtxPosTex");
	m_wpRectBuffer.lock()->SetTextureFileInfo(L"UI_Download_Bg0", 0, 0);
	m_wpRectBuffer.lock()->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_wpRectBuffer.lock()->SetLevelType(Engine::LEVEL_STATIC);

	m_fSizeX = 1280;
	m_fSizeY = 720;
	m_fX = Client::g_iWinSizeX * 0.5f;
	m_fY = Client::g_iWinSizeY * 0.5f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Client::g_iWinSizeX, Client::g_iWinSizeY, 0.f, 1.f));

	SetScale(XMVectorSet(m_fSizeX, m_fSizeY, 1.0f, 0.0f));
	SetActorTranslation_NoNavigation(XMVectorSet(m_fX - Client::g_iWinSizeX * 0.5f, -m_fY + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	CActor::Initialize();
}

_int CBackGround::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CBackGround::Update(const _float& _fDeltaDelta)
{
	return CActor::Update(_fDeltaDelta);
}

_int CBackGround::LateUpdate(const _float& _fDeltaDelta)
{
	return CActor::LateUpdate(_fDeltaDelta);
}

void CBackGround::Render()
{
	BindShaderResources();

	CActor::Render();
}

void CBackGround::EndPlay()
{

	CActor::EndPlay();
}

void CBackGround::Release()
{

	CActor::Release();
}

HRESULT CBackGround::BindShaderResources()
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
