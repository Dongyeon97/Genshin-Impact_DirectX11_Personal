#include "pch.h"
#include "SkillIcon.h"

#include "FontManager.h"

#include "VIBuffer_UI.h"

void CSkillIcon::Initialize()
{
	CUIActor_Base::Initialize();
}

_int CSkillIcon::PreUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::PreUpdate(_fTimeDelta);
}

_int CSkillIcon::Update(const _float& _fTimeDelta)
{
	return CUIActor_Base::Update(_fTimeDelta);
}

_int CSkillIcon::LateUpdate(const _float& _fTimeDelta)
{
	return CUIActor_Base::LateUpdate(_fTimeDelta);
}

void CSkillIcon::Render()
{
	BindShaderResources();

	CUIActor_Base::Render();

	// 쿨타임이 돌고 있는 경우
	if (0.f != m_vSkillCoolTime.y - m_vSkillCoolTime.x)
	{
		m_bCoolTimeProgressed = true;

		_float fCoolTimeRatio = _float(_uint(round((m_vSkillCoolTime.y - m_vSkillCoolTime.x) * 10.f))) / 10.f;

		wstring wstrCoolTime = to_wstring(fCoolTimeRatio);

		size_t iCursor = wstrCoolTime.find_last_of(L".");

		for (size_t i = wstrCoolTime.size() - 1; i > iCursor + 1; --i)
			wstrCoolTime.pop_back();

		_vector vColor = XMVectorSet(1.f, 1.f, 1.f, 0.8f);

		if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"), wstrCoolTime, m_vFontPosition, vColor, 0.f, { 0.f,0.f }, m_fCoolTimeFontSize)))
			return;
	}
	// 쿨타임이 다 찬 경우
	else
	{
		m_bCoolTimeProgressed = false;
	}
}

void CSkillIcon::EndPlay()
{
	CUIActor_Base::EndPlay();
}

void CSkillIcon::Release()
{
	CUIActor_Base::Release();
}

HRESULT CSkillIcon::BindShaderResources()
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

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vCoolTime", &m_vSkillCoolTime, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_bCoolTimeProgressed", &m_bCoolTimeProgressed, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}