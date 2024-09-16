#include "Engine_pch.h"
#include "VIBuffer_Terrain.h"

#include "DeviceManager.h"

void CVIBuffer_Terrain::Initialize()
{
	// 프리미티브 컴포넌트를 상속받은 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// 버퍼
	CVIBuffer_Terrain::InitializeBuffer(m_wstrHeightMapFilePath);

	CNonMeshComponent::Initialize();
}

_int CVIBuffer_Terrain::PreUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::PreUpdate(_fTimeDelta);
}

_int CVIBuffer_Terrain::Update(const _float& _fTimeDelta)
{
	return CNonMeshComponent::Update(_fTimeDelta);
}

_int CVIBuffer_Terrain::LateUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::LateUpdate(_fTimeDelta);
}

void CVIBuffer_Terrain::Render()
{
	CNonMeshComponent::Render();

	// m_pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);
}

void CVIBuffer_Terrain::EndPlay()
{
	CNonMeshComponent::EndPlay();
}

void CVIBuffer_Terrain::Release()
{
	CNonMeshComponent::Release();
}

void CVIBuffer_Terrain::SetHeightMapFileInfo(const wstring& _wstrHeightMapFilePath)
{
	m_wstrHeightMapFilePath = _wstrHeightMapFilePath;
}

HRESULT CVIBuffer_Terrain::InitializeBuffer(const wstring& _wstrHeightMapFilePath)
{
	_ulong		dwByte = { 0 };
	HANDLE		hFile = CreateFile(_wstrHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER			fh;
	ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr);

	BITMAPINFOHEADER			ih;
	ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr);

	_uint* pPixels = new _uint[ih.biWidth * ih.biHeight];
	ReadFile(hFile, pPixels, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVerticesX = ih.biWidth;
	m_iNumVerticesZ = ih.biHeight;

	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesZ;

	m_iIndexStride = 4;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = (m_iNumVerticesX - 1) * (m_iNumVerticesZ - 1) * 2 * 3;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// m_vecVertexWorldPos.resize(m_iNumVertices);

#pragma region VERTEX_BUFFER

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			/*		11111111 11111111 11111111 10110101

			&		00000000 00000000 00000000 11111111

					00000000 00000000 00000000 10110101*/

			pVertices[iIndex].vPosition = _float3((_float)j, (pPixels[iIndex] & 0x000000ff) / 10.0f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2((_float)j / (m_iNumVerticesX - 1.f), (_float)i / (m_iNumVerticesZ - 1.f));

			_float4 vVertexPos;
			vVertexPos.x = pVertices[iIndex].vPosition.x;
			vVertexPos.y = pVertices[iIndex].vPosition.y;
			vVertexPos.z = pVertices[iIndex].vPosition.z;
			vVertexPos.w = 1.0f;
			m_vecVertexPos.push_back(vVertexPos);
		}
	}

	Safe_Delete_Array(pPixels);

#pragma endregion

#pragma region INDEX_BUFFER
	/* 인덱스버퍼를 만든다.*/

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint		iNumIndices = { 0 };


	for (_uint i = 0; i < m_iNumVerticesZ - 1; i++)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; j++)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			_uint	iIndices[4] = {
				iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector vSourDir, vDestDir, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal));

			// 삼각형 하나 만들고, 인덱스 보관.
			m_vecVertexIndices.push_back(_float3{ (_float)iIndices[0], (_float)iIndices[1], (_float)iIndices[2] });
			++m_iNumTriangles;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSourDir, vDestDir));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal));
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal));

			// 삼각형 하나 만들고, 인덱스 보관.
			m_vecVertexIndices.push_back(_float3{ (_float)iIndices[0], (_float)iIndices[2], (_float)iIndices[3] });
			++m_iNumTriangles;
		}
	}

	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	m_InitialData.pSysMem = pVertices;

	if (FAILED(CPrimitiveComponent::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;


	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(CPrimitiveComponent::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}
