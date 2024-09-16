#include "Engine_pch.h"
#include "Texture.h"

HRESULT CTexture::Initialize()
{
	return S_OK;
}

void CTexture::Release()
{
	for (auto& pSRV : m_vecSRVs)
	{
		if (nullptr != pSRV)
			pSRV->Release();
	}

	m_vecSRVs.clear();
}

HRESULT CTexture::BindShaderResource(ID3DX11Effect* _pEffect, const _char* _pConstantName, _uint _iIndex)
{
	if (_iIndex > _iIndex ||
		_iIndex < 0)
		return E_FAIL;

	return S_OK;

	// return _pEffect->Bind_SRV(pConstantName, m_SRVs[iIndex]);
}

HRESULT CTexture::BindShaderResources(ID3DX11Effect* _pEffect, const _char* _pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures)
{

	return S_OK;
}