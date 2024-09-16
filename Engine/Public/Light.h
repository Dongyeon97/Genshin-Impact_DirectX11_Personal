#pragma once

BEGIN(Engine)

class CRect_RenderTarget;

class ENGINE_DLL CLight final : public enable_shared_from_this<CLight>
{
public:
	explicit CLight() = default;
	virtual ~CLight() = default;
/********************
	Framework
********************/
public:
	HRESULT Initialize(const LIGHT_DESC& _tLightDesc);
	HRESULT Render(shared_ptr<CRect_RenderTarget> _spRectRenderTarget);
	void	Release();

/********************
	Methods
********************/
public:


/********************
	Getter / Setter
********************/
public:
	const LIGHT_DESC* GetLightDesc() const { return &m_tLightDesc; }


/********************
	Data Members
********************/
private:
	ID3D11Device* m_pDevice = nullptr;

	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	LIGHT_DESC m_tLightDesc = {};
};

END