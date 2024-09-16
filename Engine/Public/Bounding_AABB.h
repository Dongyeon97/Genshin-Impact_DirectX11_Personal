#pragma once
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_AABB final : public CBounding
{
public:
	explicit CBounding_AABB() = default;
	virtual ~CBounding_AABB() = default;

/********************
	Framework
********************/
public:
	virtual HRESULT Initialize(_float3 _vCenter, _float3 _vExtents);
	virtual _int Update(_fmatrix _matOwnerWorld) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual void Release() override;

public:
	static shared_ptr<CBounding_AABB> Create(_float3 _vCenter, _float3 _fExtents);

/********************
	Methods
********************/
public:
	virtual _bool Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding) override;

/********************
	Getter / Setter
********************/
public:
	virtual void* GetBounding() override { return m_pAABB; }

/********************
	Data Members
********************/
protected:
	BoundingBox* m_pOriginalAABB = { nullptr };
	BoundingBox* m_pAABB = { nullptr };
};

END