#include "Engine_pch.h"
#include "NonAnimModel.h"

#include "ResourceManager.h"
#include "DeviceManager.h"
#include "PrefabManager.h"

#include "Mesh.h"

void CNonAnimModel::Initialize()
{
	// 프리미티브 컴포넌트를 상속받은 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// m_ModelData = CPrefabManager::GetInstance()->GetModelData(m_wstrModelFileName);

	// 모델 데이터로부터 받아온 정보 세팅
	m_eModelType = m_SpawnData.tPrefab.eModelType;
	// m_eLevelType = m_ModelData.tPrefab.eLevelType;
	
	// 만약 셰이더 파일이 세팅이 되어있지 않은 경우
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

	// pModelFilePath = 모델 파일(FBX)의 풀 경로
	// pModelFilePath로부터 [드라이브 경로], [폴더 경로]를 얻어온다.
	_splitpath_s(strFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	m_iNumMaterials = m_ModelData.tAIScenes.iNumMaterials;

	m_vecMaterials.resize(m_iNumMaterials);
	m_vecTextureBitFlags.assign(m_iNumMaterials, 0); // 벡터 크기 지정, 0으로 싹 초기화

	// 모델의 재질 개수만큼 순회
	for (_uint i = 0; i < m_iNumMaterials; i++)
	{
		MY_AIMATERIAL tMyAIMaterial = m_ModelData.tAIScenes.vecMaterials[i];
		m_vecMaterials[i].resize(TEXTURE_TYPE::TRANSMISSION); // 최대 텍스처 개수만큼 초기화

		// 재질의 최대 텍스처 개수인 21회만큼 순회
		for (_uint j = 0; j < TEXTURE_TYPE::TRANSMISSION; j++)
		{
			// 문자열이 비어잇다면 통과
			if (0 == strlen(&tMyAIMaterial.szTextureFilePath[j][0]))
				continue;

			// 텍스처 파일 경로에서 [파일명]과 [확장자]만 추출한다.
			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(tMyAIMaterial.szTextureFilePath[j], nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			// [드라이브] + [폴더] + [파일명] + [확장자] 를 조합해 텍스처 파일의 총 경로를 만든다.
			_char				szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			// 멀티바이트를 와이드캐릭터형으로 형변환
			_tchar				szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, (_uint)strlen(szTmp), szFullPath, MAX_PATH);

			ID3D11ShaderResourceView* pSRV = { nullptr };

			// 완성된 경로로부터 텍스처를 읽어와 생성한다.
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV)))
				return E_FAIL;

			// 텍스처가 존재하는 경우 비트 플래그 on
			m_vecTextureBitFlags[i] |= (_ubyte)j;

			// 컨테이너에 저장한다.
			m_vecMaterials[i][j] = pSRV;
		}
	}

	return S_OK;
}