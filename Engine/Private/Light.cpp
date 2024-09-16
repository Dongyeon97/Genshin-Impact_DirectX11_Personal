#include "Engine_pch.h"
#include "Light.h"

#include "Rect_RenderTarget.h"

HRESULT CLight::Initialize(const LIGHT_DESC& _tLightDesc)
{
	m_tLightDesc = _tLightDesc;

	return S_OK;
}

HRESULT CLight::Render(shared_ptr<CRect_RenderTarget> _spRectRenderTarget)
{
	_uint iPassIndex = 0;

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_tLightDesc.eLightType)
	{
		iPassIndex = 1;

		if (FAILED(_spRectRenderTarget->BindRawValue("g_vLightDir", &m_tLightDesc.vLightDir, sizeof(_float4))))
			return E_FAIL;
	}
	else if (LIGHT_DESC::TYPE_POINT == m_tLightDesc.eLightType)
	{
		iPassIndex = 2;

		if (FAILED(_spRectRenderTarget->BindRawValue("g_vLightPos", &m_tLightDesc.vLightPos, sizeof(_float4))))
			return E_FAIL;

		if (FAILED(_spRectRenderTarget->BindRawValue("g_fLightRange", &m_tLightDesc.fRange, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(_spRectRenderTarget->BindRawValue("g_vLightDiffuse", &m_tLightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_spRectRenderTarget->BindRawValue("g_vLightAmbient", &m_tLightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(_spRectRenderTarget->BindRawValue("g_vLightSpecular", &m_tLightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(_spRectRenderTarget->BeginShader(iPassIndex)))
		return E_FAIL;

	if (FAILED(_spRectRenderTarget->BindBuffers()))
		return E_FAIL;

	_spRectRenderTarget->Render();

	return S_OK;
}

void CLight::Release()
{

}
