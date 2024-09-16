#include "pch.h"
#include "ClientLevel.h"

#include "ObjectManager.h"
#include "NavigationManager.h"
#include "Navigation.h"
#include "ResourceManager.h"
#include "SpawnManager.h"
#include "PrefabManager.h"
#include "ThreadManager.h"

#include "Layer.h"

#include "AnimActor.h"
#include "NonAnimActor.h"

#include "Slime_Ice.h"
#include "Slime_Fire.h"
#include "Hili_Ice.h"
#include "Hili_Fire.h"
#include "Defender.h"

void CClientLevel::Initialize(_uint _iNumLayers)
{
	CLevel::Initialize(_iNumLayers);
}

_int CClientLevel::PreUpdate(const _float& _fTimeDelta)
{
	return CLevel::PreUpdate(_fTimeDelta);
}

_int CClientLevel::Update(const _float& _fTimeDelta)
{
	return CLevel::Update(_fTimeDelta);
}

_int CClientLevel::LateUpdate(const _float& _fTimeDelta)
{
	return CLevel::LateUpdate(_fTimeDelta);
}

void CClientLevel::Render()
{
	CLevel::Render();
}

void CClientLevel::EndPlay()
{
	CLevel::EndPlay();
}

void CClientLevel::Release()
{
	CLevel::Release();
}

void CClientLevel::LoadSpawn(const wstring& _wstrLevelName)
{
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	const wstring& wstrSpawnDataFile = wstrDataPath + _wstrLevelName + L"_Spawn" + L".txt";

	// 프리팹 데이터 파일을 읽고 저장합니다.
	ifstream istrDataFile(wstrSpawnDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open SpawnDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())
	{
		// 예외 처리
		if (istrDataFile.eof())
			break;

		SPAWN_DATA Spawn = {};

		istrDataFile.read((char*)(&Spawn), sizeof(SPAWN_DATA));

		CSpawnManager::GetInstance()->AddSpawnData(Spawn.tPrefab.szActorName, Spawn);

		wstring wstrFileName = L"";
		wstring wstrActorName = L"";

		for (_uint i = 0; i < MAX_PATH; ++i)
		{
			if (Spawn.tPrefab.szModelFileName[i] == '\0') { break; }
			wstrFileName += Spawn.tPrefab.szModelFileName[i];
		}
		for (_uint i = 0; i < MAX_PATH; ++i)
		{
			if (Spawn.tPrefab.szActorName[i] == '\0') { break; }
			wstrActorName += Spawn.tPrefab.szActorName[i];
		}

		MODEL_DATA Data = *CPrefabManager::GetInstance()->GetModelData(wstrFileName);

		shared_ptr<CLayer> spLayer = m_vecLayers[Spawn.tPrefab.iLayerIndex];
		shared_ptr<CActor> spActor = nullptr;

		switch (Spawn.tPrefab.iLayerIndex)
		{
			// 0. 플레이어는 클라이언트에서 직접 생성한다.
		case LAYER::LAYER_PLAYER:
		{
			break;
		}
			// 1. 몬스터
		case LAYER::LAYER_MONSTER:
		{
			switch (Spawn.tPrefab.eMonsterVariation)
			{
			case MONSTER_VARIATION::SLIME_ICE:
			{
				wstring wstrVariation = L"Slime_Ice";

				spActor = spLayer->CreateActorToPool<CSlime_Ice>(wstrActorName);
				Spawn.tPrefab.vTranslate = _float3(10000.f, 10000.f, 10000.f);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetSpawnData(Spawn);
				break;
			}
			case MONSTER_VARIATION::SLIME_FIRE:
			{
				wstring wstrVariation = L"Slime_Fire";

				spActor = spLayer->CreateActorToPool<CSlime_Fire>(wstrActorName);
				Spawn.tPrefab.vTranslate = _float3(10000.f, 10000.f, 10000.f);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetSpawnData(Spawn);
				break;
			}
			case MONSTER_VARIATION::HILI_ICE:
			{
				wstring wstrVariation = L"Hili_Ice";

				spActor = spLayer->CreateActorToPool<CHili_Ice>(wstrActorName);
				Spawn.tPrefab.vTranslate = _float3(10000.f, 10000.f, 10000.f);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetSpawnData(Spawn);
				break;
			}
			case MONSTER_VARIATION::HILI_FIRE:
			{
				wstring wstrVariation = L"Hili_Fire";

				spActor = spLayer->CreateActorToPool<CHili_Fire>(wstrActorName);
				Spawn.tPrefab.vTranslate = _float3(10000.f, 10000.f, 10000.f);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetSpawnData(Spawn);
				break;
			}
			case MONSTER_VARIATION::DEFENDER:
			{
				wstring wstrVariation = L"Defender";

				spActor = spLayer->CreateActorToPool<CDefender>(wstrActorName);
				Spawn.tPrefab.vTranslate = _float3(10000.f, 10000.f, 10000.f);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(spActor)->SetSpawnData(Spawn);
				break;
			}
			default:
				break;
			}
			break;
		}
			// 2. 그 외
		case LAYER::LAYER_TERRAIN:
		case LAYER::LAYER_OBJECT:
		{
			spLayer->CreateActor<CNonAnimActor>(wstrActorName);
			dynamic_pointer_cast<CNonAnimActor>(spLayer->FindActor(wstrActorName))->SetModelData(Data);
			dynamic_pointer_cast<CNonAnimActor>(spLayer->FindActor(wstrActorName))->SetSpawnData(Spawn);
			break;
		}
		/*case LAYER::LAYER_PARTICLE:
		case LAYER::LAYER_EFFECTMESH:
		case LAYER::LAYER_NAVIGATION:*/
		}
	}

	vector<pair<string, SPAWN_DATA>>& temp = CSpawnManager::GetInstance()->GetSpawnData();
	int a = 0;
}

void CClientLevel::InitializeNavigationIndex()
{
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	for (auto& Layer : m_vecLayers)
	{
		for (auto& Pair : Layer->GetActors())
		{
			Pair.second->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(Pair.second->GetActorTranslation()));
		}
		for (auto& Pair : Layer->GetActors_Pool())
		{
			for (auto& spActor : Pair.second)
			{
				spActor->SetCurNaviCellIndex(spNavi->InitializeActorCellIndex(spActor->GetActorTranslation()));
			}
		}
	}
}
