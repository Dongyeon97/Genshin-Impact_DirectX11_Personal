#pragma once

BEGIN(Engine)

class CRenderTarget;
class CRect_RenderTarget;

class ENGINE_DLL CRenderTargetManager final
{
	DECLARE_SINGLETON(CRenderTargetManager)
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
	HRESULT AddRenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT AddMRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT BeginMRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSView = nullptr);
	HRESULT EndMRT();
	HRESULT BindSRV(const wstring& strTargetTag, shared_ptr<CRect_RenderTarget> _spRectRenderTarget, const _char* pConstantName);

	HRESULT Copy_BackBufferTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	HRESULT Ready_BackBufferCopyTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, _uint _iWinCX, _uint _iWinCY);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT RenderMRT(const wstring& strMRTTag, shared_ptr<CRect_RenderTarget> spVIBuffer_RenderTarget);
#endif // _DEBUG

private:
	shared_ptr<CRenderTarget> FindRenderTarget(const wstring& strTargetTag);
	list<shared_ptr<CRenderTarget>>* FindMRT(const wstring& strMRTTag);

/********************
	Getter / Setter
********************/
public:
	ID3D11ShaderResourceView* Get_BackBufferCopySRV() { return m_pBackBufferSRV; }

/********************
	Data Members
********************/
private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	ID3D11RenderTargetView*		m_pBackBufferView = { nullptr };
	ID3D11Texture2D*			m_pBackBufferTextureCopy = { nullptr };
	ID3D11ShaderResourceView*	m_pBackBufferSRV = { nullptr };

	ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

private:
	map<const wstring, shared_ptr<CRenderTarget>>			m_mapRenderTargets;

	/* 장치에 동시에 바인딩 되어야할 렌더타겟들을 미리 모아둔다. */
	map<const wstring, list<shared_ptr<CRenderTarget>>>	m_mapMRTs;
};

END