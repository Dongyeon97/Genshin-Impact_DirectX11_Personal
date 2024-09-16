#pragma once

/* AssimpManager : assimp ���̺귯���� ������ �𵨵��� ����, ����*/

class CAssimpManager
{
	DECLARE_SINGLETON(CAssimpManager)

public:
	HRESULT Initialize();
	void Release();

public:
	// [ ������ �ش��ϴ� ������ �� _eModelType�� �ش��ϴ� ���� ���� ���� ] -> ����
	// HRESULT CreateModelFromFolder(const wstring& _wstrLevelName, MODEL_TYPE _eModelType);

	// [ ���� ��� �ϳ��� �޾Ƽ� _eModelType�� �ش��ϴ� ���� �Ѱ� ���� ]
	const aiScene* CreateModelFromFile(const string& _strFilePath, MODEL_TYPE _eModelType);

public:
	// [ wstring �±׷� aiScene�� ã���� ]
	const aiScene* FindAISceneFromLevel(const wstring& _wstrFileName);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	// [ ������ �� �� �������� ]
	static _uint iNumCreatedModel;

	// [ ������ ��� / aiScene ]
	unordered_map<wstring, pair<shared_ptr<Assimp::Importer>, const aiScene*>> m_umapImporter_AIScene;

};
