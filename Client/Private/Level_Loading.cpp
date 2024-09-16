#include "pch.h"
#include "Level_Loading.h"
#include "Loader.h"

#include "ObjectManager.h"
#include "InputManager.h"

#include "Level_Logo.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"
#include "Layer.h"

#include "BackGround_Loading1.h"
#include "BackGround_Loading2.h"
#include "BackGround_Loading3.h"
#include "Logo_LoadingText2.h"

_uint CLevel_Loading::g_iLoadingCount = 0;

void CLevel_Loading::Initialize(_uint _iNumLayers)
{
	// 로더 객체 생성
	m_spLoader = make_shared<CLoader>();
	if (FAILED(m_spLoader->Initialize(m_eNextLevelIndex)))
	{
		MSG_BOX("Failed to Initialize Loader at CLevel_Loading");
		return;
	}

	if (LEVEL_STAGE1 == m_eNextLevelIndex)
	{
		m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CBackGround_Loading2>(L"Bg2");
		m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CLogo_LoadingText2>(L"Logo_LoadingText1");
	}
	else if (LEVEL_STAGE2 == m_eNextLevelIndex)
	{
		m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CBackGround_Loading1>(L"Bg1");
		// m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CLogo_LoadingText2>(L"Logo_LoadingText2");
	}
	else if (LEVEL_STAGE3 == m_eNextLevelIndex)
	{
		m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CBackGround_Loading3>(L"Bg3");
		// m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CLogo_LoadingText2>(L"Logo_LoadingText3");
	}

	CLevel::Initialize(_iNumLayers);
}

_int CLevel_Loading::PreUpdate(const _float& _fTimeDelta)
{
	return CLevel::PreUpdate(_fTimeDelta);
}

_int CLevel_Loading::Update(const _float& _fTimeDelta)
{
	return CLevel::Update(_fTimeDelta);
}

_int CLevel_Loading::LateUpdate(const _float& _fTimeDelta)
{
	if (nullptr == m_spLoader)
		return E_FAIL;

#ifdef _DEBUG
	m_spLoader->Output_LoadingText();
#endif // _DEBUG

	return CLevel::LateUpdate(_fTimeDelta);
}

void CLevel_Loading::Render()
{
	CLevel::Render();
}

void CLevel_Loading::EndPlay()
{
	if (true == m_spLoader->isFinished())
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
		{
			return;
		}

		return;
	}

	// CLevel::EndPlay();
}

void CLevel_Loading::Release()
{
	if(m_spLoader)
		m_spLoader->Release();

	CLevel::Release();
}

HRESULT CLevel_Loading::Ready_Layer_BackGround()
{
	return E_NOTIMPL;
}

HRESULT CLevel_Loading::Ready_Layer_UI()
{
	return E_NOTIMPL;
}
