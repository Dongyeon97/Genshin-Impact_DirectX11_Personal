#pragma once
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_OBB final : public CBounding
{
public:
	explicit CBounding_OBB() = default;
	virtual ~CBounding_OBB() = default;

/********************
	Framework
********************/
public:
	virtual HRESULT Initialize(_float3 _vCenter, _float3 _vExtents, _float3 _vRadians);
	virtual _int Update(_fmatrix _matOwnerWorld) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual void Release() override;

public:
	/* vRadians -> x : x축 기준 회전 각도, y : y축 기준 회전 각도, z : z축 기준 회전 각도 */
	static shared_ptr<CBounding_OBB> Create(_float3 _vCenter, _float3 _vExtents, _float3 _vRadians);

/********************
	Methods
********************/
public:
	virtual _bool Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding) override;

/********************
	Getter / Setter
********************/
public:
	virtual void* GetBounding() override { return m_pOBB; }

/********************
	Data Members
********************/
protected:
	BoundingOrientedBox* m_pOriginalOBB = { nullptr };
	BoundingOrientedBox* m_pOBB = { nullptr };
};

END