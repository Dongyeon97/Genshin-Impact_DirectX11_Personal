#include "Engine_pch.h"
#include "Mesh.h"

#include "DeviceManager.h"

#include "ModelComponent.h"
#include "AnimModel.h"

#include "Bone.h"

HRESULT CMesh::Initialize(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix)
{
	// ������Ƽ�� ������Ʈ�� ��ӹ��� ������Ʈ�� �ʱ�ȭ�ÿ� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	if (FAILED(InitializeBuffer(_eModelType, pAIMesh, spModel, PivotMatrix)))
		return E_FAIL;

	CSceneComponent::Initialize();
	
	return S_OK;
}

void CMesh::Release()
{
	CPrimitiveComponent::Release();
}

shared_ptr<CMesh> CMesh::Create(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix)
{
	shared_ptr<CMesh> spMesh = make_shared<CMesh>();

	if (FAILED(spMesh->Initialize(_eModelType, pAIMesh, spModel, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		spMesh->Release();
		spMesh.reset();
		return nullptr;
	}

	return spMesh;
}

HRESULT CMesh::InitializeBuffer(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->szName);

	m_iMaterialIndex = pAIMesh->iMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->iNumVertices;
	m_iIndexStride = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = pAIMesh->iNumFaces * 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_iNumTriangles = pAIMesh->iNumFaces;

#pragma region VERTEX_BUFFER
	
	HRESULT		hr = _eModelType == MODEL_NONANIM ? ReadyVertexBuffer_NonAnim(pAIMesh, PivotMatrix) : ReadyVertexBuffer_Anim(pAIMesh, spModel);
	if (FAILED(hr))
		return E_FAIL;

#pragma endregion

#pragma region INDEX_BUFFER
	/* �ε������۸� �����.*/
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };

	for (_uint i = 0; i < pAIMesh->iNumFaces; i++)
	{
		pIndices[iNumIndices++] = pAIMesh->vecFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->vecFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->vecFaces[i].mIndices[2];

		m_vecVertexIndices.push_back(_float3{ (_float)pAIMesh->vecFaces[i].mIndices[0],
											  (_float)pAIMesh->vecFaces[i].mIndices[1],
											  (_float)pAIMesh->vecFaces[i].mIndices[2] });
	}

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CMesh::ReadyVertexBuffer_NonAnim(const MY_MESH* pAIMesh, _fmatrix _matPivot)
{
	m_iVertexStride = sizeof(VTXMESH);

	/* �������۸� �����.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_vecVertexPos.reserve(m_iNumVertices);
	m_vecVertexPos.resize(m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->vecVertices[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition,
			XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), _matPivot));

		m_vecVertexPos[i] = pVertices[i].vPosition;

		memcpy(&pVertices[i].vNormal, &pAIMesh->vecVertices[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal,
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), _matPivot));

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->vecVertices[i].vTexcoord, sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->vecVertices[i].vTangent, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, 
			XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), _matPivot));
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::ReadyVertexBuffer_Anim(const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	/* �������۸� �����.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->vecVertices[i].vPosition, sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->vecVertices[i].vNormal, sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->vecVertices[i].vTexcoord, sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->vecVertices[i].vTangent, sizeof(_float3));
	}

	/* �� �޽��� �������� ���¸� �޾ƿ;��ϴ�(�� �޽ÿ� ������ �ִ�) ������ ��ü ���� .*/
	m_iNumBones = pAIMesh->iNumBones;

	for (_uint i = 0; i < m_iNumBones; i++)
	{
		/* _pAIMesh->mBones[i] : �޽ÿ� ������ �ִ� i��° �� */
		//aiBone* pAIBone = _pAIMesh->mBones[i];

		MY_AIBONE tMyAIBone = pAIMesh->vecAIBone[i];

		_float4x4 matOffset;

		/* Assimp�� ��� �ҷ������� �� ��ġ ���� ��. */
		memcpy(&matOffset, &tMyAIBone.matOffset, sizeof(_float4x4));
		XMStoreFloat4x4(&matOffset, XMMatrixTranspose(XMLoadFloat4x4(&matOffset)));

		m_vecOffsetMatrices.push_back(matOffset);

		// ���� ��ü ������ �ڽſ��� ������ �ִ� ���� �̸��� �˻��ؼ�, ������ ���� �ε�����
		// ���� �����Ѵ�.(��� �� Ÿ���� �̸��� �����Ѵ�.)
		m_vecBoneIndices.push_back(dynamic_pointer_cast<CAnimModel>(spModel)->GetBoneIndex(tMyAIBone.szBoneName));

		/* pAIBone->mNumWeights : �޽ÿ� ������ �ִ� i��° ���� ������ �ִ� ������ ���� */
		for (_uint j = 0; j < tMyAIBone.iNumWeights; j++)
		{
			/* pAIBone->mWeights[j].mVertexId : �� ���� ������ �ִ� j��° ������ �ε��� */
			/* �� �ε����� ������ vBlendWeights�� ����ִٸ�, x���� ���������� ä���ش�. */
			if (0.f == pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.x)
			{
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendIndices.x = i;
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.x = tMyAIBone.vecWeights[j].fWeight;
			}
			else if (0.f == pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.y)
			{
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendIndices.y = i;
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.y = tMyAIBone.vecWeights[j].fWeight;
			}
			else if (0.f == pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.z)
			{
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendIndices.z = i;
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.z = tMyAIBone.vecWeights[j].fWeight;
			}
			else if (0.f == pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.w)
			{
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendIndices.w = i;
				pVertices[tMyAIBone.vecWeights[j].iVertexId].vBlendWeights.w = tMyAIBone.vecWeights[j].fWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		m_vecBoneIndices.push_back(dynamic_pointer_cast<CAnimModel>(spModel)->GetBoneIndex(m_szName));

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_vecOffsetMatrices.push_back(OffsetMatrix);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

void CMesh::SetUpBoneMatrices(_float4x4* _pBoneMatrices, vector<shared_ptr<CBone>>& _vecBones) /* _vecBones : ���� ��ü �� ���� */
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		if (5000 < m_vecBoneIndices[i] || 0 > m_vecBoneIndices[i])
			continue;
		XMStoreFloat4x4(&_pBoneMatrices[i], XMLoadFloat4x4(&m_vecOffsetMatrices[i]) * XMLoadFloat4x4(_vecBones[m_vecBoneIndices[i]]->GetCombinedTransformationMatrix()));
	}
}
