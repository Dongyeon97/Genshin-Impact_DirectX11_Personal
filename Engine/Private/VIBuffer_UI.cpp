#include "Engine_pch.h"
#include "VIBuffer_UI.h"

#include "DeviceManager.h"

void CVIBuffer_UI::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	InitializeBuffer();

	CNonMeshComponent::Initialize();
}

_int CVIBuffer_UI::PreUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::PreUpdate(_fTimeDelta);
}

_int CVIBuffer_UI::Update(const _float& _fTimeDelta)
{
	return CNonMeshComponent::Update(_fTimeDelta);
}

_int CVIBuffer_UI::LateUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::LateUpdate(_fTimeDelta);
}

void CVIBuffer_UI::Render()
{
	CNonMeshComponent::Render();
}

void CVIBuffer_UI::EndPlay()
{
	CNonMeshComponent::EndPlay();
}

void CVIBuffer_UI::Release()
{
	CNonMeshComponent::Release();
}

HRESULT CVIBuffer_UI::InitializeBuffer()
{
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 1;

	// ※Initialize가 호출되기 전에, 얘를 꼭 세팅해줄 것.※ -> 인스턴스 개수 설정
	// m_iNumInstance = iNumInstance;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = 1;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	/* 인덱스버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}
