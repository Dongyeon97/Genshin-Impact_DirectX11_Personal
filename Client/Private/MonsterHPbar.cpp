#include "pch.h"
#include "MonsterHPbar.h"

#include "CameraManager.h"

#include "VIBuffer_UI.h"
#include "Charactor.h"
#include "CameraComponent.h"

void CMonsterHPbar::Initialize()
{
	CIngameUI::Initialize();
}

_int CMonsterHPbar::PreUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::PreUpdate(_fTimeDelta);
}

_int CMonsterHPbar::Update(const _float& _fTimeDelta)
{
	_vector vViewportPos = CalculateToViewPort();
	SetActorTranslation_NoNavigation(XMVectorSet(XMVectorGetX(vViewportPos) - Client::g_iWinSizeX * 0.5f,
												-XMVectorGetY(vViewportPos) + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	return CIngameUI::Update(_fTimeDelta);
}

_int CMonsterHPbar::LateUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::LateUpdate(_fTimeDelta);
}

void CMonsterHPbar::Render()
{
	BindShaderResources();

	CIngameUI::Render();
}

void CMonsterHPbar::EndPlay()
{
	CIngameUI::EndPlay();
}

void CMonsterHPbar::Release()
{
	CIngameUI::Release();
}

HRESULT CMonsterHPbar::BindShaderResources()
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

	shared_ptr<CCharactor> spCharactor = dynamic_pointer_cast<CCharactor>(GetOwnerActor().lock());

	_float2 vHp = {};

	vHp.x = spCharactor->GetCurHp();
	vHp.y = spCharactor->GetMaxHp();

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vHp", &vHp, sizeof(_float2))))
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
