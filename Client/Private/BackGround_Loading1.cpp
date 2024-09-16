#include "pch.h"
#include "BackGround_Loading1.h"

#include "CameraManager.h"

#include "VIBuffer_Rect.h"

void CBackGround_Loading1::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_spRectBuffer = CreateActorComponent<CVIBuffer_Rect>(L"Component_VIBuffer_Rect");
	m_spRectBuffer->SetShaderFileInfo(L"Shader_VtxPosTex");
	m_spRectBuffer->SetTextureFileInfo(L"Zhongli_0", 0, 0);
	m_spRectBuffer->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_spRectBuffer->SetLevelType(Engine::LEVEL_STATIC);

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

_int CBackGround_Loading1::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CBackGround_Loading1::Update(const _float& _fDeltaDelta)
{
	return CActor::Update(_fDeltaDelta);
}

_int CBackGround_Loading1::LateUpdate(const _float& _fDeltaDelta)
{
	return CActor::LateUpdate(_fDeltaDelta);
}

void CBackGround_Loading1::Render()
{
	BindShaderResources();

	CActor::Render();
}

void CBackGround_Loading1::EndPlay()
{

	CActor::EndPlay();
}

void CBackGround_Loading1::Release()
{

	CActor::Release();
}

HRESULT CBackGround_Loading1::BindShaderResources()
{
	if (FAILED(m_spRectBuffer->BindMatrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_spRectBuffer->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRectBuffer->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_spRectBuffer->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_spRectBuffer->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_spRectBuffer->BindBuffers()))
		return E_FAIL;

	return S_OK;
}
