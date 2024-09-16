#include "pch.h"
#include "HPbar_Frame.h"

#include "VIBuffer_UI.h"

void CHPbar_Frame::Initialize()
{
	CUIActor_Base::Initialize();
}

_int CHPbar_Frame::PreUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::PreUpdate(_fTimeDelta);
}

_int CHPbar_Frame::Update(const _float& _fTimeDelta)
{
	return CUIActor_Base::Update(_fTimeDelta);
}

_int CHPbar_Frame::LateUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::LateUpdate(_fTimeDelta);
}

void CHPbar_Frame::Render()
{
	BindShaderResources();

	CUIActor_Base::Render();
}

void CHPbar_Frame::EndPlay()
{
	CUIActor_Base::EndPlay();
}

void CHPbar_Frame::Release()
{
	CUIActor_Base::Release();
}

HRESULT CHPbar_Frame::BindShaderResources()
{
	if (FAILED(m_wpUI.lock()->BindMatrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vSize", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}