#include "Engine_pch.h"
#include "PrimitiveComponent.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

void CPrimitiveComponent::Initialize()
{
	// 프리미티브 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// CreateShader(m_wstrShaderFileName, m_pElements, m_iNumElements); -> 01.05 NonMesh, Model로 각자 이전

	CSceneComponent::Initialize();
}

_int CPrimitiveComponent::PreUpdate(const _float& _fTimeDelta)
{
	return CSceneComponent::PreUpdate(_fTimeDelta);
}

_int CPrimitiveComponent::Update(const _float& _fTimeDelta)
{

	return CSceneComponent::Update(_fTimeDelta);
}

_int CPrimitiveComponent::LateUpdate(const _float& _fTimeDelta)
{

	return CSceneComponent::LateUpdate(_fTimeDelta);
}

void CPrimitiveComponent::Render()
{
	m_pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);

	CSceneComponent::Render();
}

void CPrimitiveComponent::EndPlay()
{

	CSceneComponent::EndPlay();
}

void CPrimitiveComponent::Release()
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();

	CSceneComponent::Release();
}

HRESULT CPrimitiveComponent::BindBuffers()
{
	// 버텍스 버퍼들
	ID3D11Buffer* pVertexBuffers[] = { m_pVertexBuffer, };

	// 버텍스 버퍼당 버텍스 한개의 바이트 사이즈 세팅
	_uint iVertexStrides[] = { m_iVertexStride, };

	// 버퍼의 몇번째 칸부터 시작할 것인지?
	_uint iOffsets[] = { 0, };

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CPrimitiveComponent::CreateBuffer(_Out_ ID3D11Buffer** _ppBuffer)
{
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, _ppBuffer);
}