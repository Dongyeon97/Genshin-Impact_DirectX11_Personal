#include "pch.h"
#include "Editor_Navi.h"

#include "CameraManager.h"
#include "NavigationManager.h"

#include "Navigation.h"

void CEditor_Navi::Initialize()
{
	m_eRenderGroup = RENDER_NONBLEND;

	m_wpNavigation = CreateActorComponent<CNavigation>(L"NavigationComponent");
	m_wpNavigation.lock()->SetShaderFileInfo(L"Shader_Cell");
	m_wpNavigation.lock()->SetVertexInfo(VTXPOS::Elements, VTXPOS::iNumElements);
	m_wpNavigation.lock()->SetDataFileName(L"Test");
	m_wpNavigation.lock()->SetStartCellIndex(-1);

	CActor::Initialize();

	// 네비게이션을 매니저에 등록
	CNavigationManager::GetInstance()->AddNavigation(L"Navigation_Editor", m_wpNavigation.lock());
}

_int CEditor_Navi::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CEditor_Navi::Update(const _float& _fDeltaDelta)
{
	m_wpNavigation.lock()->Update(XMLoadFloat4x4(&m_matWorld));

	return CActor::Update(_fDeltaDelta);
}

_int CEditor_Navi::LateUpdate(const _float& _fDeltaDelta)
{
	return CActor::LateUpdate(_fDeltaDelta);
}

void CEditor_Navi::Render()
{
	m_wpNavigation.lock()->Render();

	CActor::Render();
}

void CEditor_Navi::EndPlay()
{
	CActor::EndPlay();
}

void CEditor_Navi::Release()
{

	CActor::Release();
}
