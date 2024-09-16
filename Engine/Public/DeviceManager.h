#pragma once

BEGIN(Engine)

/* 1. ID3D11Device, ID3D11DeviceContext라는 객체를 우선 생성한다. */
/* 2. IDXGISwapChain를 생성한다.(백버퍼(ID3D11Texture2D)도 같이 생성한거야.) */
/* 3. 백버퍼뷰를 생성한다. */
/* 4. 깊이버퍼 텍스쳐를 생성하고 깊이버퍼 뷰를 생성한다. */

class ENGINE_DLL CDeviceManager final
{
	DECLARE_SINGLETON(CDeviceManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize(HWND _hWnd, _uint _iWinSizeX, _uint iWinSizeY);
	void RenderBegin(_float4 vClearColor);
	void RenderEnd();
	void Release();


/********************
	Methods
********************/
public:
	/* 그래픽 디바이스의 초기화. */
	HRESULT Ready_Graphic_Device(const GRAPHIC_DESC& _tGraphicDesc, _Out_ ID3D11Device** _ppDevice,
		_Out_ ID3D11DeviceContext** _ppDeviceContext);

	/* 백버퍼를 지운다. */
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);

	/* 깊이버퍼 + 스텐실버퍼를 지운다. */
	HRESULT Clear_DepthStencil_View();

	/* 후면 버퍼를 전면버퍼로 교체한다.(백버퍼를 화면에 직접 보여준다.) */
	HRESULT Present();

private:
	/* 스왑체인에게 필수적으로 필요한 데이터는 백버퍼가 필요하여 백버퍼를 생성하기위한 정보를 던져준다. */
	/* 스왑체인을 만들었다 == 백버퍼(텍스쳐)가 생성된다. */
	HRESULT Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);


/********************
	Getter/Setter
********************/
public:
	ID3D11Device* GetDevice11() { return m_pDevice; }

	ID3D11DeviceContext* GetDeviceContext11() { return m_pDeviceContext; }

/********************
	Data Members
********************/
private:
	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) 컴객체의 생성과 관련된 역할 */
	ID3D11Device* m_pDevice = nullptr;

	// ComPtr<ID3D11Device> m_pDevice = nullptr;

	/* 기능실행.(바인딩작업, 정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	/* 그린다. */
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };
	
	/* 후면버퍼와 전면버퍼를 교체해가면서 화면에 보여주는 역할 */
	IDXGISwapChain* m_pSwapChain = nullptr;

	/* ID3D11Texture2D : 텍스쳐를 표현하는 사전객체 타입이다.
	why? 용도에 맞는 실질적으로 사용하기 위한 텍스쳐객체를 만들어내기위해.  */
	
	/* ID3D11ShaderResourceView : 셰이더에 전달될 수 있는 텍스처 타입. */
	/* ID3D11RenderTargetView : 렌더타겟용으로 사용될 수 있는 텍스처 타입. */
	/* ID3D11DepthStencilView : 깊이스텐실 버퍼로서 사용될 수 있는 타입.  */
	ID3D11RenderTargetView* m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;


};

END