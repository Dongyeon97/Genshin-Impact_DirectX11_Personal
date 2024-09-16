#pragma once

BEGIN(Engine)

class CRect_RenderTarget;

class ENGINE_DLL CRenderTarget
{
public:
	explicit CRenderTarget() = default;
	virtual ~CRenderTarget() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	void Release();

	static shared_ptr<CRenderTarget> Create(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
/********************
	Methods
********************/
public:
	HRESULT Clear();
	HRESULT BindSRV(shared_ptr<CRect_RenderTarget> _spRectRenderTarget, const _char* pConstantName);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(shared_ptr<CRect_RenderTarget> spVIBuffer_RenderTarget);
#endif // _DEBUG

/********************
	Getter / Setter
********************/
public:
	ID3D11RenderTargetView* GetRTV() const { return m_pRTV; }

/********************
	Data Members
********************/
private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;

	ID3D11Texture2D*			m_pTexture2D = nullptr;
	ID3D11RenderTargetView*		m_pRTV = nullptr;
	ID3D11ShaderResourceView*	m_pSRV = nullptr;

private:
	_float4 m_vClearColor;

#ifdef _DEBUG
	_float4x4 m_WorldMatrix;
#endif // _DEBUG
};

END