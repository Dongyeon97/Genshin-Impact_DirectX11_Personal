#include "pch.h"
#include "SidePortrait.h"

#include "FontManager.h"

#include "VIBuffer_UI.h"

void CSidePortrait::Initialize()
{
	CUIActor_Base::Initialize();
}

_int CSidePortrait::PreUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::PreUpdate(_fTimeDelta);
}

_int CSidePortrait::Update(const _float& _fTimeDelta)
{
	return CUIActor_Base::Update(_fTimeDelta);
}

_int CSidePortrait::LateUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::LateUpdate(_fTimeDelta);
}

void CSidePortrait::Render()
{
	BindShaderResources();

	CUIActor_Base::Render();

	if (!m_wstrCharactorName.empty())
	{
		if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"), m_wstrCharactorName, m_vFontPosition, { 0.98f, 0.98f, 0.98f, 1.0f }, 0.f, { 0.f,0.f }, 0.5f)))
			return;
	}
}

void CSidePortrait::EndPlay()
{
	CUIActor_Base::EndPlay();
}

void CSidePortrait::Release()
{
	CUIActor_Base::Release();
}

HRESULT CSidePortrait::BindShaderResources()
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
