#include "pch.h"
#include "Loader.h"

#include "ResourceManager.h"
#include "ObjectManager.h"

#include "Level.h"

#include "Level_Logo.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"

typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY wThreadMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader* pLoader = static_cast<CLoader*>(pArg);

	pLoader->Loading_Selector();

	return 0;
}

HRESULT CLoader::Initialize(LEVEL _eNextLevelIndex)
{
	m_eNextLevelIndex = _eNextLevelIndex;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, wThreadMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_Selector()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr = {};

	if (LEVEL_LOGO == m_eNextLevelIndex)
	{
		hr = Loading_For_Logo();
	}
	else if (LEVEL_STAGE1 == m_eNextLevelIndex)
	{
		hr = Loading_For_Stage1();
	}
	else if (LEVEL_STAGE2 == m_eNextLevelIndex)
	{
		hr = Loading_For_Stage2();
	}
	else if (LEVEL_STAGE3 == m_eNextLevelIndex)
	{
		hr = Loading_For_Stage3();
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

#ifdef _DEBUG
void CLoader::CreateLevel()
{
	if (true == m_isFinished)
	{
		/* 진짜 진입하고 싶었던 레벨로 들어간다. */
		shared_ptr<CLevel> spLevel = nullptr;

		if (LEVEL_LOGO == m_eNextLevelIndex)
		{
			spLevel = CLevel_Logo::Create((_uint)LAYER::LAYER_END);
		}
		else if (LEVEL_STAGE1 == m_eNextLevelIndex)
		{
			spLevel = CLevel_Stage1::Create((_uint)LAYER::LAYER_END);
		}
		else if (LEVEL_STAGE2 == m_eNextLevelIndex)
		{
			spLevel = CLevel_Stage2::Create((_uint)LAYER::LAYER_END);
		}
		else if (LEVEL_STAGE3 == m_eNextLevelIndex)
		{
			spLevel = CLevel_Stage3::Create((_uint)LAYER::LAYER_END);
		}

		if (nullptr == spLevel)
			return;

		if (FAILED(CObjectManager::GetInstance()->OpenLevel(spLevel)))
			return;

		return;
	}
}
void CLoader::Output_LoadingText()
{
	wstring wstrCurrentFilePath = L"";

	if (false == m_isFinished)
		wstrCurrentFilePath = CResourceManager::GetInstance()->GetCurrentResourcePath();
	else
		wstrCurrentFilePath = L"로딩이 완료되었습니다.";

	SetWindowText(g_hWnd, wstrCurrentFilePath.c_str());
}
#endif

HRESULT CLoader::Loading_For_Logo()
{
	CResourceManager::GetInstance()->ClearCurrentScene();

	CResourceManager::GetInstance()->ReadResourceFilePaths(L"Level_Logo", RESOURCE_DIFFUSETEXTURE);
	// ReadResourceFilePaths(L"Level_Logo", RESOURCE_SOUND);

	CResourceManager::GetInstance()->CreateTextureFromFile(L"Level_Logo");

	wstring wstrCreateModel = L"모델을 생성중입니다.";
	SetWindowText(g_hWnd, wstrCreateModel.c_str());

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage1()
{
 	CResourceManager::GetInstance()->ClearCurrentScene();
	
	CResourceManager::GetInstance()->ReadResourceFilePaths(L"Level_Stage1", RESOURCE_DIFFUSETEXTURE);
	// ReadResourceFilePaths(L"Level_Stage1", RESOURCE_SOUND);

 	CResourceManager::GetInstance()->CreateTextureFromFile(L"Level_Stage1");

	wstring wstrCreateModel = L"모델을 생성중입니다.";
	SetWindowText(g_hWnd, wstrCreateModel.c_str());

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage2()
{
	CResourceManager::GetInstance()->ClearCurrentScene();

	CResourceManager::GetInstance()->ReadResourceFilePaths(L"Level_Stage2", RESOURCE_DIFFUSETEXTURE);
	// ReadResourceFilePaths(L"Level_Stage1", RESOURCE_SOUND);

	CResourceManager::GetInstance()->CreateTextureFromFile(L"Level_Stage2");

	wstring wstrCreateModel = L"모델을 생성중입니다.";
	SetWindowText(g_hWnd, wstrCreateModel.c_str());

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Stage3()
{
	CResourceManager::GetInstance()->ClearCurrentScene();

	CResourceManager::GetInstance()->ReadResourceFilePaths(L"Level_Stage3", RESOURCE_DIFFUSETEXTURE);
	// ReadResourceFilePaths(L"Level_Stage1", RESOURCE_SOUND);


	CResourceManager::GetInstance()->CreateTextureFromFile(L"Level_Stage3");

	wstring wstrCreateModel = L"모델을 생성중입니다.";
	SetWindowText(g_hWnd, wstrCreateModel.c_str());

	m_isFinished = true;

	return S_OK;
}

void CLoader::Release()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);
}