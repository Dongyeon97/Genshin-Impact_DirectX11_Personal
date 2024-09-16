#include "Engine_pch.h"
#include "Instancing.h"

#include "DeviceManager.h"

#include "Actor.h"

void CInstancing::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	m_pSpeed = new _float[m_InstanceDesc.iNumInstance];
	m_pLifeTime = new _float[m_InstanceDesc.iNumInstance];
	m_pRotation = new _float3[m_InstanceDesc.iNumInstance];

	uniform_real_distribution<float>	SpeedRange(m_InstanceDesc.vSpeed.x, m_InstanceDesc.vSpeed.y);
	uniform_real_distribution<float>	TimeRange(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	uniform_real_distribution<float>	RotationRangeX(0.f, m_InstanceDesc.vRotate.x);
	uniform_real_distribution<float>	RotationRangeY(0.f, m_InstanceDesc.vRotate.y);
	uniform_real_distribution<float>	RotationRangeZ(0.f, m_InstanceDesc.vRotate.z);

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		m_pSpeed[i] = SpeedRange(m_RandomNumber);
		m_pLifeTime[i] = TimeRange(m_RandomNumber);

		m_pRotation[i].x = RotationRangeX(m_RandomNumber);
		m_pRotation[i].y = RotationRangeY(m_RandomNumber);
		m_pRotation[i].z = RotationRangeZ(m_RandomNumber);
	}

	// 만약 셰이더 파일과, 텍스처 파일이 세팅되어 있지 않은 경우
	if (m_wstrShaderFileName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_InstanceDesc.szShaderFileName[i] == '\0') { break; }
			m_wstrShaderFileName += m_InstanceDesc.szShaderFileName[i];
		}
	}
	if (m_wstrTextureFileName.empty())
	{
		for (_uint i = 0; i < 64; ++i)
		{
			if (m_InstanceDesc.szDiffuseTextureFileName[i] == '\0') { break; }
			m_wstrTextureFileName += m_InstanceDesc.szDiffuseTextureFileName[i];
		}
	}

	CNonMeshComponent::Initialize();
}

_int CInstancing::PreUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::PreUpdate(_fTimeDelta);
}

_int CInstancing::Update(const _float& _fTimeDelta)
{
	return CNonMeshComponent::Update(_fTimeDelta);
}

_int CInstancing::LateUpdate(const _float& _fTimeDelta)
{
	return CNonMeshComponent::LateUpdate(_fTimeDelta);
}

void CInstancing::Render()
{
	if (m_fTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	m_pDeviceContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_InstanceDesc.iNumInstance, 0, 0, 0);
}

HRESULT CInstancing::BindBuffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVertexBuffer,
		m_pVBInstance
	};

	_uint				iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

// 오브젝트 풀에서 재활용 할 시 호출되는 함수
void CInstancing::RecycleBegin()
{
	m_fTimeAcc = 0.f;
	m_isFinished = false;
	m_isPause = false;

	D3D11_MAPPED_SUBRESOURCE			SubResource{};

	uniform_real_distribution<float>	TimeRange(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	uniform_real_distribution<float>	RadiusRange(m_InstanceDesc.vRadius.x, m_InstanceDesc.vRadius.y);
	uniform_real_distribution<float>	WidthRange(m_InstanceDesc.vRange.x * -0.5f, m_InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	HeightRange(m_InstanceDesc.vRange.y * -0.5f, m_InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	DepthRange(m_InstanceDesc.vRange.z * -0.5f, m_InstanceDesc.vRange.z * 0.5f);

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		m_pLifeTime[i] = TimeRange(m_RandomNumber);

		((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 1.f;
		((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
			m_InstanceDesc.vCenter.x,
			m_InstanceDesc.vCenter.y,
			m_InstanceDesc.vCenter.z,
			1.0f);

		_vector vDir = XMVectorSet(WidthRange(m_RandomDevice), HeightRange(m_RandomDevice), DepthRange(m_RandomDevice), 0.f);

		// 이동 변환 적용
		XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation) + XMVector3Normalize(vDir) * RadiusRange(m_RandomNumber));
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CInstancing::Tick_Drop(const _float& _fTimeDelta)
{
	if (m_isPause)
		return;

	m_fTimeAcc += _fTimeDelta;

	if (m_fTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	if (m_InstanceDesc.fDuration <= m_fTimeAcc)
		m_isFinished = true;

	D3D11_MAPPED_SUBRESOURCE			SubResource{};

	uniform_real_distribution<float>	TimeRange(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	uniform_real_distribution<float>	WidthRange(m_InstanceDesc.vRange.x * -0.5f, m_InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	HeightRange(m_InstanceDesc.vRange.y * -0.5f, m_InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	DepthRange(m_InstanceDesc.vRange.z * -0.5f, m_InstanceDesc.vRange.z * 0.5f);

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		((VTXINSTANCE*)SubResource.pData)[i].vTranslation.y -= m_pSpeed[i] * _fTimeDelta;

		m_pLifeTime[i] -= _fTimeDelta;
		
		if (0.0f >= m_pLifeTime[i])
		{
			if (true == m_InstanceDesc.isLoop)
			{
				m_pLifeTime[i] = TimeRange(m_RandomNumber);
				((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
					m_InstanceDesc.vCenter.x + WidthRange(m_RandomNumber),
					m_InstanceDesc.vCenter.y + HeightRange(m_RandomNumber),
					m_InstanceDesc.vCenter.z + DepthRange(m_RandomNumber),
					1.f);
			}
			else
			{

				((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0.f;
			}
		}
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CInstancing::Tick_Spread(const _float& _fTimeDelta)
{
	if (m_isPause)
		return;

	m_fTimeAcc += _fTimeDelta;

	if (m_fTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	if (m_InstanceDesc.isLoop == false && m_InstanceDesc.fDuration <= m_fTimeAcc)
		m_isFinished = true;

	D3D11_MAPPED_SUBRESOURCE			SubResource{};

	uniform_real_distribution<float>	TimeRange(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	uniform_real_distribution<float>	RadiusRange(m_InstanceDesc.vRadius.x, m_InstanceDesc.vRadius.y);
	uniform_real_distribution<float>	WidthRange(m_InstanceDesc.vRange.x * -0.5f, m_InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	HeightRange(m_InstanceDesc.vRange.y * -0.5f, m_InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	DepthRange(m_InstanceDesc.vRange.z * -0.5f, m_InstanceDesc.vRange.z * 0.5f);

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

		_vector		vDir = XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_InstanceDesc.vPivot);
		vDir = XMVectorSetW(vDir, 0.f);

		// 이동 변환 적용
		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * _fTimeDelta);

		m_pLifeTime[i] -= _fTimeDelta;

		if (0.0f >= m_pLifeTime[i])
		{
			if (true == m_InstanceDesc.isLoop)
			{
				m_pLifeTime[i] = TimeRange(m_RandomNumber);
				((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
					m_InstanceDesc.vCenter.x + WidthRange(m_RandomNumber),
					m_InstanceDesc.vCenter.y + HeightRange(m_RandomNumber),
					m_InstanceDesc.vCenter.z + DepthRange(m_RandomNumber),
					1.f);

				_vector vDir;

				// 랜덤 방향벡터
				vDir = XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation) - XMLoadFloat3(&m_InstanceDesc.vPivot);
				vDir = XMVectorSetW(vDir, 0.f);

				// 이동 변환 적용
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation) + XMVector3Normalize(vDir));
			}
			else
			{
				// ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0.f;
			}
		}
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CInstancing::Tick_Spread_Radius(const _float& _fTimeDelta)
{
	if (m_isPause)
		return;

	m_fTimeAcc += _fTimeDelta;

	if (m_fTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	if (m_InstanceDesc.isLoop == false && m_InstanceDesc.fDuration <= m_fTimeAcc)
		m_isFinished = true;

	D3D11_MAPPED_SUBRESOURCE			SubResource{};

	uniform_real_distribution<float>	TimeRange(m_InstanceDesc.vLifeTime.x, m_InstanceDesc.vLifeTime.y);
	uniform_real_distribution<float>	RadiusRange(m_InstanceDesc.vRadius.x, m_InstanceDesc.vRadius.y);
	uniform_real_distribution<float>	WidthRange(m_InstanceDesc.vRange.x * -0.5f, m_InstanceDesc.vRange.x * 0.5f);
	uniform_real_distribution<float>	HeightRange(m_InstanceDesc.vRange.y * -0.5f, m_InstanceDesc.vRange.y * 0.5f);
	uniform_real_distribution<float>	DepthRange(m_InstanceDesc.vRange.z * -0.5f, m_InstanceDesc.vRange.z * 0.5f);

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	_matrix matWorld = m_wpOwnerActor.lock()->GetWorldMatrix();

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

		// 회전 변환 적용
		_vector vAxis = XMVectorSet(m_pRotation[i].x, m_pRotation[i].y, m_pRotation[i].z, 0.f);
		_matrix matRotation = XMMatrixIdentity();

		matRotation = matRotation * XMMatrixRotationX(XMConvertToRadians(m_pRotation[i].x * _fTimeDelta));
		matRotation = matRotation * XMMatrixRotationY(XMConvertToRadians(m_pRotation[i].y * _fTimeDelta));
		matRotation = matRotation * XMMatrixRotationZ(XMConvertToRadians(m_pRotation[i].z * _fTimeDelta));

		XMStoreFloat4(&pVertices[i].vRight, XMVector4Transform(XMLoadFloat4(&pVertices[i].vRight), matRotation));
		XMStoreFloat4(&pVertices[i].vUp, XMVector4Transform(XMLoadFloat4(&pVertices[i].vUp), matRotation));
		XMStoreFloat4(&pVertices[i].vLook, XMVector4Transform(XMLoadFloat4(&pVertices[i].vLook), matRotation));

		_vector		vDir = XMLoadFloat4(&pVertices[i].vTranslation) - XMLoadFloat3(&m_InstanceDesc.vPivot);
		vDir = XMVectorSetW(vDir, 0.f);

		// 이동 변환 적용
		XMStoreFloat4(&pVertices[i].vTranslation, XMLoadFloat4(&pVertices[i].vTranslation) + XMVector3Normalize(vDir) * m_pSpeed[i] * _fTimeDelta);

		m_pLifeTime[i] -= _fTimeDelta;

		if (0.0f >= m_pLifeTime[i])
		{
			if (true == m_InstanceDesc.isLoop)
			{
				m_pLifeTime[i] = TimeRange(m_RandomNumber);

				((VTXINSTANCE*)SubResource.pData)[i].vTranslation = _float4(
					m_InstanceDesc.vCenter.x,
					m_InstanceDesc.vCenter.y,
					m_InstanceDesc.vCenter.z,
					1.f);

				_vector vDir = XMVectorSet(WidthRange(m_RandomDevice), HeightRange(m_RandomDevice), DepthRange(m_RandomDevice), 0.f);

				// 이동 변환 적용
				XMStoreFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation, XMLoadFloat4(&((VTXINSTANCE*)SubResource.pData)[i].vTranslation) + XMVector3Normalize(vDir) * RadiusRange(m_RandomNumber));
			}
			else
			{
				// ((VTXINSTANCE*)SubResource.pData)[i].vColor.w = 0.f;
			}
		}
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

void CInstancing::Tick_Sprite(const _float& _fTimeDelta)
{
	if (m_isPause)
		return;

	m_fTimeAcc += _fTimeDelta;

	if (m_fTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	if (m_InstanceDesc.isLoop == false && m_InstanceDesc.fDuration <= m_fTimeAcc)
		m_isFinished = true;

	for (_uint i = 0; i < m_InstanceDesc.iNumInstance; ++i)
	{
		m_pLifeTime[i] -= _fTimeDelta;
	}
}

void CInstancing::EndPlay()
{
	CNonMeshComponent::EndPlay();
}

void CInstancing::Release()
{
	Safe_Delete_Array(m_pSpeed);
	Safe_Delete_Array(m_pLifeTime);
	Safe_Delete_Array(m_pRotation);

	Safe_Release(m_pVBInstance);

	CNonMeshComponent::Release();
}
