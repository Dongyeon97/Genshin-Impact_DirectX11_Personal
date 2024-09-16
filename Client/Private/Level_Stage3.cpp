#include "pch.h"
#include "Level_Stage3.h"

#include "ObjectManager.h"
#include "LightManager.h"
#include "NavigationManager.h"
#include "FontManager.h"
#include "InputManager.h"
#include "SpawnManager.h"
#include "SoundManager.h"

#include "Level_Loading.h"
#include "Layer.h"

#include "Boss_Andreus.h"
#include "Stage3_Navi.h"
#include "Skybox2.h"
#include "Player.h"
#include "DungeonExit.h"

#include "Navigation.h"

void CLevel_Stage3::Initialize(_uint _iNumLayers)
{
	ReadyLights();

	LoadLevelDataFromFileName(L"Stage4_New");

	m_vecLayers[LAYER::LAYER_NAVIGATION]->CreateActor<CStage3_Navi>(L"Actor_Terrain");
	m_vecLayers[LAYER::LAYER_TERRAIN]->CreateActor<CSkybox2>(L"Actor_Skybox");
	// m_vecLayers[LAYER::LAYER_MONSTER]->CreateActor<CBoss_Andreus>(L"Actor_Andreus");

	CClientLevel::Initialize(_iNumLayers);

	/* 보스 */
	shared_ptr<CActor> spAndreus = m_vecLayers[LAYER::LAYER_MONSTER]->CreateActorToPool<CBoss_Andreus>(L"Actor_Andreus");
	spAndreus->Initialize();

	/* 던전 나가기 */
	ReadyDungeonExit();

	CClientLevel::InitializeNavigationIndex();

	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	CObjectManager::GetInstance()->GetCurPlayer()->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	CObjectManager::GetInstance()->GetCurPlayer()->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(CObjectManager::GetInstance()->GetCurPlayer()->GetActorTranslation()));

	CObjectManager::GetInstance()->SetIsLoading(false);

	CSoundManager::GetInstance()->PlayBGMLoop("Music24_2.ogg", 0.15f);
}

_int CLevel_Stage3::PreUpdate(const _float& _fTimeDelta)
{
	return CClientLevel::PreUpdate(_fTimeDelta);
}

_int CLevel_Stage3::Update(const _float& _fTimeDelta)
{
	if(!CObjectManager::GetInstance()->GetIsLoading())
		m_fStageLimitedTime -= _fTimeDelta;

	if (m_fStageLimitedTime < 465.f && !m_bTemp)
	{
		m_vecLayers[LAYER::LAYER_MONSTER]->RecycleFromPool(L"Actor_Andreus");
		m_bTemp = true;
	}

	if (0 == CalculateActivatedMonsterNum())
	{
		m_wpDungeonExit_Seal.lock()->SetActorUse(true);
		m_wpDungeonExit_Floor.lock()->SetActorUse(true);
	}

	return CClientLevel::Update(_fTimeDelta);
}

_int CLevel_Stage3::LateUpdate(const _float& _fTimeDelta)
{
	return CClientLevel::LateUpdate(_fTimeDelta);
}

void CLevel_Stage3::Render()
{
	_float fCurHp = dynamic_pointer_cast<CCharactor>(CObjectManager::GetInstance()->GetCurPlayer())->GetCurHp();
	_float fMaxHp = dynamic_pointer_cast<CCharactor>(CObjectManager::GetInstance()->GetCurPlayer())->GetMaxHp();

	if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"), to_wstring((_uint)fCurHp) + L" / " + to_wstring((_uint)fMaxHp), _float2(585.f, 675.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, { 0.f,0.f }, 0.45f)))
		return;

	_uint iLeftMonsterCount = CalculateActivatedMonsterNum();

	if (0 == iLeftMonsterCount)
		return;

	if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"),
		L"안드리우스",
		{ Client::g_iWinSizeX * 0.5f - 30.f, Client::g_iWinSizeY * 0.02f },
		XMVectorSet(1.f, 1.f, 1.f, 0.8f),
		0.f,
		{ 0.f,0.f },
		0.5f)))
		return;

	if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"),
		L"울프의 영주",
		{ Client::g_iWinSizeX * 0.5f - 19.f, Client::g_iWinSizeY * 0.048f },
		XMVectorSet(0.45f, 0.45f, 0.45f, 0.45f),
		0.f,
		{ 0.f,0.f },
		0.35f)))
		return;

	CClientLevel::Render();
}

void CLevel_Stage3::EndPlay()
{
	CClientLevel::EndPlay();
}

void CLevel_Stage3::Release()
{
	CClientLevel::Release();
}

HRESULT CLevel_Stage3::ReadyLights()
{
	LIGHT_DESC tLightDesc = {};

	tLightDesc.eLightType = LIGHT_DESC::TYPE_DIRECTIONAL;
	tLightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(0.9f, 0.9f, 0.9f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CLightManager::GetInstance()->AddLight(tLightDesc)))
		return E_FAIL;

	//tLightDesc.eLightType = LIGHT_DESC::TYPE_POINT;
	//tLightDesc.vLightPos = _float4(0.f, 10.f, 50.f, 1.f);
	//tLightDesc.fRange = 500.f;
	//tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//tLightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
	//tLightDesc.vSpecular = tLightDesc.vDiffuse;

	//if (FAILED(CLightManager::GetInstance()->AddLight(tLightDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Stage3::ReadyDungeonExit()
{
	// DungeonExit_Seal
	m_wpDungeonExit_Seal = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActorToPool<CDungeonExit>(L"DungeonExit_Seal");
	if (m_wpDungeonExit_Seal.expired())
	{
		MSG_BOX("던전 나가기 오브젝트 생성에 실패하였습니다. CLevel_Stage3");
		return E_FAIL;
	}
	m_wpDungeonExit_Seal.lock()->SetJsonFileName(L"Dungeon_Exit_Seal_Stage3");
	m_wpDungeonExit_Seal.lock()->SetCurLevelIndex(3);
	m_wpDungeonExit_Seal.lock()->Initialize(m_wpDungeonExit_Seal.lock()->GetJsonFileName());
	m_wpDungeonExit_Seal.lock()->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 0.f, 1300.f, 1.f));

	// DungeonExit_Floor
	m_wpDungeonExit_Floor = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActorToPool<CDungeonExit>(L"DungeonExit_Floor");
	if (m_wpDungeonExit_Floor.expired())
	{
		MSG_BOX("던전 나가기 오브젝트 생성에 실패하였습니다. CLevel_Stage3");
		return E_FAIL;
	}
	m_wpDungeonExit_Floor.lock()->SetJsonFileName(L"Dungeon_Exit_Floor_Stage3");
	m_wpDungeonExit_Floor.lock()->SetCurLevelIndex(3);
	m_wpDungeonExit_Floor.lock()->Initialize(m_wpDungeonExit_Floor.lock()->GetJsonFileName());
	m_wpDungeonExit_Floor.lock()->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 0.f, 1300.f, 1.f));

	return S_OK;
}

_uint CLevel_Stage3::CalculateActivatedMonsterNum()
{
	shared_ptr<CLayer> spMonsterLayer = GetLayer(LAYER_MONSTER);

	_uint iMonsterCount = 0;

	//for (auto& Monster : spMonsterLayer->GetActors())
	//{
	//	if (Monster.second->GetActorUse())
	//		++iMonsterCount;
	//}
	for (auto& MonsterVec : spMonsterLayer->GetActors_Pool())
	{
		for (auto& Monster : MonsterVec.second)
		{
			if (Monster->GetActorUse())
				++iMonsterCount;
		}
	}

	return iMonsterCount;
}
