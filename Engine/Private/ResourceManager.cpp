#include "Engine_pch.h"
#include "ResourceManager.h"
#include "DeviceManager.h"

IMPLEMENT_SINGLETON(CResourceManager)

HRESULT CResourceManager::Initialize(PROJECT_TYPE _eProjectType)
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	switch (_eProjectType)
	{
	case PROJECT_CLIENT:
	{
		// ����ƽ������ �ʿ��� ���ҽ� ��θ� �н��ϴ�.
		ReadResourceFilePaths(L"Level_Static", RESOURCE_SHADER); // ���̴��� static�� ����ֽ��ϴ�.
		ReadResourceFilePaths(L"Level_Static", RESOURCE_DIFFUSETEXTURE);
		ReadResourceFilePaths(L"Level_Static", RESOURCE_NOISETEXTURE);
		ReadResourceFilePaths(L"Level_Static", RESOURCE_MASKTEXTURE);
		// ReadResourceFilePaths(L"Level_Static", RESOURCE_NONANIMMODEL);
		// ReadResourceFilePaths(L"Level_Static", RESOURCE_ANIMMODEL);
		// ReadResourceFilePaths(L"Level_Loading", RESOURCE_SOUND);
		// ReadResourceFilePaths(L"Level_Loading", RESOURCE_MESH); ... ���� �߰� ����

		// ����ƽ������ �ʿ��� �ؽ�ó �����͸� ����ϴ�.
		CreateTextureFromFile(L"Level_Static");
		break;
	}
	case PROJECT_TOOL:
	{
		// Tool�� �ʿ��� ���ҽ� ��θ� �н��ϴ�.
		ReadResourceFilePaths(L"Level_Static", RESOURCE_SHADER); // ���̴��� static�� ����ֽ��ϴ�.
		ReadResourceFilePaths(L"Level_Static", RESOURCE_DIFFUSETEXTURE);
		ReadResourceFilePaths(L"Level_Static", RESOURCE_NOISETEXTURE);
		ReadResourceFilePaths(L"Level_Static", RESOURCE_MASKTEXTURE);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_DIFFUSETEXTURE);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_NOISETEXTURE);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_MASKTEXTURE);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_NONANIMMODEL);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_ANIMMODEL);
		ReadResourceFilePaths(L"Level_Tool", RESOURCE_EFFECTMESH);
		// ReadResourceFilePaths(L"Level_Loading", RESOURCE_SOUND);
		// ReadResourceFilePaths(L"Level_Loading", RESOURCE_MESH); ... ���� �߰� ����

		// ����ƽ������ �ʿ��� �ؽ�ó �����͸� ����ϴ�.
		CreateTextureFromFile(L"Level_Tool");
		CreateTextureFromFile(L"Level_Static");
		break;
	}
	}

	return S_OK;
}

void CResourceManager::Release()
{
	/* Diffuse */
	for (auto& pair : m_vecDiffuse)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecDiffuse.clear();

	for (auto& pair : m_vecDiffuse_Static)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecDiffuse_Static.clear();


	/* Noise */
	for (auto& pair : m_vecNoise)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecNoise.clear();

	for (auto& pair : m_vecNoise_Static)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecNoise_Static.clear();


	/* Mask */
	for (auto& pair : m_vecMask)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecMask.clear();

	for (auto& pair : m_vecMask_Static)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecMask_Static.clear();

	// ���ҽ� �Ŵ��� �ı�
	m_upInstance.reset();
}

void CResourceManager::ReadResourceFilePaths(const wstring& _wstrLevelName, RESOURCE_TYPE _eResourceType)
{
	// L"..\\..\\Resources\\"
	filesystem::path fsFilePath = m_fsResourcePath;

	// L"..\\..\\Resources\\_wstrLevelName"
	fsFilePath += _wstrLevelName;
	
	// L"..\\..\\Resources\\LevelName\\ResourceType\\"
	switch (_eResourceType)
	{
	case RESOURCE_DIFFUSETEXTURE:
	{
		fsFilePath += L"\\Textures\\Diffuse\\";
		break;
	}
	case RESOURCE_NOISETEXTURE:
	{
		fsFilePath += L"\\Textures\\Noise\\";
		break;
	}
	case RESOURCE_MASKTEXTURE:
	{
		fsFilePath += L"\\Textures\\Mask\\";
		break;
	}
	case RESOURCE_SOUND:
	{
		fsFilePath += L"\\Sounds\\";
		break;
	}
	case RESOURCE_SHADER:
	{
		fsFilePath += L"\\ShaderFiles\\";
		break;
	}
	case RESOURCE_NONANIMMODEL:
	{
		fsFilePath += L"\\Models\\NonAnim\\";
		break;
	}
	case RESOURCE_ANIMMODEL:
	{
		fsFilePath += L"\\Models\\Anim\\";
		break;
	}
	case RESOURCE_EFFECTMESH:
	{
		fsFilePath += L"\\Models\\EffectMesh\\";
		break;
	}
	}

	if (fsFilePath.empty())
	{
		MSG_BOX("Failed to Read ResourceFilePath : CResourceManager");
		return;
	}

	for (const filesystem::directory_entry& fsEntry : filesystem::recursive_directory_iterator(fsFilePath))
	{
		if (filesystem::is_regular_file(fsEntry))
		{
			wstring wstrFilePath = fsEntry.path();

			// ���� ��ο��� Ȯ���ڸ� ������ ������ �̸��� �����մϴ�.
			size_t iCursor = wstrFilePath.find_last_of(L"\\/");

			wstring wstrFileNameExt = L"";
			if (iCursor != string::npos) { wstrFileNameExt = wstrFilePath.substr(iCursor + 1); }
			else { wstrFileNameExt = wstrFilePath; }

			iCursor = wstrFileNameExt.find_last_of(L".");

			// ������ Ȯ����
			const wstring& wstrFileExtension = wstrFileNameExt.substr(iCursor + 1);

			if (wstrFileExtension == L"tga" || wstrFileExtension == L"TGA")
				continue;

			if (L"Level_Static" == _wstrLevelName)
			{
				if (wstrFileExtension == L"dds" || wstrFileExtension == L"DDS")
				{
					m_vecWstrFilePaths_Static[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path());
					m_vecStrFilePaths_Static[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path().generic_string());
				}
				else if (wstrFileExtension == L"hlsl" || wstrFileExtension == L"HLSL")
				{
					m_vecWstrFilePaths_Static[RESOURCE_SHADER].push_back(fsEntry.path());
					m_vecStrFilePaths_Static[RESOURCE_SHADER].push_back(fsEntry.path().generic_string());
				}
				else if((wstrFileExtension == L"png" || wstrFileExtension == L"PNG") ||
						(wstrFileExtension == L"jpg" || wstrFileExtension == L"JPG"))
				{
					// ã�� ��ΰ� �������� ��ζ��, �ؽ�ó ��ηδ� �������� �ʽ��ϴ�.
					size_t iFindName = wstrFilePath.find(L"Models");
					if (iFindName != wstring::npos) continue;

					if (RESOURCE_DIFFUSETEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_NOISETEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_NOISETEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_NOISETEXTURE].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_MASKTEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_MASKTEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_MASKTEXTURE].push_back(fsEntry.path().generic_string());
					}
				}
				else if ((wstrFileExtension == L"mp3" || wstrFileExtension == L"MP3") ||
						 (wstrFileExtension == L"ogg" || wstrFileExtension == L"OGG"))
				{
					m_vecWstrFilePaths_Static[RESOURCE_SOUND].push_back(fsEntry.path());
					m_vecStrFilePaths_Static[RESOURCE_SOUND].push_back(fsEntry.path().generic_string());
				}
				else if (wstrFileExtension == L"fbx" || wstrFileExtension == L"FBX")
				{
					if (RESOURCE_NONANIMMODEL == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_NONANIMMODEL].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_NONANIMMODEL].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_ANIMMODEL == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_ANIMMODEL].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_ANIMMODEL].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_EFFECTMESH == _eResourceType)
					{
						m_vecWstrFilePaths_Static[RESOURCE_EFFECTMESH].push_back(fsEntry.path());
						m_vecStrFilePaths_Static[RESOURCE_EFFECTMESH].push_back(fsEntry.path().generic_string());
					}
				}
			}
			else
			{
				if (wstrFileExtension == L"dds" || wstrFileExtension == L"DDS")
				{
					m_vecWstrFilePaths[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path());
					m_vecStrFilePaths[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path().generic_string());
				}
				else if (wstrFileExtension == L"hlsl" || wstrFileExtension == L"HLSL")
				{
					m_vecWstrFilePaths[RESOURCE_SHADER].push_back(fsEntry.path());
					m_vecStrFilePaths[RESOURCE_SHADER].push_back(fsEntry.path().generic_string());
				}
				else if ((wstrFileExtension == L"png" || wstrFileExtension == L"PNG") ||
					(wstrFileExtension == L"jpg" || wstrFileExtension == L"JPG"))
				{
					// ã�� ��ΰ� �������� ��ζ��, �ؽ�ó ��ηδ� �������� �ʽ��ϴ�.
					size_t iFindName = wstrFilePath.find(L"Models");
					if (iFindName != wstring::npos) continue;

					if (RESOURCE_DIFFUSETEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_DIFFUSETEXTURE].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_NOISETEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_NOISETEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_NOISETEXTURE].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_MASKTEXTURE == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_MASKTEXTURE].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_MASKTEXTURE].push_back(fsEntry.path().generic_string());
					}
				}
				else if ((wstrFileExtension == L"mp3" || wstrFileExtension == L"MP3") ||
					(wstrFileExtension == L"ogg" || wstrFileExtension == L"OGG"))
				{
					m_vecWstrFilePaths[RESOURCE_SOUND].push_back(fsEntry.path());
					m_vecStrFilePaths[RESOURCE_SOUND].push_back(fsEntry.path().generic_string());
				}
				else if (wstrFileExtension == L"fbx" || wstrFileExtension == L"FBX")
				{
					if (RESOURCE_NONANIMMODEL == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_NONANIMMODEL].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_NONANIMMODEL].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_ANIMMODEL == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_ANIMMODEL].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_ANIMMODEL].push_back(fsEntry.path().generic_string());
					}
					else if (RESOURCE_EFFECTMESH == _eResourceType)
					{
						m_vecWstrFilePaths[RESOURCE_EFFECTMESH].push_back(fsEntry.path());
						m_vecStrFilePaths[RESOURCE_EFFECTMESH].push_back(fsEntry.path().generic_string());
					}
				}
			}
		}
	}
}

void CResourceManager::ClearCurrentScene()
{
	/* Diffuse ���� */
	for (auto& pair : m_vecDiffuse)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecDiffuse.clear();

	/* Noise ���� */
	for (auto& pair : m_vecNoise)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecNoise.clear();

	/* Mask ���� */
	for (auto& pair : m_vecMask)
	{
		if (pair.second)
		{
			pair.second->Release();
			pair.second = nullptr;
		}
	}
	m_vecMask.clear();

	for (_uint i = 0; i < RESOURCE_END; ++i)
	{
		m_vecWstrFilePaths[i].clear();
		m_vecStrFilePaths[i].clear();

		m_vecWstrFilePaths_Tool[i].clear();
		m_vecStrFilePaths_Tool[i].clear();
	}
	m_vecWstrFilePaths->clear();
	m_vecStrFilePaths->clear();
}

void CResourceManager::CreateTextures(const wstring& _wstrLevelName, RESOURCE_TYPE _eResourceType)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	vector<wstring> vecPaths = {};

	if (L"Level_Static" == _wstrLevelName)
		vecPaths = GetFilePaths_Static(_eResourceType);
	else
		vecPaths = GetFilePaths(_eResourceType);

	for (const auto& wstrFilePath : vecPaths)
	{
		// ���� ��ο��� Ȯ���ڸ� ������ ������ �̸��� �����մϴ�.
		size_t iCursor = wstrFilePath.find_last_of(L"\\/");

		wstring wstrFileNameExt = L"";
		if (iCursor != string::npos) { wstrFileNameExt = wstrFilePath.substr(iCursor + 1); }
		else { wstrFileNameExt = wstrFilePath; }

		iCursor = wstrFileNameExt.find_last_of(L".");

		// ������ Ȯ����(.png)
		const wstring& wstrFileExtension = wstrFileNameExt.substr(iCursor + 1);

		// ������ �̸�("WhiteWall0")
		const wstring& wstrFileName = wstrFileNameExt.substr(0, iCursor);

		HRESULT hr = 0;

		// dds Ȯ���� ������ ���
		if (wstrFileExtension == L"dds" || wstrFileExtension == L"DDS")
		{
			hr = CreateDDSTextureFromFile(m_pDevice, wstrFilePath.c_str(), nullptr, &pSRV);
		}

		// tga Ȯ���� ������ ��� : �Ұ���
		else if (wstrFileExtension == L"tga" || wstrFileExtension == L"TGA")
		{
			hr = E_FAIL;
		}

		// dds, tga�� ������ ������
		else
		{
			hr = CreateWICTextureFromFile(m_pDevice, wstrFilePath.c_str(), nullptr, &pSRV);
		}

		// �ε����� �ؽ�ó ����� ���ڿ�
		m_wstrCurrentResourcePath = wstrFileName + wstrFileExtension;

		// ����ƽ���� ���ҽ��� ���� ����ñ��� �������� �ʴ� �����̳ʿ� ����.
		if (L"Level_Static" == _wstrLevelName)
		{
			switch (_eResourceType)
			{
			case RESOURCE_TYPE::RESOURCE_DIFFUSETEXTURE:
			{
				m_vecDiffuse_Static.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			case RESOURCE_TYPE::RESOURCE_NOISETEXTURE:
			{
				m_vecNoise_Static.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			case RESOURCE_TYPE::RESOURCE_MASKTEXTURE:
			{
				m_vecMask_Static.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			}
		}	
		else
		{
			switch (_eResourceType)
			{
			case RESOURCE_TYPE::RESOURCE_DIFFUSETEXTURE:
			{
				m_vecDiffuse.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			case RESOURCE_TYPE::RESOURCE_NOISETEXTURE:
			{
				m_vecNoise.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			case RESOURCE_TYPE::RESOURCE_MASKTEXTURE:
			{
				m_vecMask.push_back(make_pair(wstrFileName, pSRV));
				break;
			}
			}
		}
	}
}

void CResourceManager::CreateTextureFromFile(const wstring& _wstrLevelName)
{
	CreateTextures(_wstrLevelName, RESOURCE_DIFFUSETEXTURE);
	CreateTextures(_wstrLevelName, RESOURCE_NOISETEXTURE);
	CreateTextures(_wstrLevelName, RESOURCE_MASKTEXTURE);
}

ID3D11ShaderResourceView* CResourceManager::FindDiffuseFromStatic(const wstring& _wstrFileName)
{
	for (auto& pair : m_vecDiffuse_Static)
	{
		if (pair.first == _wstrFileName)
			return pair.second;
	}

	return nullptr;
}

ID3D11ShaderResourceView* CResourceManager::FindDiffuse(const wstring& _wstrFileName)
{
	for (auto& pair : m_vecDiffuse)
	{
		if (pair.first == _wstrFileName)
			return pair.second;
	}
	return nullptr;
}