#pragma once

/* CResourceManager */
// 1. 스태틱 레벨에 해당하는 리소스 경로와 리소스 포인터를 보관
// 2. 현재 레벨에 해당하는 리소스 경로와 리소스 포인터를 보관
// 3. 보관된 리소스 경로로부터 텍스처를 생성하여 컨테이너에 보관
// 4. 레벨이 변경되면 컨테이너를 정리하고, 새로 생성되는 레벨에 대한 리소스만을 보관

BEGIN(Engine)

class ENGINE_DLL CResourceManager final
{
	DECLARE_SINGLETON(CResourceManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize(PROJECT_TYPE _eProjectType);
	void Release();

/********************
	Methods
********************/
public:
	/* Diffuse */
	// [ 스태틱 레벨의 <SRV> 검색 ]
	ID3D11ShaderResourceView* FindDiffuseFromStatic(const wstring& _wstrFileName);

	// [ 현 레벨의 <SRV> 검색 ]
	ID3D11ShaderResourceView* FindDiffuse(const wstring& _wstrFileName);

public:
	// [ 파일 경로 저장 ] - (폴더별로 정리된)리소스 타입별로 저장됨.
	void ReadResourceFilePaths(const wstring& _wstrLevelName, RESOURCE_TYPE _eResourceType);

	// [ 텍스처 생성 ]
	void CreateTextureFromFile(const wstring& _wstrLevelName);

	// [ 현재 씬에 해당하는 리소스와 리소스 경로를 정리 ]
	void ClearCurrentScene();

private:
	void CreateTextures(const wstring& _wstrLevelName, RESOURCE_TYPE _eResourceType);

/********************
	Getter/Setter
********************/
#pragma region File Paths

public:
	const std::wstring GetDataPath() const { return m_fsDataPath; }

public:
	// [ 스태틱 레벨의 리소스 파일 경로 wstring ]
	const vector<wstring>& GetFilePaths_Static(RESOURCE_TYPE _eResourceType) { return m_vecWstrFilePaths_Static[_eResourceType]; }

	// [ 현 레벨의 리소스 파일 경로 wstring ]
	const vector<wstring>& GetFilePaths(RESOURCE_TYPE _eResourceType) { return m_vecWstrFilePaths[_eResourceType]; }

	// [ 스태틱 레벨의 리소스 파일 경로 string ]
	const vector<string>& GetStrFilePaths_Static(RESOURCE_TYPE _eResourceType) { return m_vecStrFilePaths_Static[_eResourceType]; }

	// [ 현 레벨의 리소스 파일 경로string ]
	const vector<string>& GetStrFilePaths(RESOURCE_TYPE _eResourceType) { return m_vecStrFilePaths[_eResourceType]; }

#pragma endregion File Paths


#pragma region Diffuse

public:
	// [ 스태틱 레벨의 <Diffuse>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetSRVs_Static() { return m_vecDiffuse_Static; }

	// [ 현 레벨의 <Diffuse>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetSRVs() { return m_vecDiffuse; }

#pragma endregion Diffuse

#pragma region Noise

	// [ 스태틱 레벨의 <Noise>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetNoises_Static() { return m_vecNoise_Static; }

	// [ 현 레벨의 <Noise>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetNoises() { return m_vecNoise; }

#pragma endregion Noise

#pragma region Mask

	// [ 스태틱 레벨의 <Mask>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetMasks_Static() { return m_vecMask_Static; }

	// [ 현 레벨의 <Mask>벡터 ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetMasks() { return m_vecMask; }

#pragma endregion Mask

#pragma region Loading Text
public:
	// [ 현재 읽고있는 리소스 파일 경로 - 로딩 씬용 ]
	void SetCurrentResourcePath(const wstring& _wstrResourcePath) { m_wstrCurrentResourcePath = _wstrResourcePath; }
	const wstring& GetCurrentResourcePath() { return m_wstrCurrentResourcePath; }
#pragma endregion Loading Text

/********************
	Data Members
********************/
#pragma region Device
private:
	// [ 디바이스 ]
	ID3D11Device* m_pDevice = nullptr;

	// [ 디바이스 컨텍스트 ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
#pragma endregion Device

#pragma region File Paths

private:
	// [ 리소스 폴더 경로 ]
	filesystem::path m_fsResourcePath = L"..\\..\\Resources\\";

	// [ 데이터파일 폴더 경로 ]
	filesystem::path m_fsDataPath = L"..\\..\\Resources\\Data\\";

private:
	// [ Static 레벨의 모든 리소스 파일의 상대 경로 wstring ]
	vector<wstring> m_vecWstrFilePaths_Static[RESOURCE_END];

	// [ Tool 레벨의 모든 리소스 파일의 상대 경로 wstring ]
	vector<wstring> m_vecWstrFilePaths_Tool[RESOURCE_END];

	// [ Current 레벨의 모든 리소스 파일의 상대 경로 wstring ]
	vector<wstring> m_vecWstrFilePaths[RESOURCE_END];
	// ===============================================================
	// [ Static 레벨의 모든 리소스 파일의 상대 경로 string ]
	vector<string> m_vecStrFilePaths_Static[RESOURCE_END];

	// [ Tool 레벨의 모든 리소스 파일의 상대 경로 string ]
	vector<string> m_vecStrFilePaths_Tool[RESOURCE_END];

	// [ Current 레벨의 모든 리소스 파일의 상대 경로 string ]
	vector<string> m_vecStrFilePaths[RESOURCE_END];

#pragma endregion File Paths


#pragma region Diffuse
	private:
	// [ Static 레벨의 Diffuse 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecDiffuse_Static;

	// [ Current 레벨의 Diffuse 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecDiffuse;
#pragma endregion Diffuse

#pragma region Noise
	// [ Static 레벨의 Noise 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecNoise_Static;

	// [ Current 레벨의 Noise 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecNoise;
#pragma endregion Noise

#pragma region Mask
	// [ Static 레벨의 Mask 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecMask_Static;

	// [ Current 레벨의 Mask 텍스처 포인터들 ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecMask;
#pragma endregion Mask

#pragma region Loading Text
private:
	// [ 현재 읽고있는 리소스의 파일 경로 ]
	wstring m_wstrCurrentResourcePath = L"";
#pragma endregion Loading Text
};

END