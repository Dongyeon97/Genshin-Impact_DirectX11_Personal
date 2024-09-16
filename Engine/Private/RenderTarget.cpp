#include "Engine_pch.h"
#include "RenderTarget.h"

#include "DeviceManager.h"

#include "Rect_RenderTarget.h"

HRESULT CRenderTarget::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
    m_pContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	m_vClearColor = vClearColor;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = ePixelFormat;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;

	return S_OK;
}

shared_ptr<CRenderTarget> CRenderTarget::Create(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    shared_ptr<CRenderTarget> spRenderTarget = make_shared<CRenderTarget>();

    if (FAILED(spRenderTarget->Initialize(iSizeX, iSizeY, ePixelFormat, vClearColor)))
    {
        MSG_BOX("Failed to Create : CRenderTarget");
        spRenderTarget->Release();
        spRenderTarget.reset();
        return nullptr;
    }

    return spRenderTarget;
}

HRESULT CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);

	return S_OK;
}

HRESULT CRenderTarget::BindSRV(shared_ptr<CRect_RenderTarget> _spRectRenderTarget, const _char* pConstantName)
{
	_spRectRenderTarget->BindShaderResource(pConstantName, m_pSRV);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	D3D11_VIEWPORT			ViewPortDesc{};

	_uint		iNumViewports = 1;

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;
	m_WorldMatrix._41 = fX - ViewPortDesc.Width * 0.5f;
	m_WorldMatrix._42 = -fY + ViewPortDesc.Height * 0.5f;

	return S_OK;
}

HRESULT CRenderTarget::Render(shared_ptr<CRect_RenderTarget> spVIBuffer_RenderTarget)
{
	if (FAILED(spVIBuffer_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(spVIBuffer_RenderTarget->BindShaderResource("g_Texture", m_pSRV)))
		return E_FAIL;

	spVIBuffer_RenderTarget->BeginShader(0);

	spVIBuffer_RenderTarget->BindBuffers();

	spVIBuffer_RenderTarget->Render();

	return S_OK;
}
#endif // _DEBUG

void CRenderTarget::Release()
{
    Safe_Release(m_pSRV);
    Safe_Release(m_pRTV);
    Safe_Release(m_pTexture2D);
}