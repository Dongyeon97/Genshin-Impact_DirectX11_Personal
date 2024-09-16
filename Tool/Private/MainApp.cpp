#include "pch.h"
#include "MainApp.h"

#include "DeviceManager.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "LightManager.h"
#include "NavigationManager.h"
#include "RenderTargetManager.h"

#include "ImguiManager.h"

#include "ResourceManager.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "AssimpManager.h"

#include "Level_Editor.h"

HRESULT CMainApp::PreInitialize(HINSTANCE _hInst, HWND _hWnd)
{
	// [ System ]
	SUCCEEDED(CDeviceManager::GetInstance()->Initialize(_hWnd, Client::g_iWinSizeX, Client::g_iWinSizeY));
	SUCCEEDED(CInputManager::GetInstance()->Initialize(_hInst, _hWnd));

	return S_OK;
}

HRESULT CMainApp::Initialize()
{
	// [ Engine ]
	SUCCEEDED(CResourceManager::GetInstance()->Initialize(PROJECT_TOOL));
	SUCCEEDED(CRenderTargetManager::GetInstance()->Initialize());
	SUCCEEDED(CRenderManager::GetInstance()->Initialize());
	SUCCEEDED(CAssimpManager::GetInstance()->Initialize());
	SUCCEEDED(CCollisionManager::GetInstance()->Initialize());
	SUCCEEDED(CObjectManager::GetInstance()->Initialize(2, Client::LAYER_END));
	SUCCEEDED(CCameraManager::GetInstance()->Initialize());
	SUCCEEDED(CLightManager::GetInstance()->Initialize());
	SUCCEEDED(CPrefabManager::GetInstance()->Initialize());
	SUCCEEDED(CSpawnManager::GetInstance()->Initialize());
	SUCCEEDED(CNavigationManager::GetInstance()->Initialize());
	SUCCEEDED(CImguiManager::GetInstance()->Initialize());

	if (FAILED(OpenLevel()))
		return E_FAIL;

	return S_OK;
}

_int CMainApp::PreUpdate(const _float& _fTimeDelta)
{
	CObjectManager::GetInstance()->PreUpdate(_fTimeDelta);

	return S_OK;
}

_int CMainApp::Update(const _float& _fTimeDelta)
{
	CInputManager::GetInstance()->Update();

	CObjectManager::GetInstance()->Update(_fTimeDelta);

	return S_OK;
}

_int CMainApp::LateUpdate(const _float& _fTimeDelta)
{
	CObjectManager::GetInstance()->LateUpdate(_fTimeDelta);

	return S_OK;
}

void CMainApp::Render()
{
	// 렌더 시작
	CDeviceManager::GetInstance()->RenderBegin(_float4(1.f, 1.f, 1.f, 1.f));

	// [ Render ]
	// CObjectManager::GetInstance()->Render();
	CRenderManager::GetInstance()->Render();
	CImguiManager::GetInstance()->Render();

	// [ Present ]
	CDeviceManager::GetInstance()->Present();

	// 렌더 끝
	CDeviceManager::GetInstance()->RenderEnd();
}

void CMainApp::EndPlay()
{
	CObjectManager::GetInstance()->EndPlay();
}

void CMainApp::Release()
{
	// [ Engine ]
	CImguiManager::GetInstance()->Release();
	CRenderManager::GetInstance()->Release();
	CCollisionManager::GetInstance()->Release();
	CObjectManager::GetInstance()->Release();
	CAssimpManager::GetInstance()->Release();
	CResourceManager::GetInstance()->Release();
	CCameraManager::GetInstance()->Release();
	CLightManager::GetInstance()->Release();
	CPrefabManager::GetInstance()->Release();
	CSpawnManager::GetInstance()->Release();
	CNavigationManager::GetInstance()->Release();
	CRenderTargetManager::GetInstance()->Release();

	// [ System ]
	CDeviceManager::GetInstance()->Release();
	CInputManager::GetInstance()->Release();
	CTimerManager::GetInstance()->Release();
}

HRESULT CMainApp::OpenLevel()
{
	// 열고자 하는 레벨의 이름이 Loading인 경우 FAIL 반환
	// if (LEVEL_LOADING == _eStartLevel)
		// return E_FAIL;

	// 반드시 로딩레벨을 거쳐서 레벨을 로드 하게끔 한다. 
	// 레이어 개수만큼 레이어를 확보해서 레벨을 생성한다.
	shared_ptr<CLevel_Editor> spTool = CLevel_Editor::Create((_uint)LAYER::LAYER_END);
	CObjectManager::GetInstance()->OpenLevel(spTool);

	return S_OK;
}
