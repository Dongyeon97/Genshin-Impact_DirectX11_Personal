#include "pch.h"
#include "UIActor_Base.h"

#include "VIBuffer_UI.h"

void CUIActor_Base::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_wpUI = CreateActorComponent<CVIBuffer_UI>(L"Component_VIBuffer_UI");

	// 자식 클래스에서 셰이더파일, 텍스처 파일, 위치, 크기 세팅해주기
	m_wpUI.lock()->SetShaderFileInfo(m_wstrShaderFileName);
	m_wpUI.lock()->SetTextureFileInfo(m_wstrTextureFileName, 0, 0);
	m_wpUI.lock()->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_wpUI.lock()->SetLevelType(Engine::LEVEL_STATIC);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Client::g_iWinSizeX, Client::g_iWinSizeY, 0.f, 1.f));

	// SetScale(XMVectorSet(m_fSizeX, m_fSizeY, 1.0f, 0.0f));
	SetActorTranslation_NoNavigation(XMVectorSet(m_fX - Client::g_iWinSizeX * 0.5f, -m_fY + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	CActor::Initialize();
}

_int CUIActor_Base::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CUIActor_Base::Update(const _float& _fTimeDelta)
{
	return CActor::Update(_fTimeDelta);
}

_int CUIActor_Base::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CUIActor_Base::Render()
{
	// BindShaderResources();

	if(m_bEnable)
		CActor::Render();
}

void CUIActor_Base::EndPlay()
{
	CActor::EndPlay();
}

void CUIActor_Base::Release()
{
	CActor::Release();
}

void CUIActor_Base::SetPosition(const _float2& _vPosition)
{
	m_fX = _vPosition.x;
	m_fY = _vPosition.y;
}

void CUIActor_Base::SetSize(const _float2& _vSize)
{
	m_vSize.x = _vSize.x;
	m_vSize.y = _vSize.y;
}
