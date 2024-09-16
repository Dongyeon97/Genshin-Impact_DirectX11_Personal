#include "Engine_pch.h"
#include "Level.h"

#include "DeviceManager.h"
#include "ResourceManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "ObjectManager.h"

#include "Layer.h"
#include "Actor.h"
#include "NonAnimActor.h"
#include "AnimActor.h"

void CLevel::Initialize(_uint _iNumLayers)
{
	// 모든 레벨은 Initialize 단계에서 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();
	
	// 레벨의 레이어를 정해진 개수만큼 준비합니다.
	// ReserveLayers(_iNumLayers);

	// 레이어를 초기화합니다.
	for (auto& layer : m_vecLayers)
	{
		layer->Initialize();
	}
}

_int CLevel::PreUpdate(const _float& _fTimeDelta)
{
	for (auto& layer : m_vecLayers)
	{
		_int iResult = layer->PreUpdate(_fTimeDelta);
		if (iResult != S_OK) { return iResult; }
	}
	return S_OK;
}

_int CLevel::Update(const _float& _fTimeDelta)
{
	for (auto& layer : m_vecLayers)
	{
		_int iResult = layer->Update(_fTimeDelta);
		if (iResult != S_OK) { return iResult; }
	}
	return S_OK;
}

_int CLevel::LateUpdate(const _float& _fTimeDelta)
{
	for (auto& layer : m_vecLayers)
	{
		_int iResult = layer->LateUpdate(_fTimeDelta);
		if (iResult != S_OK) { return iResult; }
	}
	return S_OK;
}

void CLevel::Render()
{
	/*for (auto& layer : m_vecLayers)
	{
		layer->Render();
	}*/
}

void CLevel::EndPlay()
{
	for (auto& layer : m_vecLayers)
	{
		layer->EndPlay();
	}
}

void CLevel::Release()
{
	for (auto& layer : m_vecLayers)
	{
		layer->Release();
		layer.reset();
	}
	m_vecLayers.clear();
}

HRESULT CLevel::ReserveLayers(_uint _iNumLayers)
{
	// 레이어의 벡터가 이미 초기화 되어있을 경우, E_FAIL을 반환합니다.
	if (!m_vecLayers.empty())
		return E_FAIL;
	
	// 지정된 레이어 개수만큼 공간을 확보합니다.
	m_vecLayers.reserve(_iNumLayers);

	// 지정된 레이어 개수만큼 레이어를 생성합니다.
	for (_uint i = 0; i < _iNumLayers; i++)
		CreateLayer(i);

	m_iNumLayers = _iNumLayers;

	return S_OK;
}

shared_ptr<CLayer> CLevel::CreateLayer(_uint _iLayerIndex)
{
	shared_ptr<CLayer> spLayer = make_shared<CLayer>();
	spLayer->SetLevel(shared_from_this());
	spLayer->SetLayerIndex(_iLayerIndex);

	m_vecLayers.push_back(spLayer);
	return spLayer;
}

shared_ptr<CLayer> CLevel::FindLayer(_uint _iLayerIndex)
{
	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return nullptr;

	return m_vecLayers[_iLayerIndex];
}

HRESULT CLevel::LoadLevelDataFromFileName(const wstring& _wstrLevelName)
{
	LoadPrefab(_wstrLevelName);
	LoadSpawn(_wstrLevelName);

	return S_OK;
}

void CLevel::LoadPrefab(const wstring& _wstrLevelName)
{
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	const wstring& wstrModelDataFile = wstrDataPath + _wstrLevelName + L"_Prefab" + L".txt";

	// 프리팹 데이터 파일을 읽고 저장합니다.
	ifstream istrDataFile(wstrModelDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open PrefabDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())

	{
		// 1. 새로운 모델 데이터를 만들고, 거기에 데이터를 기록한다.
		MODEL_DATA Data = {};

		// 예외 처리
		if (istrDataFile.eof())
			break;

		// 2. [파일 경로]와 [프리팹]은 고정된 크기를 가지기 때문에, 구조체 통째로 불러온다.
		istrDataFile.read((char*)(&Data.szModelFilePath), MAX_PATH);
		istrDataFile.read((char*)(&Data.szModelFileName), MAX_PATH);
		istrDataFile.read((char*)(&Data.eModelType), sizeof(MODEL_TYPE));

		// 3. [AI_Scene]은 가변적인 데이터가 포함되어 있기 때문에, 모델마다 가변적으로 불러온다.
		MY_AISCENE& AIScene = Data.tAIScenes;

		// == 3.1 애니메이션 ==========================================================================
		istrDataFile.read((char*)(&AIScene.iNumAnimations), sizeof(unsigned int));

		// 애니메이션 벡터에 데이터를 기록하기 위해 resize
		AIScene.vecAnimations.resize(AIScene.iNumAnimations);
		for (_uint i = 0; i < AIScene.iNumAnimations; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].szName), MAX_PATH);

			istrDataFile.read((char*)(&AIScene.vecAnimations[i].Duration), sizeof(double));
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].TickPerSecond), sizeof(double));
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].iNumChannels), sizeof(unsigned int));

			AIScene.vecAnimations[i].vecChannels.resize(AIScene.vecAnimations[i].iNumChannels);
			for (_uint j = 0; j < AIScene.vecAnimations[i].iNumChannels; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].szName), MAX_PATH);

				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys), sizeof(unsigned int));
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys), sizeof(unsigned int));
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys), sizeof(unsigned int));

				AIScene.vecAnimations[i].vecChannels[j].vecScalingKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecScalingKeys[k]), sizeof(MY_VECTORKEY));
				}

				AIScene.vecAnimations[i].vecChannels[j].vecRotationKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecRotationKeys[k]), sizeof(MY_QUATKEY));
				}

				AIScene.vecAnimations[i].vecChannels[j].vecPositionKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecPositionKeys[k]), sizeof(MY_VECTORKEY));
				}
			}
		}

		// == 3.2 본 ==========================================================================
		MY_AINODE& RootNode = AIScene.tRootBone;

		if (MODEL_ANIM == Data.eModelType)
		{
			// 이름
			istrDataFile.read(RootNode.szBoneName, MAX_PATH);

			// TransformationMatrix
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));

			// iNumChildren(자식 본 개수)
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));

			// vecChildren 재귀적으로 저장
			CPrefabManager::GetInstance()->ReadBones(RootNode, istrDataFile);
		}
		else
		{
			// 논애님 모델의 경우 본 데이터가 없으므로 빈 값으로 초기화하여 기록한다.
			char szName[MAX_PATH] = "";
			XMFLOAT4X4 matTransformation = {};
			unsigned int iNumChildren = 0;

			istrDataFile.read(RootNode.szBoneName, MAX_PATH);
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));
		}

		// == 3.3 메시 ==========================================================================

		// 메시 개수
		istrDataFile.read((char*)(&AIScene.iNumMeshes), sizeof(unsigned int));

		// 메시 벡터에 데이터를 기록하기 위해 resize
		AIScene.vecMeshs.resize(AIScene.iNumMeshes);

		// 메시
		for (_uint i = 0; i < AIScene.iNumMeshes; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].szName), MAX_PATH);

			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iMaterialIndex), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumVertices), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumFaces), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumBones), sizeof(_uint));

			_uint iMesh_MaterialIndex = AIScene.vecMeshs[i].iMaterialIndex;	// 메시가 사용하는 재질의 인덱스
			_uint iMesh_NumVertices = AIScene.vecMeshs[i].iNumVertices;		// 정점 개수
			_uint iMesh_NumFaces = AIScene.vecMeshs[i].iNumFaces;			// 삼각형 개수
			_uint iMesh_NumBones = AIScene.vecMeshs[i].iNumBones;			// 메시에 영향을 주는 본의 개수

			// i번 째 메시의 j번 째 Vertex 기록
			AIScene.vecMeshs[i].vecVertices.resize(iMesh_NumVertices);
			for (_uint j = 0; j < iMesh_NumVertices; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecVertices[j]), sizeof(VTXANIMMESH));
			}

			// i번 째 메시의 인덱스 기록
			AIScene.vecMeshs[i].vecFaces.resize(iMesh_NumFaces);
			for (_uint j = 0; j < iMesh_NumFaces; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecFaces[j]), sizeof(MY_AIFACE));
			}

			// i번째 메시에 영향을 주는 j번 째 Bone 기록
			AIScene.vecMeshs[i].vecAIBone.resize(iMesh_NumBones);
			for (_uint j = 0; j < iMesh_NumBones; ++j)
			{
				// j번 째 Bone에 참조로 접근해서 기록
				MY_AIBONE& AIBone = AIScene.vecMeshs[i].vecAIBone[j];

				istrDataFile.read((char*)(&AIBone.szBoneName), MAX_PATH);

				istrDataFile.read((char*)(&AIBone.matOffset), sizeof(XMFLOAT4X4));
				istrDataFile.read((char*)(&AIBone.iNumWeights), sizeof(unsigned int));

				AIBone.vecWeights.resize(AIBone.iNumWeights);
				for (_uint Index = 0; Index < AIBone.iNumWeights; ++Index)
				{
					istrDataFile.read((char*)(&AIBone.vecWeights[Index]), sizeof(MY_WEIGHT));
				}
			}
		}


		// == 3.4 재질 ==========================================================================

		// 재질 개수
		istrDataFile.read((char*)(&AIScene.iNumMaterials), sizeof(unsigned int));

		// 재질
		_uint iNumMaterials = AIScene.iNumMaterials;
		AIScene.vecMaterials.resize(iNumMaterials);
		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecMaterials[i]), sizeof(MY_AIMATERIAL));
		}

		wstring wstrModelFileName = L"";
		wstring wstrActorName = L"";

		for (_uint i = 0; i < 64; ++i)
		{
			if (Data.szModelFileName[i] == '\0') { break; }
			wstrModelFileName += Data.szModelFileName[i];
		}

		// 프리팹 매니저에 모델 정보 추가
		CPrefabManager::GetInstance()->AddModelData(wstrModelFileName, Data);

		// (※가라 주의보※) 레이어가 0(LAYER::PLAYER일 경우) or 몬스터일 경우
		// 실제 객체생성은 하지 않는다. 일단 프리팹만 생성
		// if (0 == Data.Data.iLayerIndex || 1 == Data.Data.iLayerIndex)
			// continue;

		// 실제 레벨의 레이어에 오브젝트 추가
		if (MODEL_NONANIM == Data.eModelType)
		{
			// m_vecLayers[Data.iLayerIndex]->CreateActor<CNonAnimActor>(wstrActorName);
			// MODEL_DATA& gData = CPrefabManager::GetInstance()->GetModelData(wstrModelFileName);
			// dynamic_pointer_cast<CNonAnimActor>(m_vecLayers[Prefab.iLayerIndex]->FindActor(wstrActorName))->SetModelData(gData);
		}
		else
		{
			// m_vecLayers[Data.iLayerIndex]->CreateActor<CAnimActor>(wstrActorName);
			// MODEL_DATA& gData = CPrefabManager::GetInstance()->GetModelData(wstrModelFileName);
			// dynamic_pointer_cast<CAnimActor>(m_vecLayers[Prefab.iLayerIndex]->FindActor(wstrActorName))->SetModelData(gData);
		}
	}
	istrDataFile.close();
}

void CLevel::LoadSpawn(const wstring& _wstrLevelName)
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

		switch (Spawn.tPrefab.iLayerIndex)
		{
		// 0. 플레이어는 클라이언트에서 직접 생성한다.
		case 0:
		{
			break;
		}
		// 1. 몬스터
		case 1:
		{
			spLayer->CreateActor<CAnimActor>(wstrActorName);
			dynamic_pointer_cast<CAnimActor>(spLayer->FindActor(wstrActorName))->SetModelData(Data);
			dynamic_pointer_cast<CAnimActor>(spLayer->FindActor(wstrActorName))->SetSpawnData(Spawn);
			break;
		}
		case 2:
		case 3:
		{
			spLayer->CreateActor<CNonAnimActor>(wstrActorName);
			dynamic_pointer_cast<CNonAnimActor>(spLayer->FindActor(wstrActorName))->SetModelData(Data);
			dynamic_pointer_cast<CNonAnimActor>(spLayer->FindActor(wstrActorName))->SetSpawnData(Spawn);
			break;
		}
		}
	}
}
