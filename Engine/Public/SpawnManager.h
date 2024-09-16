#pragma once

BEGIN(Engine)

class ENGINE_DLL CSpawnManager final
{
	DECLARE_SINGLETON(CSpawnManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize();
	void Release();


/********************
	Methods
********************/
public:
	void ClearCurrentScene();

public:
	SPAWN_DATA FindSpawnData(string _strActorName);
	void AddSpawnData(string _strActorName, SPAWN_DATA _spSpawnData);
	void RemoveSpawnData(string _strActorName);


/********************
	Getter / Setter
********************/
public:
	vector<pair<string, SPAWN_DATA>>& GetSpawnData() { return m_vecSpawnData; }


/********************
	Data Members
********************/
private:
	// 스폰 데이터
	vector<pair<string, SPAWN_DATA>> m_vecSpawnData;

public:
	// 스폰 식별자
	static _uint m_iID;
};

END