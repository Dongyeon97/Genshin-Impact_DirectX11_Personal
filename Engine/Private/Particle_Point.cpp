#include "Engine_pch.h"
#include "Particle_Point.h"

#include "DeviceManager.h"

void CParticle_Point::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	m_RandomNumber = mt19937_64(m_RandomDevice());

	InitializeBuffer();

	CInstancing::Initialize();
}

_int CParticle_Point::PreUpdate(const _float& _fTimeDelta)
{
	return CInstancing::PreUpdate(_fTimeDelta);
}

_int CParticle_Point::Update(const _float& _fTimeDelta)
{
	return CInstancing::Update(_fTimeDelta);
}

_int CParticle_Point::LateUpdate(const _float& _fTimeDelta)
{
	return CInstancing::LateUpdate(_fTimeDelta);
}

void CParticle_Point::Render()
{
	CInstancing::Render();
}

void CParticle_Point::EndPlay()
{
	CInstancing::EndPlay();
}

void CParticle_Point::Release()
{
	CInstancing::Release();
}

HRESULT CParticle_Point::InitializeBuffer()
{
	m_iNumVertexBuffers = 2;
	m_iVertexStride = sizeof(VTXPOINT);
	m_iNumVertices = 1;

	// ※Initialize가 호출되기 전에, 얘를 꼭 세팅해줄 것.※ -> 인스턴스 개수 설정
	// m_iNumInstance = iNumInstance;
	m_iIndexCountPerInstance = 1;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = m_iIndexCountPerInstance * m_InstanceDesc.iNumInstance;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOINT* pVertices = new VTXPOINT[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOINT) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices[0].vPSize = _float2(1.f, 1.f);

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

#pragma region INSTANCE_BUFFER

	m_iInstanceStride = sizeof(VTXINSTANCE);

	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_InstanceDesc.iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXINSTANCE* pInstances = new VTXINSTANCE[m_InstanceDesc.iNumInstance];
	ZeroMemory(pInstances, sizeof(VTXINSTANCE) * m_InstanceDesc.iNumInstance);

	uniform_real_distribution<float>	WidthRange(m_InstanceDesc.vRange.x * -0.5f, m_InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	HeightRange(m_InstanceDesc.vRange.y * -0.5f, m_InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	DepthRange(m_InstanceDesc.vRange.z * -0.5f, m_InstanceDesc.vRange.z * 0.5f);
	uniform_real_distribution<float>	RadiusRange(m_InstanceDesc.vRadius.x, m_InstanceDesc.vRadius.y);
	uniform_real_distribution<float>	SizeRange(m_InstanceDesc.vSize.x, m_InstanceDesc.vSize.y);

	/* uniform_real_distribution<float>	RandomRange(0.1f, m_InstancingDesc.fRange);
	uniform_real_distribution<float>	RandomRotation(0.0f, XMConvertToRadians(360.0f));
	uniform_real_distribution<float>	RandomScale(m_InstancingDesc.vScale.x, m_InstancingDesc.vScale.y);
	uniform_real_distribution<float>	RandomSpeed(m_InstancingDesc.vSpeed.x, m_InstancingDesc.vSpeed.y);
	uniform_real_distribution<float>	RandomLifeTime(m_InstancingDesc.vLifeTime.x, m_InstancingDesc.vLifeTime.y); */

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		_float		fSize = SizeRange(m_RandomNumber);

		pInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		pInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		pInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);
		pInstances[i].vTranslation = _float4(
			m_InstanceDesc.vCenter.x + WidthRange(m_RandomNumber),
			m_InstanceDesc.vCenter.y + HeightRange(m_RandomNumber),
			m_InstanceDesc.vCenter.z + DepthRange(m_RandomNumber),
			1.f);

		pInstances[i].vColor = m_InstanceDesc.vColor;
	}

	m_InitialData.pSysMem = pInstances;

	if (FAILED(__super::CreateBuffer(&m_pVBInstance)))
		return E_FAIL;

	Safe_Delete_Array(pInstances);

#pragma endregion

	return S_OK;
}
