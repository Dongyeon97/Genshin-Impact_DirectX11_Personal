#include "pch.h"
#include "Stage1_Navi.h"

#include "RenderManager.h"
#include "CameraManager.h"
#include "NavigationManager.h"

#include "Navigation.h"

void CStage1_Navi::Initialize()
{
	m_eRenderGroup = RENDER_NONBLEND;

	m_wpNavigation = CreateActorComponent<CNavigation>(L"NavigationComponent");
	m_wpNavigation.lock()->SetShaderFileInfo(L"Shader_Cell");
	m_wpNavigation.lock()->SetVertexInfo(VTXPOS::Elements, VTXPOS::iNumElements);
	m_wpNavigation.lock()->SetDataFileName(L"Stage1_New");
	m_wpNavigation.lock()->SetStartCellIndex(0);

	CActor::Initialize();

	// 네비게이션을 매니저에 등록
	CNavigationManager::GetInstance()->AddNavigation(L"Navigation_Stage1", m_wpNavigation.lock());
}

_int CStage1_Navi::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CStage1_Navi::Update(const _float& _fDeltaDelta)
{
	m_wpNavigation.lock()->Update(XMLoadFloat4x4(&m_matWorld));

	return CActor::Update(_fDeltaDelta);
}

_int CStage1_Navi::LateUpdate(const _float& _fDeltaDelta)
{
#ifdef _DEBUG
	CRenderManager::GetInstance()->AddDebugComponent(m_wpNavigation.lock());
#endif // _DEBUG

	return CActor::LateUpdate(_fDeltaDelta);
}

void CStage1_Navi::Render()
{
	CActor::Render();
}

void CStage1_Navi::EndPlay()
{
	CActor::EndPlay();
}

void CStage1_Navi::Release()
{
	CActor::Release();
}