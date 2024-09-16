#include "Engine_pch.h"
#include "ColliderComponent.h"

#include "DeviceManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"

#include "Bounding.h"
#include "Bounding_Sphere.h"
#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

void CCollider::Initialize()
{
	// 프리미티브 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	switch (m_eColliderType)
	{
	case COLLIDER_TYPE::AABB:
		m_pBounding = CBounding_AABB::Create(m_vCenter, m_vExtents);
		break;

	case COLLIDER_TYPE::OBB:
		m_pBounding = CBounding_OBB::Create(m_vCenter, m_vExtents, m_vRadians);
		break;

	case COLLIDER_TYPE::SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_vCenter, m_fRadius);
		break;

	default:
		MSG_BOX("콜라이더 타입을 지정해라");
		break;
	}

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pDeviceContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iByteCodeLength, &m_pInputLayout)))
	{
		MSG_BOX("Failed to Create InputLayout : CColliderComponent");
		return;
	}

	CPrimitiveComponent::Initialize();
}

_int CCollider::PreUpdate(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::PreUpdate(_fTimeDelta);
}

_int CCollider::Update(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::Update(_fTimeDelta);
}

_int CCollider::LateUpdate(const _float& _fTimeDelta)
{
	m_pBounding->Update(XMLoadFloat4x4(&m_matOwnerWorld));

	return CPrimitiveComponent::LateUpdate(_fTimeDelta);
}

void CCollider::Render()
{
	if (!m_bRender)
		return;

	m_pBounding->SetIsCollideOn(m_bCollideOn);

	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);

	BindShaderResource(m_pEffect);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();
	// m_pBounding->Render(m_pBatch);
	m_pBatch->End();

	// CPrimitiveComponent::Render();
}

void CCollider::EndPlay()
{
	CPrimitiveComponent::EndPlay();
}

void CCollider::Release()
{
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);

	Safe_Release(m_pInputLayout);

	m_usetColliders.clear();

	if (nullptr != m_pBounding)
	{
		m_pBounding->Release();
		m_pBounding.reset();
	}

	CPrimitiveComponent::Release();
}

void CCollider::BindShaderResource(BasicEffect* _pEffect)
{
	if (nullptr == _pEffect)
		return;

	_float4x4 ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	// 어짜피 Bounding들의 함수호출로 변환이 된 상태로 던질 것이기 때문에, WorldMatrix는 항등으로 둔다.
	_pEffect->SetWorld(XMMatrixIdentity());
	_pEffect->SetView(XMLoadFloat4x4(&ViewMatrix));
	_pEffect->SetProjection(XMLoadFloat4x4(&ProjMatrix));
}

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
	// 매개변수 - [ 내 콜라이더 타입, 상대 콜라이더의 바운딩 ]
	return m_pBounding->Intersect(pTargetCollider->m_eColliderType, pTargetCollider->m_pBounding);
}
