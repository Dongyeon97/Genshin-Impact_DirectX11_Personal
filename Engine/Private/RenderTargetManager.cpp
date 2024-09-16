#include "Engine_pch.h"
#include "RenderTargetManager.h"

#include "DeviceManager.h"

#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CRenderTargetManager)

HRESULT CRenderTargetManager::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	return S_OK;
}

HRESULT CRenderTargetManager::AddRenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	if (nullptr != FindRenderTarget(strTargetTag))
		return E_FAIL;

	shared_ptr<CRenderTarget> spRenderTarget = CRenderTarget::Create(iSizeX, iSizeY, ePixelFormat, vClearColor);

	if (nullptr == spRenderTarget)
		return E_FAIL;

	m_mapRenderTargets.emplace(strTargetTag, spRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetManager::AddMRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
	shared_ptr<CRenderTarget> spRenderTarget = FindRenderTarget(strTargetTag);

	if (nullptr == spRenderTarget)
		return E_FAIL;

	list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(strMRTTag);

	if (nullptr == pMRTList)
	{
		list<shared_ptr<CRenderTarget>> MRTList;
		MRTList.push_back(spRenderTarget);

		m_mapMRTs.emplace(strMRTTag, MRTList);
	}
	else
	{
		pMRTList->push_back(spRenderTarget);
	}

	return S_OK;
}

HRESULT CRenderTargetManager::BeginMRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSView)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(strMRTTag);
	ID3D11RenderTargetView* pRTVs[8] = { nullptr };

	m_pContext->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);

	_uint iNumRTVs = 0;

	for (auto& spRenderTarget : *pMRTList)
	{
		spRenderTarget->Clear();
		pRTVs[iNumRTVs++] = spRenderTarget->GetRTV();
	}

	m_pContext->OMSetRenderTargets(iNumRTVs, pRTVs, m_pDepthStencilView);

	return S_OK;
}

HRESULT CRenderTargetManager::EndMRT()
{
	m_pContext->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pDepthStencilView);

	return S_OK;
}

HRESULT CRenderTargetManager::BindSRV(const wstring& strTargetTag, shared_ptr<CRect_RenderTarget> _spRectRenderTarget, const _char* pConstantName)
{
	shared_ptr<CRenderTarget> spRectRenderTarget = FindRenderTarget(strTargetTag);
	if (nullptr == spRectRenderTarget)
		return E_FAIL;

	return spRectRenderTarget->BindSRV(_spRectRenderTarget, pConstantName);
}

HRESULT CRenderTargetManager::Copy_BackBufferTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	/* Get Current RenderTarget (BackBuffer) */
	ID3D11RenderTargetView* pBackBufferRTV = nullptr;
	_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

	/* Get BackBuffer RenderTarget Texture */
	ID3D11Resource* pBackBufferResource = nullptr;
	pBackBufferRTV->GetResource(&pBackBufferResource);

	/* Copy the BackBuffer Texture into "m_pBackBufferTextureCopy" */
	_pContext->CopyResource(m_pBackBufferTextureCopy, pBackBufferResource);

	Safe_Release(m_pBackBufferSRV);

	/* Make a Shader Resource View based on the copied "m_pBackBufferTextureCopy" */
	if (FAILED(_pDevice->CreateShaderResourceView(m_pBackBufferTextureCopy, nullptr, &m_pBackBufferSRV)))
		return E_FAIL;

	Safe_Release(pBackBufferResource);
	Safe_Release(pBackBufferRTV);

	return S_OK;
}

shared_ptr<CRenderTarget> CRenderTargetManager::FindRenderTarget(const wstring& strTargetTag)
{
	auto iter = m_mapRenderTargets.find(strTargetTag);

	if (iter == m_mapRenderTargets.end())
		return nullptr;

	return iter->second;
}

list<shared_ptr<CRenderTarget>>* CRenderTargetManager::FindMRT(const wstring& strMRTTag)
{
	auto iter = m_mapMRTs.find(strMRTTag);

	if (iter == m_mapMRTs.end())
		return nullptr;
	
	return &iter->second;
}

HRESULT CRenderTargetManager::Ready_BackBufferCopyTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _uint _iWinCX, _uint _iWinCY)
{
	/* Get Current RenderTarget (BackBuffer) */
	ID3D11RenderTargetView* pBackBufferRTV = nullptr;
	_pContext->OMGetRenderTargets(1, &pBackBufferRTV, nullptr);

	/* Get BackBuffer RenderTarget Description */
	D3D11_RENDER_TARGET_VIEW_DESC pBackBufferDesc;
	pBackBufferRTV->GetDesc(&pBackBufferDesc);

	D3D11_TEXTURE2D_DESC tBackBufferCopyDesc;
	ZeroMemory(&tBackBufferCopyDesc, sizeof(D3D11_TEXTURE2D_DESC));

	tBackBufferCopyDesc.Width = _iWinCX;
	tBackBufferCopyDesc.Height = _iWinCY;
	tBackBufferCopyDesc.MipLevels = 1;
	tBackBufferCopyDesc.ArraySize = 1;
	tBackBufferCopyDesc.Format = pBackBufferDesc.Format;
	tBackBufferCopyDesc.SampleDesc.Quality = 0;
	tBackBufferCopyDesc.SampleDesc.Count = 1;
	tBackBufferCopyDesc.Usage = D3D11_USAGE_DEFAULT;
	tBackBufferCopyDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tBackBufferCopyDesc.CPUAccessFlags = 0;
	tBackBufferCopyDesc.MiscFlags = 0;

	if (FAILED(_pDevice->CreateTexture2D(&tBackBufferCopyDesc, nullptr, &m_pBackBufferTextureCopy)))
		return E_FAIL;

	Safe_Release(pBackBufferRTV);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTargetManager::Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	shared_ptr<CRenderTarget> spRenderTarget = FindRenderTarget(strTargetTag);
	if (nullptr == spRenderTarget)
		return E_FAIL;

	return spRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTargetManager::RenderMRT(const wstring& strMRTTag, shared_ptr<CRect_RenderTarget> spVIBuffer_RenderTarget)
{
	list<shared_ptr<CRenderTarget>>* pMRTList = FindMRT(strMRTTag);

	for (auto& spRenderTarget : *pMRTList)
	{
		spRenderTarget->Render(spVIBuffer_RenderTarget);
	}

	return S_OK;
}
#endif // _DEBUG

void CRenderTargetManager::Release()
{
	for (auto& Pair : m_mapMRTs)
	{
		for (auto& RenderTarget : Pair.second)
		{
			if (nullptr != RenderTarget)
			{
				RenderTarget->Release();
				RenderTarget.reset();
			}
		}
		Pair.second.clear();
	}

	m_mapMRTs.clear();

	for (auto& Pair : m_mapRenderTargets)
	{
		if (nullptr != Pair.second)
		{
			Pair.second->Release();
			Pair.second.reset();
		}
	}

	m_mapRenderTargets.clear();

	// Safe_Release(m_pBackBufferView);
	Safe_Release(m_pBackBufferTextureCopy);
	Safe_Release(m_pBackBufferSRV);

	m_upInstance.reset();
}