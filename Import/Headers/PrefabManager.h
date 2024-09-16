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
	/* ���� �����ͷ� �����ϱ� ���� ����Լ� */
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
	// ����ƽ ������ ������ -> �÷��̾� ���� ������� �ȵǴ� �����յ�����
	vector<pair<wstring, MODEL_DATA>> m_vecPrefabData_Static;

	// ������ ������
	vector<pair<wstring, MODEL_DATA>> m_vecPrefabData;

public:
	// ������ �ĺ���
	static _uint m_iID;
};

END