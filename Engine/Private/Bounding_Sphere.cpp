#include "Engine_pch.h"
#include "Bounding_Sphere.h"
#include "DebugDraw.h"

HRESULT CBounding_Sphere::Initialize(_float3 _vCenter, _float _fRadius)
{
	/* 초기상태는 로컬상태 */
	m_pOriginalSphere = new BoundingSphere(_vCenter, _fRadius);
	m_pSphere = new BoundingSphere(*m_pOriginalSphere);

	return CBounding::Initialize();
}

_int CBounding_Sphere::Update(_fmatrix _matOwnerWorld)
{
	// 매 프레임 충돌체 소유주의 월드행렬을 받아 업데이트한다.
	m_pOriginalSphere->Transform(*m_pSphere, _matOwnerWorld);

	return CBounding::Update(_matOwnerWorld);
}

void CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	if (!m_isCollideOn)
	{
		DX::Draw(pBatch, *m_pSphere, XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}
	else
	{
		DX::Draw(pBatch, *m_pSphere, m_isColl == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));
	}
}

shared_ptr<CBounding_Sphere> CBounding_Sphere::Create(_float3 _vCenter, _float _fRadius)
{
	shared_ptr<CBounding_Sphere> spInstance = make_shared<CBounding_Sphere>();

	if (FAILED(spInstance->Initialize(_vCenter, _fRadius)))
	{
		MSG_BOX("Failed to Create : CBounding_Sphere");
		spInstance->Release();
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

_bool CBounding_Sphere::Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding)
{
	m_isColl = false;

	void* pTargetBounding = _spBounding->GetBounding();

	switch (_eColliderType)
	{
	case COLLIDER_TYPE::AABB:
		m_isColl = m_pSphere->Intersects(*(BoundingBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::OBB:
		m_isColl = m_pSphere->Intersects(*(BoundingOrientedBox*)pTargetBounding);
		break;
	case COLLIDER_TYPE::SPHERE:
		m_isColl = m_pSphere->Intersects(*(BoundingSphere*)pTargetBounding);
		break;
	}

	return m_isColl;
}

void CBounding_Sphere::Release()
{
	Safe_Delete(m_pOriginalSphere);
	Safe_Delete(m_pSphere);

	CBounding::Release();
}