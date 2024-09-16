#include "pch.h"
#include "HPbar.h"

#include "Charactor.h"
#include "VIBuffer_UI.h"

void CHPbar::Initialize()
{
	CUIActor_Base::Initialize();
}

_int CHPbar::PreUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::PreUpdate(_fTimeDelta);
}

_int CHPbar::Update(const _float& _fTimeDelta)
{
	return CUIActor_Base::Update(_fTimeDelta);
}

_int CHPbar::LateUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::LateUpdate(_fTimeDelta);
}

void CHPbar::Render()
{
	BindShaderResources();

	CUIActor_Base::Render();
}

void CHPbar::EndPlay()
{
	CUIActor_Base::EndPlay();
}

void CHPbar::Release()
{
	CUIActor_Base::Release();
}

HRESULT CHPbar::BindShaderResources()
{
	if (FAILED(m_wpUI.lock()->BindMatrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	shared_ptr<CCharactor> spCharactor = dynamic_pointer_cast<CCharactor>(GetOwnerActor().lock());

	_float2 vHp = {};

	vHp.x = spCharactor->GetCurHp();
	vHp.y = spCharactor->GetMaxHp();
	_float fHPRatio = vHp.x / vHp.y;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vSize", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vHp", &vHp, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_fHPRatio", &fHPRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}