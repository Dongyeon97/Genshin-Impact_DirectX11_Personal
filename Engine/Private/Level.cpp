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
	// ��� ������ Initialize �ܰ迡�� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();
	
	// ������ ���̾ ������ ������ŭ �غ��մϴ�.
	// ReserveLayers(_iNumLayers);

	// ���̾ �ʱ�ȭ�մϴ�.
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
	// ���̾��� ���Ͱ� �̹� �ʱ�ȭ �Ǿ����� ���, E_FAIL�� ��ȯ�մϴ�.
	if (!m_vecLayers.empty())
		return E_FAIL;
	
	// ������ ���̾� ������ŭ ������ Ȯ���մϴ�.
	m_vecLayers.reserve(_iNumLayers);

	// ������ ���̾� ������ŭ ���̾ �����մϴ�.
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

	// ������ ������ ������ �а� �����մϴ�.
	ifstream istrDataFile(wstrModelDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open PrefabDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())

	{
		// 1. ���ο� �� �����͸� �����, �ű⿡ �����͸� ����Ѵ�.
		MODEL_DATA Data = {};

		// ���� ó��
		if (istrDataFile.eof())
			break;

		// 2. [���� ���]�� [������]�� ������ ũ�⸦ ������ ������, ����ü ��°�� �ҷ��´�.
		istrDataFile.read((char*)(&Data.szModelFilePath), MAX_PATH);
		istrDataFile.read((char*)(&Data.szModelFileName), MAX_PATH);
		istrDataFile.read((char*)(&Data.eModelType), sizeof(MODEL_TYPE));

		// 3. [AI_Scene]�� �������� �����Ͱ� ���ԵǾ� �ֱ� ������, �𵨸��� ���������� �ҷ��´�.
		MY_AISCENE& AIScene = Data.tAIScenes;

		// == 3.1 �ִϸ��̼� ==========================================================================
		istrDataFile.read((char*)(&AIScene.iNumAnimations), sizeof(unsigned int));

		// �ִϸ��̼� ���Ϳ� �����͸� ����ϱ� ���� resize
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

		// == 3.2 �� ==========================================================================
		MY_AINODE& RootNode = AIScene.tRootBone;

		if (MODEL_ANIM == Data.eModelType)
		{
			// �̸�
			istrDataFile.read(RootNode.szBoneName, MAX_PATH);

			// TransformationMatrix
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));

			// iNumChildren(�ڽ� �� ����)
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));

			// vecChildren ��������� ����
			CPrefabManager::GetInstance()->ReadBones(RootNode, istrDataFile);
		}
		else
		{
			// ��ִ� ���� ��� �� �����Ͱ� �����Ƿ� �� ������ �ʱ�ȭ�Ͽ� ����Ѵ�.
			char szName[MAX_PATH] = "";
			XMFLOAT4X4 matTransformation = {};
			unsigned int iNumChildren = 0;

			istrDataFile.read(RootNode.szBoneName, MAX_PATH);
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));
		}

		// == 3.3 �޽� ==========================================================================

		// �޽� ����
		istrDataFile.read((char*)(&AIScene.iNumMeshes), sizeof(unsigned int));

		// �޽� ���Ϳ� �����͸� ����ϱ� ���� resize
		AIScene.vecMeshs.resize(AIScene.iNumMeshes);

		// �޽�
		for (_uint i = 0; i < AIScene.iNumMeshes; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].szName), MAX_PATH);

			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iMaterialIndex), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumVertices), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumFaces), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumBones), sizeof(_uint));

			_uint iMesh_MaterialIndex = AIScene.vecMeshs[i].iMaterialIndex;	// �޽ð� ����ϴ� ������ �ε���
			_uint iMesh_NumVertices = AIScene.vecMeshs[i].iNumVertices;		// ���� ����
			_uint iMesh_NumFaces = AIScene.vecMeshs[i].iNumFaces;			// �ﰢ�� ����
			_uint iMesh_NumBones = AIScene.vecMeshs[i].iNumBones;			// �޽ÿ� ������ �ִ� ���� ����

			// i�� ° �޽��� j�� ° Vertex ���
			AIScene.vecMeshs[i].vecVertices.resize(iMesh_NumVertices);
			for (_uint j = 0; j < iMesh_NumVertices; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecVertices[j]), sizeof(VTXANIMMESH));
			}

			// i�� ° �޽��� �ε��� ���
			AIScene.vecMeshs[i].vecFaces.resize(iMesh_NumFaces);
			for (_uint j = 0; j < iMesh_NumFaces; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecFaces[j]), sizeof(MY_AIFACE));
			}

			// i��° �޽ÿ� ������ �ִ� j�� ° Bone ���
			AIScene.vecMeshs[i].vecAIBone.resize(iMesh_NumBones);
			for (_uint j = 0; j < iMesh_NumBones; ++j)
			{
				// j�� ° Bone�� ������ �����ؼ� ���
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


		// == 3.4 ���� ==========================================================================

		// ���� ����
		istrDataFile.read((char*)(&AIScene.iNumMaterials), sizeof(unsigned int));

		// ����
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

		// ������ �Ŵ����� �� ���� �߰�
		CPrefabManager::GetInstance()->AddModelData(wstrModelFileName, Data);

		// (�ذ��� ���Ǻ���) ���̾ 0(LAYER::PLAYER�� ���) or ������ ���
		// ���� ��ü������ ���� �ʴ´�. �ϴ� �����ո� ����
		// if (0 == Data.Data.iLayerIndex || 1 == Data.Data.iLayerIndex)
			// continue;

		// ���� ������ ���̾ ������Ʈ �߰�
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

	// ������ ������ ������ �а� �����մϴ�.
	ifstream istrDataFile(wstrSpawnDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open SpawnDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())
	{
		// ���� ó��
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
		// 0. �÷��̾�� Ŭ���̾�Ʈ���� ���� �����Ѵ�.
		case 0:
		{
			break;
		}
		// 1. ����
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
