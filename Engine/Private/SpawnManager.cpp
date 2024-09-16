#include "Engine_pch.h"
#include "SpawnManager.h"

IMPLEMENT_SINGLETON(CSpawnManager)

_uint CSpawnManager::m_iID = 0;

HRESULT CSpawnManager::Initialize()
{
	return S_OK;
}

void CSpawnManager::Release()
{
	for (auto& pair : m_vecSpawnData)
	{
		//if (pair.second)
		//	pair.second.reset(); 
	}

	m_vecSpawnData.clear();

	// 매니저 파괴
	m_upInstance.reset();
}

void CSpawnManager::ClearCurrentScene()
{
	m_vecSpawnData.clear();
}

SPAWN_DATA CSpawnManager::FindSpawnData(string _strSpawnName)
{
	for (const auto& pair : m_vecSpawnData)
	{
		if (pair.first == _strSpawnName)
			return pair.second;
	}

	return SPAWN_DATA();
}

void CSpawnManager::AddSpawnData(string _strActorName, SPAWN_DATA _spSpawnData)
{
	auto iter = find_if(m_vecSpawnData.begin(), m_vecSpawnData.end(), [&](auto pair) {
		if (pair.first == _strActorName)
			return true;
		return false; });

	if (iter != m_vecSpawnData.end())
	{
		if (2 == _spSpawnData.tPrefab.iLayerIndex || 3 == _spSpawnData.tPrefab.iLayerIndex)
			return;

		// MSG_BOX("중복되는 액터 이름이 존재합니다.");
		m_vecSpawnData.push_back(make_pair(_strActorName + to_string(m_iID++), _spSpawnData));
		return;
	}

	m_vecSpawnData.push_back(make_pair(_strActorName, _spSpawnData));
}

void CSpawnManager::RemoveSpawnData(string _strActorName)
{
	for (auto iter = m_vecSpawnData.begin(); iter != m_vecSpawnData.end(); ++iter)
	{
		if ((*iter).first == _strActorName)
		{
			/*(*iter).second.reset();*/
			m_vecSpawnData.erase(iter);
			break;
		}
	}
}
