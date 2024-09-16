#include "Engine_pch.h"
#include "NonAnimModel.h"

#include "ResourceManager.h"
#include "DeviceManager.h"
#include "PrefabManager.h"

#include "Mesh.h"

void CNonAnimModel::Initialize()
{
	// ������Ƽ�� ������Ʈ�� ��ӹ��� ������Ʈ�� �ʱ�ȭ�ÿ� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// m_ModelData = CPrefabManager::GetInstance()->GetModelData(m_wstrModelFileName);

	// �� �����ͷκ��� �޾ƿ� ���� ����
	m_eModelType = m_SpawnData.tPrefab.eModelType;
	// m_eLevelType = m_ModelData.tPrefab.eLevelType;
	
	// ���� ���̴� ������ ������ �Ǿ����� ���� ���
	if (m_wstrShaderFileName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_SpawnData.tPrefab.szShaderFileName[i] == '\0') { break; }
			m_wstrShaderFileName += m_SpawnData.tPrefab.szShaderFileName[i];
		}
	}

	m_iNumElements = Engine::VTXMESH::iNumElements;
	m_pElements = Engine::VTXMESH::Elements;

	// ==================================================================================
	XMMATRIX matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	XMStoreFloat4x4(&m_PivotMatrix, matPivot);

	if (FAILED(ReadyMeshes()))
	{
		MSG_BOX("Failed to Ready_Meshes : CNonAnimModel");
		return;
	}

	if (FAILED(ReadyMaterials()))
	{
		MSG_BOX("Failed to Ready_Materials : CNonAnimModel");
		return;
	}

	CModelComponent::Initialize();
}

_int CNonAnimModel::PreUpdate(const _float& _fTimeDelta)
{
	return CModelComponent::PreUpdate(_fTimeDelta);
}

_int CNonAnimModel::Update(const _float& _fTimeDelta)
{
	return CModelComponent::Update(_fTimeDelta);
}

_int CNonAnimModel::LateUpdate(const _float& _fTimeDelta)
{
	return CModelComponent::LateUpdate(_fTimeDelta);
}

void CNonAnimModel::Render(_uint _iMeshIndex)
{
	CModelComponent::Render(_iMeshIndex);
}

void CNonAnimModel::EndPlay()
{
	CModelComponent::EndPlay();
}

void CNonAnimModel::Release()
{
	CModelComponent::Release();
}

HRESULT CNonAnimModel::ReadyMeshes()
{
	m_iNumMeshes = m_ModelData.tAIScenes.iNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		shared_ptr<CMesh> spMesh = CMesh::Create(m_eModelType, &m_ModelData.tAIScenes.vecMeshs[i], dynamic_pointer_cast<CModelComponent>(shared_from_this()), XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == spMesh)
			return E_FAIL;

		m_vecMeshes.push_back(spMesh);
	}

	return S_OK;
}

HRESULT CNonAnimModel::ReadyMaterials()
{
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";

	string strFilePath = m_ModelData.szModelFilePath;

	// pModelFilePath = �� ����(FBX)�� Ǯ ���
	// pModelFilePath�κ��� [����̺� ���], [���� ���]�� ���´�.
	_splitpath_s(strFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	m_iNumMaterials = m_ModelData.tAIScenes.iNumMaterials;

	m_vecMaterials.resize(m_iNumMaterials);
	m_vecTextureBitFlags.assign(m_iNumMaterials, 0); // ���� ũ�� ����, 0���� �� �ʱ�ȭ

	// ���� ���� ������ŭ ��ȸ
	for (_uint i = 0; i < m_iNumMaterials; i++)
	{
		MY_AIMATERIAL tMyAIMaterial = m_ModelData.tAIScenes.vecMaterials[i];
		m_vecMaterials[i].resize(TEXTURE_TYPE::TRANSMISSION); // �ִ� �ؽ�ó ������ŭ �ʱ�ȭ

		// ������ �ִ� �ؽ�ó ������ 21ȸ��ŭ ��ȸ
		for (_uint j = 0; j < TEXTURE_TYPE::TRANSMISSION; j++)
		{
			// ���ڿ��� ����մٸ� ���
			if (0 == strlen(&tMyAIMaterial.szTextureFilePath[j][0]))
				continue;

			// �ؽ�ó ���� ��ο��� [���ϸ�]�� [Ȯ����]�� �����Ѵ�.
			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(tMyAIMaterial.szTextureFilePath[j], nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			// [����̺�] + [����] + [���ϸ�] + [Ȯ����] �� ������ �ؽ�ó ������ �� ��θ� �����.
			_char				szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			// ��Ƽ����Ʈ�� ���̵�ĳ���������� ����ȯ
			_tchar				szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, (_uint)strlen(szTmp), szFullPath, MAX_PATH);

			ID3D11ShaderResourceView* pSRV = { nullptr };

			// �ϼ��� ��ηκ��� �ؽ�ó�� �о�� �����Ѵ�.
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV)))
				return E_FAIL;

			// �ؽ�ó�� �����ϴ� ��� ��Ʈ �÷��� on
			m_vecTextureBitFlags[i] |= (_ubyte)j;

			// �����̳ʿ� �����Ѵ�.
			m_vecMaterials[i][j] = pSRV;
		}
	}

	return S_OK;
}