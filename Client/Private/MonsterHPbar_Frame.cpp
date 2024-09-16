#include "pch.h"
#include "MonsterHPbar_Frame.h"

#include "CameraManager.h"

#include "VIBuffer_UI.h"

void CMonsterHPbar_Frame::Initialize()
{
	CIngameUI::Initialize();
}

_int CMonsterHPbar_Frame::PreUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::PreUpdate(_fTimeDelta);
}

_int CMonsterHPbar_Frame::Update(const _float& _fTimeDelta)
{
	_vector vViewportPos = CalculateToViewPort();
	SetActorTranslation_NoNavigation(XMVectorSet(XMVectorGetX(vViewportPos) - Client::g_iWinSizeX * 0.5f,
												-XMVectorGetY(vViewportPos) + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	return CIngameUI::Update(_fTimeDelta);
}

_int CMonsterHPbar_Frame::LateUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::LateUpdate(_fTimeDelta);
}

void CMonsterHPbar_Frame::Render()
{
	BindShaderResources();

	CIngameUI::Render();
}

void CMonsterHPbar_Frame::EndPlay()
{
	CIngameUI::EndPlay();
}

void CMonsterHPbar_Frame::Release()
{
	CIngameUI::Release();
}

HRESULT CMonsterHPbar_Frame::BindShaderResources()
{
	if (m_wpOwnerActor.expired())
		return E_FAIL;

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

	if (FAILED(m_wpUI.lock()->BindRawValue("g_fTimeAcc", &m_fRenderTimeAcc, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}
