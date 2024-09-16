#include "Engine_pch.h"
#include "Navigation.h"

#include "DeviceManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

#include "Actor.h"

#include "Cell.h"

_float4x4 CNavigation::m_matTerrainWorld = {};

void CNavigation::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	if (FAILED(CreateShader(m_wstrShaderFileName, m_pElements, m_iNumElements)))
	{
		MSG_BOX("Failed to CreateShader : CNavigation");
		return;
	}

	XMStoreFloat4x4(&m_matTerrainWorld, XMMatrixIdentity());

	// 1. ���ҽ� �Ŵ������� ������ ������ ����ִ� ��θ� �����´�.
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();
	wstring wstrNaviFilePath = wstrDataPath + L"Navigation\\";

	// 2. ������ �д´�.
	ifstream istrReadFile(wstrNaviFilePath + m_wstrDataFileName + L"_Navigation.txt", ios::in | ios::binary);

	// 3. ���� ó��
	if (!istrReadFile.is_open())
	{
		MSG_BOX("Failed to Open Navigation file : CNavigation");
		return;
	}

	while (!istrReadFile.eof())
	{
		if (istrReadFile.eof())
			break;

		_float3		vPoints[CCell::POINT_END];

		istrReadFile.read((char*)(&vPoints), sizeof(_float3) * CCell::POINT_END);

		shared_ptr<CCell> spCell = CCell::Create(vPoints, (_uint)m_vecCells.size());
		if (nullptr == spCell)
			return;

		m_vecCells.push_back(spCell);
	}

	RemoveLatestCell();

	if (FAILED(MakeNeighbors()))
		return;
}

void CNavigation::Update(_fmatrix _matTerrainWorldMatrix)
{
	XMStoreFloat4x4(&m_matTerrainWorld, _matTerrainWorldMatrix);
}

void CNavigation::Render()
{
	if (!m_bRender)
		return;

	BindShaderResources();

	BeginShader(0);	// 0�� �н��ۿ� ���� ������ 0���� ����

	/*if (-1 == m_iCurrentIndex)
	{
		for (auto& spCell : m_vecCells)
		{
			if (nullptr != spCell)
				spCell->Render();
		}
	}
	else
		m_vecCells[m_iCurrentIndex]->Render();*/

	for (auto& spCell : m_vecCells)
	{
		if (nullptr != spCell)
			spCell->Render();
	}

	return;
}


void CNavigation::Release()
{
	for (auto& spCell : m_vecCells)
	{
		spCell->Release();
		spCell.reset();
	}
	m_vecCells.clear();

	CNonMeshComponent::Release();
}


HRESULT CNavigation::BindShaderResources()
{
	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	// Terrain ��ü�� �����Ʈ������ ������.
	_float4x4	WorldMatrix = m_matTerrainWorld;

	_float4		vColor = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(BindRawValue("g_vColor", &vColor, sizeof(_float4))))
	{
		MSG_BOX("Failed to Bind vColor : CNavigation");
		return E_FAIL;
	}

	if (FAILED(BindMatrix("g_WorldMatrix", &m_matTerrainWorld)))
	{
		MSG_BOX("Failed to Bind WorldMatrix : CNavigation");
		return E_FAIL;
	}

	if (FAILED(BindMatrix("g_ViewMatrix", &ViewMatrix)))
	{
		MSG_BOX("Failed to Bind ViewMatrix : CNavigation");
		return E_FAIL;
	}

	if (FAILED(BindMatrix("g_ProjMatrix", &ProjMatrix)))
	{
		MSG_BOX("Failed to Bind ProjMatrix : CNavigation");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CNavigation::MakeNeighbors()
{
	for (auto& pSourCell : m_vecCells)
	{
		for (auto& pDestCell : m_vecCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_A), pSourCell->GetPoint(CCell::POINT_B)))
				pSourCell->SetNeighbor(CCell::LINE_AB, pDestCell);
			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_B), pSourCell->GetPoint(CCell::POINT_C)))
				pSourCell->SetNeighbor(CCell::LINE_BC, pDestCell);
			if (true == pDestCell->ComparePoints(pSourCell->GetPoint(CCell::POINT_C), pSourCell->GetPoint(CCell::POINT_A)))
				pSourCell->SetNeighbor(CCell::LINE_CA, pDestCell);
		}
	}
	return S_OK;
}

HRESULT CNavigation::AddCell(const _float3* _vPoints)
{
	// �׺���̼ǿ� ���� �߰��ϱ� ����, ���� ������ ������ ������ ������ ��ġ��Ű�� ���� �۾��� �ʿ��ϴ�.

	shared_ptr<CCell> spCell = CCell::Create(_vPoints, (_uint)m_vecCells.size());
	if (nullptr == spCell)
		return E_FAIL;

	m_vecCells.push_back(spCell);

	MakeNeighbors();

	return S_OK;
}

HRESULT CNavigation::RemoveLatestCell()
{
	if (!m_vecCells.empty())
	{
		m_vecCells.back()->Release();
		m_vecCells.back().reset();

		auto iter = m_vecCells.end() - 1;
		m_vecCells.erase(iter);
	}

	return S_OK;
}

_uint CNavigation::InitializeActorCellIndex(_vector _vActorPos)
{
	_float fPlaneY = 0.0f;

	for (auto& Cell : m_vecCells)
	{
		if (Cell->IsIn_Initialize(_vActorPos, &fPlaneY))
			return Cell->GetCellIndex();
	}

	// ���� Ž������ �ʾҴٸ�, -1�� ��ȯ���� ���� �߻�?
	return -1;
}

_bool CNavigation::IsMove(_fvector _vPosition, _int* _iCurNaviCellIndex, _float* _pPlaneY)
{
	_int		iNeighborIndex = { -1 };

	/* ���� �����ϰ� �ִ� �� �ȿ��� ��������. */
	if (true == m_vecCells[*_iCurNaviCellIndex]->IsIn(_vPosition, _pPlaneY, &iNeighborIndex))
		return true;

	/* ���� ���� ������. */
	else
	{
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_vecCells[iNeighborIndex]->IsIn(_vPosition, _pPlaneY, &iNeighborIndex))
				{
					*_iCurNaviCellIndex = iNeighborIndex;
					return true;
				}
			}
		}

		else
			return false;
	}
}
