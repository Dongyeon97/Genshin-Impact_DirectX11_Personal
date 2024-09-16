#include "Engine_pch.h"
#include "Bounding_AABB.h"
#include "DebugDraw.h"

HRESULT CBounding_AABB::Initialize(_float3 _vCenter, _float3 _vExtents)
{
	/* 초기상태는 로컬상태 */
	m_pOriginalAABB = new BoundingBox(_vCenter, _vExtents);
	m_pAABB = new BoundingBox(*m_pOriginalAABB);

	return CBounding::Initialize();
}

_int CBounding_AABB::Update(_fmatrix _matOwnerWorld)
{
	_matrix		TransformMatrix = _matOwnerWorld;

	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]).m128_f32[0];
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]).m128_f32[0];
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]).m128_f32[0];

	m_pOriginalAABB->Transform(*m_pAABB, TransformMatrix);

	return CBounding::Update(_matOwnerWorld);
}

void CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (!m_isCollideOn)
	{
		DX::Draw(pBatch, *m_pAABB, XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		DX::Draw(pBatch, *m_pAABB, m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
	}
}

_bool CBounding_AABB::Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding)
{
	m_isColl = false;

	void* pTargetBounding = _spBounding->GetBounding();

	switch (_eColliderType)
	{
	case COLLIDER_TYPE::AABB:
		m_isColl = m_pAABB->Intersects(*(BoundingBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::OBB:
		m_isColl = m_pAABB->Intersects(*(BoundingOrientedBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::SPHERE:
		m_isColl = m_pAABB->Intersects(*(BoundingSphere*)pTargetBounding);
		break;
	}

	return m_isColl;
}

shared_ptr<CBounding_AABB> CBounding_AABB::Create(_float3 _vCenter, _float3 _vExtents)
{
	shared_ptr<CBounding_AABB> spInstance = make_shared<CBounding_AABB>();

	if (FAILED(spInstance->Initialize(_vCenter, _vExtents)))
	{
		MSG_BOX("Failed to Create : CBounding_Sphere");
		spInstance->Release();
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

void CBounding_AABB::Release()
{
	Safe_Delete(m_pOriginalAABB);
	Safe_Delete(m_pAABB);

	CBounding::Release();
}