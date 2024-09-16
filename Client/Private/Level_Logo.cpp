#include "pch.h"
#include "Level_Logo.h"

#include "RenderManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "SoundManager.h"

#include "Level_Loading.h"

#include "Layer.h"
#include "BackGround.h"
#include "Logo_LoadingText.h"
#include "SidePortrait.h"

void CLevel_Logo::Initialize(_uint _iNumLayers)
{
	m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CBackGround>(L"Bg");
	m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CLogo_LoadingText>(L"Logo_LoadingText");

	CSoundManager::GetInstance()->PlayBGMLoop("Logo_BGM.ogg", 0.4f);

	CLevel::Initialize(_iNumLayers);
}

_int CLevel_Logo::PreUpdate(const float& _fTimeDelta)
{
	return CLevel::PreUpdate(_fTimeDelta);
}

_int CLevel_Logo::Update(const float& _fTimeDelta)
{
	return CLevel::Update(_fTimeDelta);
}

_int CLevel_Logo::LateUpdate(const float& _fTimeDelta)
{
	return CLevel::LateUpdate(_fTimeDelta);
}

void CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	CLevel::Render();
}

void CLevel_Logo::EndPlay()
{
	if (CInputManager::GetInstance()->IsKeyDown(DIK_RETURN))
	{
		CSoundManager::GetInstance()->StopAll();

		shared_ptr<CLevel_Loading> spLoading = CLevel_Loading::Create(LEVEL_STAGE1, (_uint)LAYER::LAYER_END);

		if (FAILED(CObjectManager::GetInstance()->OpenLevel(spLoading)))
			return;

		return;
	}

	CLevel::EndPlay();
}

void CLevel_Logo::Release()
{

	CLevel::Release();
}
