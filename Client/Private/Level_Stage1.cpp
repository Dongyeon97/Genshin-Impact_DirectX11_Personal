#include "pch.h"
#include "Level_Stage1.h"

#include "ObjectManager.h"
#include "LightManager.h"
#include "NavigationManager.h"
#include "FontManager.h"
#include "InputManager.h"
#include "SoundManager.h"

#include "Level_Loading.h"
#include "Layer.h"
#include "ParticleActor_Rect.h"
#include "ParticleActor_Point.h"
#include "Skybox.h"
#include "SidePortrait.h"
#include "HPbar.h"
#include "Monster.h"

#include "UserInstance.h"
#include "BackGround.h"
#include "Boss_Andreus.h"
#include "Stage1_Navi.h"
#include "Navigation.h"
#include "MonsterStateMachine.h"
// #include "Slime_Ice_StateMachine.h"
// #include "Hili_Fire_StateMachine.h"
// #include "Hili_Ice_StateMachine.h"
#include "AnimModel.h"

#include "MonsterKey_1.h"
#include "MonsterKey_2.h"
#include "MonsterKey_3.h"
#include "MonsterKey_4.h"
#include "DungeonExit.h"
#include "DungeonExitUI.h"

void CLevel_Stage1::Initialize(_uint _iNumLayers)
{
	ReadyLights();

	LoadLevelDataFromFileName(L"Stage1_New_5");

	shared_ptr<CLevel> spStaticLevel = CObjectManager::GetInstance()->GetStaticLevel();

	/* 유저 인스턴스 */
	spStaticLevel->GetLayer(LAYER::LAYER_PLAYER)->CreateActor<CUserInstance>(L"UserInstance")->Initialize();

	/* 환경 */
	m_vecLayers[LAYER::LAYER_NAVIGATION]->CreateActor<CStage1_Navi>(L"Actor_Stage1_Navigation");
	m_vecLayers[LAYER::LAYER_TERRAIN]->CreateActor<CSkybox>(L"Actor_Skybox");

	CClientLevel::Initialize(_iNumLayers);

	/* 보스 */
	// shared_ptr<CActor> spAndreus = m_vecLayers[LAYER::LAYER_MONSTER]->CreateActorToPool<CBoss_Andreus>(L"Actor_Andreus");
	// spAndreus->Initialize();

	/* MonsterKey(소환 트리거) */
	ReadyMonsterKey();

	/* DungeonExit(던전 나가기) */
	ReadyDungeonExit();

	CClientLevel::InitializeNavigationIndex();

	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();
	CObjectManager::GetInstance()->GetCurPlayer()->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(CObjectManager::GetInstance()->GetCurPlayer()->GetActorTranslation()));

	CSoundManager::GetInstance()->PlaySoundEx("LoginSound.ogg", SOUND_TYPE::EFFECT_1, 0.4f);
	CSoundManager::GetInstance()->PlayBGMLoop("Music22_10.ogg", 0.05f);
}

_int CLevel_Stage1::PreUpdate(const _float& _fTimeDelta)
{
	return CClientLevel::PreUpdate(_fTimeDelta);
}

_int CLevel_Stage1::Update(const _float& _fTimeDelta)
{
	if (!CObjectManager::GetInstance()->GetIsLoading() && m_bWave1_Spawned)
		m_fStageLimitedTime -= _fTimeDelta;

	ManagementSpawn();

	return CClientLevel::Update(_fTimeDelta);
}

_int CLevel_Stage1::LateUpdate(const _float& _fTimeDelta)
{
	return CClientLevel::LateUpdate(_fTimeDelta);
}

void CLevel_Stage1::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("Stage1"));
#endif

	//if (!m_bWave1_Spawned)
	//	return;

	/*_int iLeftMonsterCount = CalculateActivatedMonsterNum();

	if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont", L"남은 몬스터 : " + to_wstring(iLeftMonsterCount), _float2(0.f, 200.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, { 0.f,0.f }, 0.8f)))
		return;

	_int iMinute = (int)m_fStageLimitedTime / 60;
	_int iSecond = (int)m_fStageLimitedTime % 60;

	if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont", to_wstring(iMinute) + L" : " + to_wstring(iSecond), _float2(600.f, 10.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, { 0.f,0.f }, 0.6f)))
		return;*/

	_float fCurHp = dynamic_pointer_cast<CCharactor>(CObjectManager::GetInstance()->GetCurPlayer())->GetCurHp();
	_float fMaxHp = dynamic_pointer_cast<CCharactor>(CObjectManager::GetInstance()->GetCurPlayer())->GetMaxHp();

	if (FAILED(CFontManager::GetInstance()->Render(TEXT("GenshinFont"), to_wstring((_uint)fCurHp) + L" / " + to_wstring((_uint)fMaxHp), _float2(585.f, 675.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, { 0.f,0.f }, 0.45f)))
		return;

	CClientLevel::Render();
}

void CLevel_Stage1::EndPlay()
{
	if (CInputManager::GetInstance()->IsKeyDown(DIK_RETURN))
	{
		CSoundManager::GetInstance()->StopAll();

		shared_ptr<CLevel_Loading> spLoading = CLevel_Loading::Create(LEVEL_STAGE2, (_uint)LAYER::LAYER_END);

		CObjectManager::GetInstance()->SetIsLoading(true);

		if (FAILED(CObjectManager::GetInstance()->OpenLevel(spLoading)))
			return;

		return;
	}

	CClientLevel::EndPlay();
}

void CLevel_Stage1::Release()
{
	CClientLevel::Release();
}

HRESULT CLevel_Stage1::ReadyLights()
{
	LIGHT_DESC tLightDesc = {};

	tLightDesc.eLightType = LIGHT_DESC::TYPE_DIRECTIONAL;
	tLightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(0.8f, 0.8f, 0.8f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CLightManager::GetInstance()->AddLight(tLightDesc)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_Stage1::ReadyMonsterKey()
{
	// MonsterKey_1
	shared_ptr<CMonsterKey_1> spMonsterKey_1 = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CMonsterKey_1>(L"MonsterKey_1");
	if (nullptr == spMonsterKey_1)
	{
		MSG_BOX("몬스터 키 생성에 실패하였습니다. CLevel_State1");
		return E_FAIL;
	}
	spMonsterKey_1->SetJsonFileName(L"MonsterKey_One");
	spMonsterKey_1->SetCurLevelIndex(1);
	spMonsterKey_1->Initialize(spMonsterKey_1->GetJsonFileName());
	spMonsterKey_1->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 500.f, 300.f, 1.f));

	// MonsterKey_2
	shared_ptr<CMonsterKey_2> spMonsterKey_2 = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CMonsterKey_2>(L"MonsterKey_2");
	if (nullptr == spMonsterKey_2)
	{
		MSG_BOX("몬스터 키 생성에 실패하였습니다. CLevel_State1");
		return E_FAIL;
	}
	spMonsterKey_2->SetJsonFileName(L"MonsterKey_Two");
	spMonsterKey_2->Initialize(spMonsterKey_2->GetJsonFileName());
	spMonsterKey_2->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 500.f, 300.f, 1.f));

	// MonsterKey_3
	shared_ptr<CMonsterKey_3> spMonsterKey_3 = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CMonsterKey_3>(L"MonsterKey_3");
	if (nullptr == spMonsterKey_2)
	{
		MSG_BOX("몬스터 키 생성에 실패하였습니다. CLevel_State1");
		return E_FAIL;
	}
	spMonsterKey_3->SetJsonFileName(L"MonsterKey_Three");
	spMonsterKey_3->Initialize(spMonsterKey_3->GetJsonFileName());
	spMonsterKey_3->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 800.f, 300.f, 1.f));

	// MonsterKey_4
	shared_ptr<CMonsterKey_4> spMonsterKey_4 = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActor<CMonsterKey_4>(L"MonsterKey_4");
	if (nullptr == spMonsterKey_4)
	{
		MSG_BOX("몬스터 키 생성에 실패하였습니다. CLevel_State1");
		return E_FAIL;
	}
	spMonsterKey_4->SetJsonFileName(L"MonsterKey_Four");
	spMonsterKey_4->Initialize(spMonsterKey_4->GetJsonFileName());
	spMonsterKey_4->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 800.0f, 300.f, 1.f));

	return S_OK;
}
HRESULT CLevel_Stage1::ReadyDungeonExit()
{
	// DungeonExit_Seal
	m_wpDungeonExit_Seal = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActorToPool<CDungeonExit>(L"DungeonExit_Seal");
	if (m_wpDungeonExit_Seal.expired())
	{
		MSG_BOX("던전 나가기 오브젝트 생성에 실패하였습니다. CLevel_Stage1");
		return E_FAIL;
	}
	m_wpDungeonExit_Seal.lock()->SetJsonFileName(L"Dungeon_Exit_Seal");
	m_wpDungeonExit_Seal.lock()->SetCurLevelIndex(1);
	m_wpDungeonExit_Seal.lock()->Initialize(m_wpDungeonExit_Seal.lock()->GetJsonFileName());
	m_wpDungeonExit_Seal.lock()->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 0.f, 1300.f, 1.f));

	// DungeonExit_Floor
	m_wpDungeonExit_Floor = m_vecLayers[LAYER::LAYER_OBJECT]->CreateActorToPool<CDungeonExit>(L"DungeonExit_Floor");
	if (m_wpDungeonExit_Floor.expired())
	{
		MSG_BOX("던전 나가기 오브젝트 생성에 실패하였습니다. CLevel_Stage1");
		return E_FAIL;
	}
	m_wpDungeonExit_Floor.lock()->SetJsonFileName(L"Dungeon_Exit_Floor");
	m_wpDungeonExit_Floor.lock()->SetCurLevelIndex(1);
	m_wpDungeonExit_Floor.lock()->Initialize(m_wpDungeonExit_Floor.lock()->GetJsonFileName());
	m_wpDungeonExit_Floor.lock()->SetActorTranslation_NoNavigation(XMVectorSet(0.0f, 0.f, 1300.f, 1.f));

	return S_OK;
}

HRESULT CLevel_Stage1::ManagementSpawn()
{
	// 웨이브의 몬스터가 모두 죽었을 때
	if (0 == CalculateActivatedMonsterNum() && m_bWave1_Spawned)
	{
		if (!m_bWave2_Spawned)
		{
			Stage1_SpawnWave2();
			return S_OK;
		}
		else if (!m_bWave3_Spawned)
		{
			Stage1_SpawnWave3();
			return S_OK;
		}
	}

	if (0 == CalculateActivatedMonsterNum() && m_bWave3_Spawned)
	{
		m_wpDungeonExit_Seal.lock()->SetActorUse(true);
		m_wpDungeonExit_Floor.lock()->SetActorUse(true);
	}

	return E_FAIL;
}

HRESULT CLevel_Stage1::Stage1_SpawnWave1()
{
	wstring wstrVariation1 = L"Slime_Ice";
	wstring wstrVariation2 = L"Slime_Fire";

	shared_ptr<CMonster> spMonster;
	shared_ptr<CStateMachine> spStateMachine;
	shared_ptr<CAnimModel> spAnimModel;

	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	for (_uint i = 0; i < 3; ++i)
	{
		spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation1));
		if (nullptr == spMonster)
			continue;

		/* Translate */
		spMonster->SetActorTranslation_NoNavigation(XMVectorSet(-1250.f, 0.f, 1150.f - (_float)i * 1150.f, 1.f));
		spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

		/* State */
		spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
		if (nullptr == spStateMachine)
			continue;

		spStateMachine->RequestChangeState(MONSTER_STATE::DETECTED);

		/* Anim */
		spAnimModel = spMonster->GetAnimModel();
		spAnimModel->SetAnimationIndex(12);
	}
	for (_uint i = 0; i < 3; ++i)
	{
		spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation2));
		if (nullptr == spMonster)
			continue;

		/* Translate */
		spMonster->SetActorTranslation_NoNavigation(XMVectorSet(1250.f, 0.f, 1150.f - (_float)i * 1150.f, 1.f));
		spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

		/* State */
		spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
		if (nullptr == spStateMachine)
			continue;

		spStateMachine->RequestChangeState(MONSTER_STATE::DETECTED);

		/* Anim */
		spAnimModel = spMonster->GetAnimModel();
		spAnimModel->SetAnimationIndex(12);
	}

	// 가운데 전방 얼음 슬라임1
	spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation1));
	if (nullptr == spMonster)
		return E_FAIL;

	/* Translate */
	spMonster->SetActorTranslation_NoNavigation(XMVectorSet(0.f, 0.f, 1350.f, 1.f));
	spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

	/* State */
	spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
	if (nullptr == spStateMachine)
		return E_FAIL;

	spStateMachine->RequestChangeState(MONSTER_STATE::DETECTED);

	/* Anim */
	spAnimModel = spMonster->GetAnimModel();
	spAnimModel->SetAnimationIndex(12);



	// 가운데 후방 화염 슬라임1
	spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation2));
	if (nullptr == spMonster)
		return E_FAIL;

	/* Translate */
	spMonster->SetActorTranslation_NoNavigation(XMVectorSet(0.f, 0.f, -1350.f, 1.f));
	spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

	/* State */
	spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
	if (nullptr == spStateMachine)
		return E_FAIL;

	spStateMachine->RequestChangeState(MONSTER_STATE::DETECTED);

	/* Anim */
	spAnimModel = spMonster->GetAnimModel();
	spAnimModel->SetAnimationIndex(12);

	m_bWave1_Spawned = true;

	return S_OK;
}
HRESULT CLevel_Stage1::Stage1_SpawnWave2()
{
	wstring wstrVariation1 = L"Hili_Ice";
	wstring wstrVariation2 = L"Hili_Fire";

	shared_ptr<CMonster> spMonster;
	shared_ptr<CStateMachine> spStateMachine;
	shared_ptr<CAnimModel> spAnimModel;

	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	for (_uint i = 0; i < 3; ++i)
	{
		spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation1));
		if (nullptr == spMonster)
			continue;

		/* Translate */
		spMonster->SetActorTranslation_NoNavigation(XMVectorSet(-1250.f, 0.f, 1150.f - (_float)i * 1150.f, 1.f));
		spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

		/* State */
		spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
		if (nullptr == spStateMachine)
			continue;

		spStateMachine->RequestChangeState(MONSTER_STATE::TRACE);

		/* Anim */
		spAnimModel = spMonster->GetAnimModel();
		spAnimModel->SetAnimationIndex(1);
	}
	for (_uint i = 0; i < 3; ++i)
	{
		spMonster = dynamic_pointer_cast<CMonster>(CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation2));
		if (nullptr == spMonster)
			continue;

		/* Translate */
		spMonster->SetActorTranslation_NoNavigation(XMVectorSet(1250.f, 0.f, 1150.f - (_float)i * 1150.f, 1.f));
		spMonster->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spMonster->GetActorTranslation()));

		/* State */
		spStateMachine = dynamic_pointer_cast<CMonsterStateMachine>(spMonster->GetStateMachine());
		if (nullptr == spStateMachine)
			continue;

		spStateMachine->RequestChangeState(MONSTER_STATE::TRACE);

		/* Anim */
		spAnimModel = spMonster->GetAnimModel();
		spAnimModel->SetAnimationIndex(1);
	}

	m_bWave2_Spawned = true;

	return S_OK;
}
HRESULT CLevel_Stage1::Stage1_SpawnWave3()
{
	wstring wstrVariation = L"Defender";

	shared_ptr<CActor> spActor;
	shared_ptr<CStateMachine> spStateMachine;
	shared_ptr<CAnimModel> spAnimModel;

	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER_MONSTER)->RecycleFromPool(wstrVariation);
	if (nullptr == spActor)
		return E_FAIL;

	spActor->SetActorTranslation_NoNavigation(XMVectorSet(0.f, 0.f, 200.f, 1.f));
	spActor->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spActor->GetActorTranslation()));

	m_bWave3_Spawned = true;

	return S_OK;
}

_uint CLevel_Stage1::CalculateActivatedMonsterNum()
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
