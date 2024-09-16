#include "pch.h"
#include "MainApp.h"

#include "DeviceManager.h"
#include "InputManager.h"
#include "TimerManager.h"

#include "ResourceManager.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "LightManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "CollisionManager.h"
#include "FontManager.h"
#include "NavigationManager.h"
#include "RenderTargetManager.h"
#include "SoundManager.h"
#include "ThreadManager.h"

#include "Level_Loading.h"
#include "Level_Logo.h"
#include "Layer.h"

HRESULT CMainApp::PreInitialize(HINSTANCE _hInst, HWND _hWnd)
{
	// [ System ]
	SUCCEEDED(CDeviceManager::GetInstance()->Initialize(_hWnd, Client::g_iWinSizeX, Client::g_iWinSizeY));
	SUCCEEDED(CInputManager::GetInstance()->Initialize(_hInst, _hWnd));

	// D3D11_RASTERIZER_DESC;
	// D3D11_DEPTH_STENCIL_DESC;
	// D3D11_RENDER_TARGET_BLEND_DESC;

	return S_OK;
}

HRESULT CMainApp::Initialize()
{
	// [ Engine ]
	SUCCEEDED(CResourceManager::GetInstance()->Initialize(PROJECT_CLIENT));
	SUCCEEDED(CRenderTargetManager::GetInstance()->Initialize());
	SUCCEEDED(CRenderManager::GetInstance()->Initialize());
	SUCCEEDED(CCollisionManager::GetInstance()->Initialize());
	SUCCEEDED(CCameraManager::GetInstance()->Initialize());
	SUCCEEDED(CObjectManager::GetInstance()->Initialize(Client::LEVEL_END, LAYER_END));
	SUCCEEDED(CLightManager::GetInstance()->Initialize());
	SUCCEEDED(CPrefabManager::GetInstance()->Initialize());
	SUCCEEDED(CSpawnManager::GetInstance()->Initialize());
	SUCCEEDED(CCollisionManager::GetInstance()->Initialize());
	SUCCEEDED(CFontManager::GetInstance()->Initialize());
	SUCCEEDED(CNavigationManager::GetInstance()->Initialize());
	SUCCEEDED(CSoundManager::GetInstance()->Initialize());
	SUCCEEDED(CThreadManager::GetInstance()->Initialize());

	if (FAILED(OpenLevel(LEVEL_LOGO)))
		return E_FAIL;

#pragma region 가라
	
	// 폰트
	/* MakeSpriteFont "나눔스퀘어 Bold" /FontSize:10 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 UI_Default_Bold.spritefont */
	if (FAILED(CFontManager::GetInstance()->AddFont(TEXT("GenshinFont"), TEXT("../../Resources/Level_Static/Fonts/Genshin.spritefont"))))
		return E_FAIL;
#pragma endregion 가라

	return S_OK;
}

_int CMainApp::PreUpdate(const _float& _fTimeDelta)
{
	CInputManager::GetInstance()->Update();

	CObjectManager::GetInstance()->PreUpdate(_fTimeDelta);

	return S_OK;
}

_int CMainApp::Update(const _float& _fTimeDelta)
{
	CObjectManager::GetInstance()->Update(_fTimeDelta);

	return S_OK;
}

_int CMainApp::LateUpdate(const _float& _fTimeDelta)
{
	CObjectManager::GetInstance()->LateUpdate(_fTimeDelta);

	CCollisionManager::GetInstance()->Update(_fTimeDelta);

	return S_OK;
}

void CMainApp::Render()
{
	// 렌더 시작
	CDeviceManager::GetInstance()->RenderBegin(_float4(0.f, 0.f, 1.f, 1.f));

	// [ Render ] -> 여기서 렌더링 하면 됨
	CRenderManager::GetInstance()->Render();
	CObjectManager::GetInstance()->Render();

//#ifdef _DEBUG
//	if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"), TEXT("버거형이 지켜보는 중 -ㅅ-"), _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f))))
//		return;
//#endif // _DEBUG

	CObjectManager::GetInstance()->GetCurLevel()->Render();

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
	CRenderManager::GetInstance()->Release();
	CObjectManager::GetInstance()->Release();
	CResourceManager::GetInstance()->Release();
	CCameraManager::GetInstance()->Release();
	CCollisionManager::GetInstance()->Release();
	CLightManager::GetInstance()->Release();
	CPrefabManager::GetInstance()->Release();
	CSpawnManager::GetInstance()->Release();
	CFontManager::GetInstance()->Release();
	CNavigationManager::GetInstance()->Release();
	CRenderTargetManager::GetInstance()->Release();
	CSoundManager::GetInstance()->Release();
	CThreadManager::GetInstance()->Release();

	// [ System ]
	CDeviceManager::GetInstance()->Release();
	CInputManager::GetInstance()->Release();
	CTimerManager::GetInstance()->Release();
}

HRESULT CMainApp::OpenLevel(LEVEL _eStartLevel)
{
	// 열고자 하는 레벨의 이름이 Loading인 경우 FAIL 반환
	if (LEVEL_LOADING == _eStartLevel)
		return E_FAIL;

	// 반드시 로딩레벨을 거쳐서 레벨을 로드 하게끔 한다. 
	// 레이어 개수만큼 레이어를 확보해서 레벨을 생성한다.
	shared_ptr<CLevel_Loading> spLoading = CLevel_Loading::Create(_eStartLevel, (_uint)LAYER::LAYER_END);
	CObjectManager::GetInstance()->OpenLevel(spLoading);

	return S_OK;
}
