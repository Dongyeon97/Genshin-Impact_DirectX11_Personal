#include "Engine_pch.h"
#include "Cell.h"

#include "DeviceManager.h"

HRESULT CCell::Initialize(const _float3* _pPoints, _uint iCellIndex)
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	for (_uint i = 0; i < POINT_END; i++)
	{
		m_vPoints[i] = _pPoints[i];
	}
	m_iIndex = iCellIndex;

	// �츮�� �������͸� x,z������� �翵�ؼ�, �ΰ��� �ุ�� ������ �Ǵ��� ���̱� ������,
	// �����ϰ� x,z�� ������, x�� ��ȣ�� �ٲٴ� ������ ���� ���͸� ���Ѵ�. �׸��� �����Ѵ�.
	XMStoreFloat3(&m_vNormals[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	_float3 vNormal = _float3(m_vNormals[LINE_AB].z * -1.f, 0.f, m_vNormals[LINE_AB].x);
	XMStoreFloat3(&m_vNormals[LINE_AB], XMVector3Normalize(XMLoadFloat3(&vNormal)));

	XMStoreFloat3(&m_vNormals[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	vNormal = _float3(m_vNormals[LINE_BC].z * -1.f, 0.f, m_vNormals[LINE_BC].x);
	XMStoreFloat3(&m_vNormals[LINE_BC], XMVector3Normalize(XMLoadFloat3(&vNormal)));

	XMStoreFloat3(&m_vNormals[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));
	vNormal = _float3(m_vNormals[LINE_CA].z * -1.f, 0.f, m_vNormals[LINE_CA].x);
	XMStoreFloat3(&m_vNormals[LINE_CA], XMVector3Normalize(XMLoadFloat3(&vNormal)));

#pragma region ����� ������
	_vector vAB = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	_vector vBC = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);

	_vector vCross = XMVector3Cross(vAB, vBC);

	_float3 vNormalFloat;
	XMStoreFloat3(&vNormalFloat, vCross);

	_float A = vNormalFloat.x;
	_float B = vNormalFloat.y;
	_float C = vNormalFloat.z;

	_float D = - (A * m_vPoints[POINT_A].x + B * m_vPoints[POINT_A].y + C * m_vPoints[POINT_A].z);

	XMStoreFloat4(&m_vPlane, XMVectorSet(A, B, C, D));
#pragma endregion ����� ������

#ifdef _DEBUG
	
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOS);
	m_iNumVertices = 3;

	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = 4;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma region VERTEX_BUFFER
	/* �������۸� �����.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	pVertices[0].vPosition = _pPoints[0];
	pVertices[1].vPosition = _pPoints[1];
	pVertices[2].vPosition = _pPoints[2];

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion VERTEX_BUFFER

#pragma region INDEX_BUFFER
	/* �ε������۸� �����.*/
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
	pIndices[3] = 0;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion INDEX_BUFFER

#endif

	return S_OK;
}

_int CCell::PreUpdate(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::PreUpdate(_fTimeDelta);
}

_int CCell::Update(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::Update(_fTimeDelta);
}

_int CCell::LateUpdate(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::LateUpdate(_fTimeDelta);
}

#ifdef _DEBUG

void CCell::Render()
{
	BindBuffers();

	CPrimitiveComponent::Render();
}

#endif

void CCell::EndPlay()
{
	CPrimitiveComponent::EndPlay();
}

void CCell::Release()
{
	CPrimitiveComponent::Release();
}

shared_ptr<CCell> CCell::Create(const _float3* _pPoints, _uint _iCellIndex)
{
	shared_ptr<CCell> spCell = make_shared<CCell>();

	if (FAILED(spCell->Initialize(_pPoints, _iCellIndex)))
	{
		MSG_BOX("Failed to Create Cell : CCell");
		spCell->Release();
		spCell.reset();
	}

	return spCell;
}

_bool CCell::ComparePoints(const _float3& _vSourPoint, const _float3& _vDestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vDestPoint)))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(&_vSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&_vDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(&_vDestPoint)))
			return true;
	}

	return false;
}

_bool CCell::IsIn_Initialize(_fvector _vActorLocalPosition, _float* _pPlaneY)
{
	_uint iIsInCount = 0;

	// ���� ���ε��� ��ȸ�ϸ鼭, 
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector vActorPosRemoveY = XMVectorSetY(_vActorLocalPosition, 0.0f);

		_vector	vDir = XMVector3Normalize(vActorPosRemoveY - XMLoadFloat3(&m_vPoints[i]));

		// ������ �������� ��->�÷��̾� ���͸� ������ ����� 0���� �۰ų� ���ٸ�
		if (0 >= XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vNormals[i]), vDir)))
		{
			// ���ʹ� �� ���� ���ʿ� �ֽ��ϴ�.
			iIsInCount++;
		}
		// �� �����̶� ���ʿ� ���� �ʴٸ�, ���ʹ� �� ���� �������� �ʽ��ϴ�.
		else
			return false;
	}

	if (iIsInCount < 3)
		return false;

	// �� ������ ���ʿ� �ִٸ�, ���ʹ� �� ���� ���ʿ� �ֽ��ϴ�.

	_float3 vActorPos;
	XMStoreFloat3(&vActorPos, _vActorLocalPosition);

	*_pPlaneY = -(m_vPlane.x * vActorPos.x + m_vPlane.z * vActorPos.z + m_vPlane.w) / m_vPlane.y;

	return true;
}

_bool CCell::IsIn(_fvector _vActorLocalPosition, _float* _pPlaneY, _int* _pNeighborIndex)
{
	// ���� ���ε��� ��ȸ�ϸ鼭, 
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector vActorPosRemoveY = XMVectorSetY(_vActorLocalPosition, 0.0f);

		_vector	vDir = XMVector3Normalize(vActorPosRemoveY - XMLoadFloat3(&m_vPoints[i]));

		// ������ �������� ��->�÷��̾� ���͸� ������ ����� 0���� ũ�ٸ�
		if (0 < XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_vNormals[i]), vDir)))
		{
			// ���Ͱ� �ش� �� ���� �����ٰ� �Ǵ��ϰ�, false�� �����Ѵ�.
			*_pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}
	}

	_float3 vActorPos;
	XMStoreFloat3(&vActorPos, _vActorLocalPosition);

	// ���Ͱ� �ȿ� �ִٸ�, ����� �̿��� _pPlaneY�� ���ؼ� �Ű������� �ּҷ� ���ش�. -> �� ���� ���� ������ Y���� ������Ų��.
	// ax + by + cz + d = 0;
	// by = - ax - cz - d;
	// y = - ( ax + cz + d ) / b;

	*_pPlaneY = - (m_vPlane.x * vActorPos.x + m_vPlane.z * vActorPos.z + m_vPlane.w) / m_vPlane.y;

	return true;
}

_float CCell::GetPlaneY(_float _fActorX, _float _fActorZ)
{
	_float fPlaneY;
	fPlaneY = - (m_vPlane.x * _fActorX + m_vPlane.z * _fActorZ + m_vPlane.w) / m_vPlane.y;

	return fPlaneY;
}
