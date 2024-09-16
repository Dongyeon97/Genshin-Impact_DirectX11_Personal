#include "Engine_pch.h"
#include "VIBuffer_Rect.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

void CVIBuffer_Rect::Initialize()
{
	// 프리미티브 컴포넌트를 상속받은 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// 버퍼
	CVIBuffer_Rect::InitializeBuffer(); // 순수 가상 함수

	CNonMeshComponent::Initialize();
}

_int CVIBuffer_Rect::PreUpdate(const _float& _fTimeDelta)
{

	return CNonMeshComponent::PreUpdate(_fTimeDelta);
}

_int CVIBuffer_Rect::Update(const _float& _fTimeDelta)
{
	// 매 프레임 WorldMatrix에 따른 정점의 위치 업데이트
	UpdateVerticesTransform();

	return CNonMeshComponent::Update(_fTimeDelta);
}

_int CVIBuffer_Rect::LateUpdate(const _float& _fTimeDelta)
{

	return CNonMeshComponent::LateUpdate(_fTimeDelta);
}

void CVIBuffer_Rect::Render()
{
	CNonMeshComponent::Render();

	// m_pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);
}

void CVIBuffer_Rect::EndPlay()
{

	CNonMeshComponent::EndPlay();
}

void CVIBuffer_Rect::Release()
{
	
	CNonMeshComponent::Release();
}

HRESULT CVIBuffer_Rect::InitializeBuffer()
{
	// [ 버텍스 버퍼의 개수 ]
	m_iNumVertexBuffers = 1;
	// [ 버텍스 한개의 바이트 사이즈 ]
	m_iVertexStride = sizeof(VTXPOSTEX);
	// [ 버텍스 개수 ]
	m_iNumVertices = 4;

	// [ 인덱스 한개의 바이트 사이즈 ] -> 정점 개수가 65535개가 넘어가면 4바이트로 바꿔야 한다.
	m_iIndexStride = 2;
	// [ 인덱스 포맷 ] -> 인덱스 한개를 몇바이트짜리 어떤 자료형으로 쓸 것인가?
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	// [ 인덱스 개수 ] - [ 0, 1, 2, 0, 2, 3 ]
	m_iNumIndices = 6;
	// [ 토폴로지 ]
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_vecVertexWorldPos.resize(m_iNumVertices);

#pragma region 버텍스 버퍼

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	for (_uint i = 0; i < 4; ++i)
	{
		_float4 vVertexPos;
		vVertexPos.x = pVertices[i].vPosition.x;
		vVertexPos.y = pVertices[i].vPosition.y;
		vVertexPos.z = pVertices[i].vPosition.z;
		vVertexPos.w = 1.0f;

		m_vecVertexPos.push_back(vVertexPos);
	}

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion 버텍스 버퍼

#pragma region 인덱스 버퍼

	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	m_vecVertexIndices.push_back(_float3{ (_float)pIndices[0] , (_float)pIndices[1] , (_float)pIndices[2] });

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;
	m_vecVertexIndices.push_back(_float3{ (_float)pIndices[0] , (_float)pIndices[2] , (_float)pIndices[3] });

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion 인덱스 버퍼

	return S_OK;
}