#pragma once

BEGIN(Engine)

class ENGINE_DLL CTexture final
{
public:
	explicit CTexture() = default;
	virtual ~CTexture() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize();
	void Release();

/********************
	Methods
********************/
public:
	// 셰이더에 리소스를 바인딩합니다.
	HRESULT BindShaderResource(ID3DX11Effect* _pEffect, const _char* _pConstantName, _uint _iIndex);
	HRESULT BindShaderResources(ID3DX11Effect* _pEffect, const _char* _pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures);

/********************
	Getter / Setter
********************/
public:

/********************
	Data Members
********************/
private:
	// [ 텍스처 개수 ]
	_uint m_iNumTextures = 0;

	// [ 텍스처 ]
	vector<ID3D11ShaderResourceView*> m_vecSRVs;
};

END