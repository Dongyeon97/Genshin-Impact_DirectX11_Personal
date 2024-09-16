#pragma once

BEGIN(Engine)

class CLight;
class CRect_RenderTarget;

class ENGINE_DLL CLightManager final
{
	DECLARE_SINGLETON(CLightManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize();
	HRESULT Render(shared_ptr<CRect_RenderTarget> _spRectRenderTarget);
	void Release();

/********************
	Methods
********************/
public:
	void ClearCurrentScene();

public:
	HRESULT AddLight(const LIGHT_DESC& _tLightDesc);

/********************
	Getter / Setter
********************/
public:
	const LIGHT_DESC* GetLightDesc(_uint _iIndex) const;

/********************
	Data Members
********************/
private:
	ID3D11Device* m_pDevice = nullptr;

	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	list<shared_ptr<CLight>> m_lstLights;
};

END