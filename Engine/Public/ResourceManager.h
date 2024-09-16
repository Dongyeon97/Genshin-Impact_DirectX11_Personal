#pragma once

/* CResourceManager */
// 1. ����ƽ ������ �ش��ϴ� ���ҽ� ��ο� ���ҽ� �����͸� ����
// 2. ���� ������ �ش��ϴ� ���ҽ� ��ο� ���ҽ� �����͸� ����
// 3. ������ ���ҽ� ��ηκ��� �ؽ�ó�� �����Ͽ� �����̳ʿ� ����
// 4. ������ ����Ǹ� �����̳ʸ� �����ϰ�, ���� �����Ǵ� ������ ���� ���ҽ����� ����

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
	// [ ����ƽ ������ <SRV> �˻� ]
	ID3D11ShaderResourceView* FindDiffuseFromStatic(const wstring& _wstrFileName);

	// [ �� ������ <SRV> �˻� ]
	ID3D11ShaderResourceView* FindDiffuse(const wstring& _wstrFileName);

public:
	// [ ���� ��� ���� ] - (�������� ������)���ҽ� Ÿ�Ժ��� �����.
	void ReadResourceFilePaths(const wstring& _wstrLevelName, RESOURCE_TYPE _eResourceType);

	// [ �ؽ�ó ���� ]
	void CreateTextureFromFile(const wstring& _wstrLevelName);

	// [ ���� ���� �ش��ϴ� ���ҽ��� ���ҽ� ��θ� ���� ]
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
	// [ ����ƽ ������ ���ҽ� ���� ��� wstring ]
	const vector<wstring>& GetFilePaths_Static(RESOURCE_TYPE _eResourceType) { return m_vecWstrFilePaths_Static[_eResourceType]; }

	// [ �� ������ ���ҽ� ���� ��� wstring ]
	const vector<wstring>& GetFilePaths(RESOURCE_TYPE _eResourceType) { return m_vecWstrFilePaths[_eResourceType]; }

	// [ ����ƽ ������ ���ҽ� ���� ��� string ]
	const vector<string>& GetStrFilePaths_Static(RESOURCE_TYPE _eResourceType) { return m_vecStrFilePaths_Static[_eResourceType]; }

	// [ �� ������ ���ҽ� ���� ���string ]
	const vector<string>& GetStrFilePaths(RESOURCE_TYPE _eResourceType) { return m_vecStrFilePaths[_eResourceType]; }

#pragma endregion File Paths


#pragma region Diffuse

public:
	// [ ����ƽ ������ <Diffuse>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetSRVs_Static() { return m_vecDiffuse_Static; }

	// [ �� ������ <Diffuse>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetSRVs() { return m_vecDiffuse; }

#pragma endregion Diffuse

#pragma region Noise

	// [ ����ƽ ������ <Noise>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetNoises_Static() { return m_vecNoise_Static; }

	// [ �� ������ <Noise>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetNoises() { return m_vecNoise; }

#pragma endregion Noise

#pragma region Mask

	// [ ����ƽ ������ <Mask>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetMasks_Static() { return m_vecMask_Static; }

	// [ �� ������ <Mask>���� ]
	const vector<pair<wstring, ID3D11ShaderResourceView*>>& GetMasks() { return m_vecMask; }

#pragma endregion Mask

#pragma region Loading Text
public:
	// [ ���� �а��ִ� ���ҽ� ���� ��� - �ε� ���� ]
	void SetCurrentResourcePath(const wstring& _wstrResourcePath) { m_wstrCurrentResourcePath = _wstrResourcePath; }
	const wstring& GetCurrentResourcePath() { return m_wstrCurrentResourcePath; }
#pragma endregion Loading Text

/********************
	Data Members
********************/
#pragma region Device
private:
	// [ ����̽� ]
	ID3D11Device* m_pDevice = nullptr;

	// [ ����̽� ���ؽ�Ʈ ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;
#pragma endregion Device

#pragma region File Paths

private:
	// [ ���ҽ� ���� ��� ]
	filesystem::path m_fsResourcePath = L"..\\..\\Resources\\";

	// [ ���������� ���� ��� ]
	filesystem::path m_fsDataPath = L"..\\..\\Resources\\Data\\";

private:
	// [ Static ������ ��� ���ҽ� ������ ��� ��� wstring ]
	vector<wstring> m_vecWstrFilePaths_Static[RESOURCE_END];

	// [ Tool ������ ��� ���ҽ� ������ ��� ��� wstring ]
	vector<wstring> m_vecWstrFilePaths_Tool[RESOURCE_END];

	// [ Current ������ ��� ���ҽ� ������ ��� ��� wstring ]
	vector<wstring> m_vecWstrFilePaths[RESOURCE_END];
	// ===============================================================
	// [ Static ������ ��� ���ҽ� ������ ��� ��� string ]
	vector<string> m_vecStrFilePaths_Static[RESOURCE_END];

	// [ Tool ������ ��� ���ҽ� ������ ��� ��� string ]
	vector<string> m_vecStrFilePaths_Tool[RESOURCE_END];

	// [ Current ������ ��� ���ҽ� ������ ��� ��� string ]
	vector<string> m_vecStrFilePaths[RESOURCE_END];

#pragma endregion File Paths


#pragma region Diffuse
	private:
	// [ Static ������ Diffuse �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecDiffuse_Static;

	// [ Current ������ Diffuse �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecDiffuse;
#pragma endregion Diffuse

#pragma region Noise
	// [ Static ������ Noise �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecNoise_Static;

	// [ Current ������ Noise �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecNoise;
#pragma endregion Noise

#pragma region Mask
	// [ Static ������ Mask �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecMask_Static;

	// [ Current ������ Mask �ؽ�ó �����͵� ]
	vector<pair<std::wstring, ID3D11ShaderResourceView*>> m_vecMask;
#pragma endregion Mask

#pragma region Loading Text
private:
	// [ ���� �а��ִ� ���ҽ��� ���� ��� ]
	wstring m_wstrCurrentResourcePath = L"";
#pragma endregion Loading Text
};

END