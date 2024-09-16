#pragma once

/* AssimpManager : assimp 라이브러리로 생성된 모델들을 저장, 관리*/

class CAssimpManager
{
	DECLARE_SINGLETON(CAssimpManager)

public:
	HRESULT Initialize();
	void Release();

public:
	// [ 레벨에 해당하는 폴더로 들어가 _eModelType에 해당하는 모델을 전부 생성 ] -> 폐지
	// HRESULT CreateModelFromFolder(const wstring& _wstrLevelName, MODEL_TYPE _eModelType);

	// [ 파일 경로 하나만 받아서 _eModelType에 해당하는 모델을 한개 생성 ]
	const aiScene* CreateModelFromFile(const string& _strFilePath, MODEL_TYPE _eModelType);

public:
	// [ wstring 태그로 aiScene을 찾아줌 ]
	const aiScene* FindAISceneFromLevel(const wstring& _wstrFileName);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	// [ 생성한 모델 수 전역변수 ]
	static _uint iNumCreatedModel;

	// [ 모델파일 경로 / aiScene ]
	unordered_map<wstring, pair<shared_ptr<Assimp::Importer>, const aiScene*>> m_umapImporter_AIScene;

};
