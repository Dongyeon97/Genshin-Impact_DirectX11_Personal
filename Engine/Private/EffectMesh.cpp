#include "Engine_pch.h"
#include "EffectMesh.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

#include "Mesh.h"

// 툴 용
void CEffectMesh::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	m_eModelType = m_EffectMeshDesc.eModelType;
	m_eLevelType = LEVEL_TYPE::LEVEL_STATIC;	// 임시

#pragma region 파일명

	// 만약 셰이더 파일이 세팅이 되어있지 않은 경우
	if (m_wstrShaderFileName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_EffectMeshDesc.szShaderFileName[i] == '\0') { break; }
			m_wstrShaderFileName += m_EffectMeshDesc.szShaderFileName[i];
		}
	}

	if (m_wstrDiffuseTextureName1.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_EffectMeshDesc.szDiffuseTextureFileName1[i] == '\0') { break; }
			m_wstrDiffuseTextureName1 += m_EffectMeshDesc.szDiffuseTextureFileName1[i];
		}
	}

	if (m_wstrDiffuseTextureName2.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_EffectMeshDesc.szDiffuseTextureFileName2[i] == '\0') { break; }
			m_wstrDiffuseTextureName2 += m_EffectMeshDesc.szDiffuseTextureFileName2[i];
		}
	}

	if (m_wstrNoiseTextureName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_EffectMeshDesc.szNoiseTextureFileName[i] == '\0') { break; }
			m_wstrNoiseTextureName += m_EffectMeshDesc.szNoiseTextureFileName[i];
		}
	}

	if (m_wstrMaskTextureName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_EffectMeshDesc.szMaskTextureFileName[i] == '\0') { break; }
			m_wstrMaskTextureName += m_EffectMeshDesc.szMaskTextureFileName[i];
		}
	}

#pragma endregion 파일명

	// 임시
	m_iNumElements = Engine::VTXMESH::iNumElements;
	m_pElements = Engine::VTXMESH::Elements;
	// 임시

	// ==================================================================================
	XMMATRIX matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	XMStoreFloat4x4(&m_PivotMatrix, matPivot);

	if (FAILED(ReadyMeshes()))
	{
		MSG_BOX("Failed to Ready_Meshes : CEffectMesh");
		return;
	}

	if (FAILED(ReadyTextures()))
	{
		MSG_BOX("Failed to Ready_Textures : CEffectMesh");
		return;
	}

	CModelComponent::Initialize();
}

// 클라, 엔진용 
HRESULT CEffectMesh::Initialize(const wstring& _wstrJsonFileName)
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// 0. 외부에서 Json파일명을 입력해준다.

	// 1. Actor에서 Json파일을 읽어서, 컴포넌트에 전달해주었을 것이다. Json 파일로부터 데이터를 얻어온다.

	return S_OK;
}

_int CEffectMesh::PreUpdate(const _float& _fTimeDelta)
{
	return CModelComponent::PreUpdate(_fTimeDelta);
}

_int CEffectMesh::Update(const _float& _fTimeDelta)
{
	return CModelComponent::Update(_fTimeDelta);
}

_int CEffectMesh::LateUpdate(const _float& _fTimeDelta)
{
	return CModelComponent::LateUpdate(_fTimeDelta);
}

void CEffectMesh::Render(_uint _iMeshIndex)
{
	CModelComponent::Render(_iMeshIndex);
}

void CEffectMesh::EndPlay()
{
	CModelComponent::EndPlay();
}

void CEffectMesh::Release()
{
	CModelComponent::Release();
}

HRESULT CEffectMesh::ReadyMeshes()
{
	m_iNumMeshes = m_EffectMeshDesc.iNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		shared_ptr<CMesh> spMesh = CMesh::Create(m_eModelType, &m_EffectMeshDesc.vecMeshs[i], dynamic_pointer_cast<CModelComponent>(shared_from_this()), XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == spMesh)
			return E_FAIL;

		m_vecMeshes.push_back(spMesh);
	}

	return S_OK;
}

HRESULT CEffectMesh::ReadyTextures()
{
	if (LEVEL_DYNAMIC == m_eLevelType)
	{
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuse = CResourceManager::GetInstance()->GetSRVs();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecNoise = CResourceManager::GetInstance()->GetNoises();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecMask = CResourceManager::GetInstance()->GetMasks();

		for (auto& pair : vecDiffuse)
		{
			if (pair.first == m_wstrDiffuseTextureName1)
			{
				m_pDiffuseTexture1 = pair.second;
				break;
			}
		}
		for (auto& pair : vecDiffuse)
		{
			if (pair.first == m_wstrDiffuseTextureName2)
			{
				m_pDiffuseTexture2 = pair.second;
				break;
			}
		}

		for (auto& pair : vecNoise)
		{
			if (pair.first == m_wstrNoiseTextureName)
			{
				m_pNoiseTexture = pair.second;
				break;
			}
		}

		for (auto& pair : vecMask)
		{
			if (pair.first == m_wstrMaskTextureName)
			{
				m_pMaskTexture = pair.second;
				break;
			}
		}
	}
	else if (LEVEL_STATIC == m_eLevelType)
	{
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuse = CResourceManager::GetInstance()->GetSRVs_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecNoise = CResourceManager::GetInstance()->GetNoises_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecMask = CResourceManager::GetInstance()->GetMasks_Static();

		for (auto& pair : vecDiffuse)
		{
			if (pair.first == m_wstrDiffuseTextureName1)
			{
				m_pDiffuseTexture1 = pair.second;
				break;
			}
		}
		for (auto& pair : vecDiffuse)
		{
			if (pair.first == m_wstrDiffuseTextureName2)
			{
				m_pDiffuseTexture2 = pair.second;
				break;
			}
		}

		for (auto& pair : vecNoise)
		{
			if (pair.first == m_wstrNoiseTextureName)
			{
				m_pNoiseTexture = pair.second;
				break;
			}
		}

		for (auto& pair : vecMask)
		{
			if (pair.first == m_wstrMaskTextureName)
			{
				m_pMaskTexture = pair.second;
				break;
			}
		}
	}

	return S_OK;
}

HRESULT CEffectMesh::BindEffectTexture(EFFECT_TEXTURE_TYPE _eEffectTextureType, const _char* _pConstantName)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	switch (_eEffectTextureType)
	{
	case DIFFUSE_TEXTURE1:
	{
		if (nullptr == m_pDiffuseTexture1)
			return E_FAIL;

		return pSRVariable->SetResource(m_pDiffuseTexture1);
		break;
	}
	case DIFFUSE_TEXTURE2:
	{
		if (nullptr == m_pDiffuseTexture2)
			return E_FAIL;

		return pSRVariable->SetResource(m_pDiffuseTexture2);
		break;
	}
	case NOISE_TEXTURE:
	{
		if (nullptr == m_pNoiseTexture)
			return E_FAIL;

		return pSRVariable->SetResource(m_pNoiseTexture);
		break;
	}
	case MASK_TEXTURE:
	{
		if (nullptr == m_pMaskTexture)
			return E_FAIL;

		return pSRVariable->SetResource(m_pMaskTexture);
		break;
	}
	}

	return S_OK;
}
