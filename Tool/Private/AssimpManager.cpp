#include "pch.h"
#include "AssimpManager.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

IMPLEMENT_SINGLETON(CAssimpManager)

_uint CAssimpManager::iNumCreatedModel = 0;

HRESULT CAssimpManager::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	return S_OK;
}

void CAssimpManager::Release()
{
	/* 모델 정리 */
	for (auto& pair : m_umapImporter_AIScene)
	{
		(pair.second).first->FreeScene();
	}
	m_umapImporter_AIScene.clear();
	
	m_upInstance.reset();
}

const aiScene* CAssimpManager::FindAISceneFromLevel(const wstring& _wstrFileName)
{
	auto iter = m_umapImporter_AIScene.find(_wstrFileName);

	if (iter == m_umapImporter_AIScene.end())
		return nullptr;

	return (iter->second).second;
}

const aiScene* CAssimpManager::CreateModelFromFile(const string& _strFilePath, MODEL_TYPE _eModelType)
{
	shared_ptr<Assimp::Importer> spImporter = make_shared<Assimp::Importer>();
	const aiScene* pAIScene = nullptr;

	// 모델 세워주는 플래그 설정
	_uint iFlag = aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded;

	if (_eModelType == MODEL_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	pAIScene = (*spImporter.get()).ReadFile(_strFilePath, iFlag);
	if (nullptr == pAIScene || pAIScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pAIScene->mRootNode)
	{
		MSG_BOX("Failed to Create AIScene : ResourceManager");
		return nullptr;
	}

	_char szFileName[MAX_PATH] = "";
	_splitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

	wstring wstrFileName = L"";

	for (_uint i = 0; i < MAX_PATH; ++i)
	{
		if (szFileName[i] == '\0') { break; }
		wstrFileName += szFileName[i];
	}

	// 이름 중복 검사
	auto iter = m_umapImporter_AIScene.find(wstrFileName);
	if (iter != m_umapImporter_AIScene.end())
		wstrFileName += iNumCreatedModel++;

	// m_umapFileName_FilePath.emplace(wstrFileName, strTmp);
	m_umapImporter_AIScene.emplace(wstrFileName, make_pair(spImporter, pAIScene));

	return pAIScene;
}
