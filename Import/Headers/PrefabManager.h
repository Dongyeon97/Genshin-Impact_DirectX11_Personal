#pragma once

BEGIN(Engine)

class ENGINE_DLL CPrefabManager final
{
	DECLARE_SINGLETON(CPrefabManager)

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
	MODEL_DATA FindModelData(wstring _wstrModelName);

	void AddModelData(wstring _wstrModelName, MODEL_DATA _tModelData);

	void RemoveModelData(wstring _wstrModelName);

	void ClearCurrentScene(_uint _iLevelIndex);

public:
	/* 뼈를 데이터로 저장하기 위한 재귀함수 */
	HRESULT WriteBones(MY_AINODE& _MyaiNode, ofstream& _Ostr);
	HRESULT ReadBones(MY_AINODE& _MyaiNode, ifstream& _Istr);

/********************
	Getter/Setter
********************/
public:
	const vector<pair<wstring, MODEL_DATA>>& GetPrefabDatas() { return m_vecPrefabData; }
	const vector<pair<wstring, MODEL_DATA>>& GetPrefabDatas_Static() { return m_vecPrefabData_Static; }

public:
	MODEL_DATA* GetModelData(wstring _wstrModelName);
	MODEL_DATA* GetModelData_Static(wstring _wstrModelName);

/********************
	Data Members
********************/
private:
	// 스태틱 프리팹 데이터 -> 플레이어 등의 사라지면 안되는 프리팹데이터
	vector<pair<wstring, MODEL_DATA>> m_vecPrefabData_Static;

	// 프리팹 데이터
	vector<pair<wstring, MODEL_DATA>> m_vecPrefabData;

public:
	// 프리팹 식별자
	static _uint m_iID;
};

END