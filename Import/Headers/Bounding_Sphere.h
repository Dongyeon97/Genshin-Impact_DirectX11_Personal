#pragma once
#include "Bounding.h"

BEGIN(Engine)

class ENGINE_DLL CBounding_Sphere final : public CBounding
{
public:
	explicit CBounding_Sphere() = default;
	virtual ~CBounding_Sphere() = default;

/********************
	Framework
********************/
public:
	virtual HRESULT Initialize(_float3 _vCenter, _float _fRadius);
	virtual _int Update(_fmatrix _matOwnerWorld) override;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
	virtual void Release() override;

public:
	static shared_ptr<CBounding_Sphere> Create(_float3 _vCenter, _float _fRadius);

/********************
	Methods
********************/
public:
	virtual _bool Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding) override;


/********************
	Getter / Setter
********************/
public:
	virtual void* GetBounding() override { return m_pSphere; };

/********************
	Data Members
********************/
protected:
	BoundingSphere* m_pOriginalSphere = nullptr;	// 초기값 구
	BoundingSphere* m_pSphere = nullptr;			// 실제 충돌처리에 사용할 구(매 프레임 업데이트)

};

END