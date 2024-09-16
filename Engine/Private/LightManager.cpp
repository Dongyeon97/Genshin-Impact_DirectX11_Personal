#include "Engine_pch.h"
#include "LightManager.h"

#include "DeviceManager.h"

#include "Light.h"

IMPLEMENT_SINGLETON(CLightManager)

HRESULT CLightManager::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	return S_OK;
}

HRESULT CLightManager::Render(shared_ptr<CRect_RenderTarget> _spRectRenderTarget)
{
	for (auto& spLight : m_lstLights)
		spLight->Render(_spRectRenderTarget);

	return S_OK;
}

void CLightManager::Release()
{
	for (auto& Light : m_lstLights)
	{
		if (Light)
		{
			Light->Release();
			Light.reset();
		}
	}

	m_lstLights.clear();

	// LightManager ÆÄ±«
	m_upInstance.reset();
}

void CLightManager::ClearCurrentScene()
{
	for (auto& Light : m_lstLights)
	{
		if (nullptr != Light)
		{
			Light->Release();
			Light.reset();
		}
	}
	m_lstLights.clear();
}

HRESULT CLightManager::AddLight(const LIGHT_DESC& _tLightDesc)
{
	shared_ptr<CLight> spLight = make_shared<CLight>();

	spLight->Initialize(_tLightDesc);

	if (nullptr == spLight)
		return E_FAIL;

	m_lstLights.push_back(spLight);

	return S_OK;
}

const LIGHT_DESC* CLightManager::GetLightDesc(_uint _iIndex) const
{
	if (m_lstLights.size() < _iIndex)
		return nullptr;

	auto iter = m_lstLights.begin();

	for (_uint i = 0; i < _iIndex; ++i)
	{
		++iter;
	}

	return (*iter)->GetLightDesc();
}
