#include "Engine_pch.h"
#include "Bounding_OBB.h"
#include "DebugDraw.h"

HRESULT CBounding_OBB::Initialize(_float3 _vCenter, _float3 _vExtents, _float3 _vRadians)
{
	/* 초기상태는 로컬상태 */
	_float4 vRotation;
	
	// x,y,z 회전을 쿼터니언 회전으로 바꿔서 저장.
	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(_vRadians.x, _vRadians.y, _vRadians.z));

	m_pOriginalOBB = new BoundingOrientedBox(_vCenter, _vExtents, vRotation);
	m_pOBB = new BoundingOrientedBox(*m_pOriginalOBB);

	return CBounding::Initialize();
}

_int CBounding_OBB::Update(_fmatrix _matOwnerWorld)
{
	m_pOriginalOBB->Transform(*m_pOBB, _matOwnerWorld);

	return CBounding::Update(_matOwnerWorld);
}

void CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (!m_isCollideOn)
	{
		DX::Draw(pBatch, *m_pOBB, XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		DX::Draw(pBatch, *m_pOBB, m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
	}
}

shared_ptr<CBounding_OBB> CBounding_OBB::Create(_float3 _vCenter, _float3 _vExtents, _float3 _vRadians)
{
	shared_ptr<CBounding_OBB> spInstance = make_shared<CBounding_OBB>();

	if (FAILED(spInstance->Initialize(_vCenter, _vExtents, _vRadians)))
	{
		MSG_BOX("Failed to Create : CBounding_OBB");
		spInstance->Release();
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

_bool CBounding_OBB::Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding)
{
	m_isColl = false;

	void* pTargetBounding = _spBounding->GetBounding();

	switch (_eColliderType)
	{
	case COLLIDER_TYPE::AABB:
		m_isColl = m_pOBB->Intersects(*(BoundingBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::OBB:
		m_isColl = m_pOBB->Intersects(*(BoundingOrientedBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::SPHERE:
		m_isColl = m_pOBB->Intersects(*(BoundingSphere*)pTargetBounding);
		break;
	}

	return m_isColl;
}

void CBounding_OBB::Release()
{
	Safe_Delete(m_pOriginalOBB);
	Safe_Delete(m_pOBB);

	CBounding::Release();
}