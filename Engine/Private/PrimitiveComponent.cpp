#include "Engine_pch.h"
#include "PrimitiveComponent.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

void CPrimitiveComponent::Initialize()
{
	// ������Ƽ�� ������Ʈ�� �ʱ�ȭ�ÿ� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// CreateShader(m_wstrShaderFileName, m_pElements, m_iNumElements); -> 01.05 NonMesh, Model�� ���� ����

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
	// ���ؽ� ���۵�
	ID3D11Buffer* pVertexBuffers[] = { m_pVertexBuffer, };

	// ���ؽ� ���۴� ���ؽ� �Ѱ��� ����Ʈ ������ ����
	_uint iVertexStrides[] = { m_iVertexStride, };

	// ������ ���° ĭ���� ������ ������?
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