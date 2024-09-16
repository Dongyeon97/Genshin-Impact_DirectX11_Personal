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
	// ���̴��� ���ҽ��� ���ε��մϴ�.
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
	// [ �ؽ�ó ���� ]
	_uint m_iNumTextures = 0;

	// [ �ؽ�ó ]
	vector<ID3D11ShaderResourceView*> m_vecSRVs;
};

END