#pragma once

BEGIN(Engine)

class ENGINE_DLL CBounding
{
public:
	explicit CBounding() = default;
	virtual ~CBounding() = default;

/********************
	Framework
********************/
public:
	virtual HRESULT Initialize();
	virtual _int Update(_fmatrix _matOwnerWorld) = 0;
	virtual void Render(PrimitiveBatch<VertexPositionColor>* pBatch) = 0;
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual _bool Intersect(COLLIDER_TYPE _eColliderType, shared_ptr<CBounding> _spBounding) = 0;

/********************
	Getter / Setter
********************/
public:
	virtual void* GetBounding() = 0;

	void SetIsCollideOn(_bool _bIsCollide) { m_isCollideOn = _bIsCollide; }
/********************
	Data Members
********************/
protected:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

protected:
	_bool m_isColl = false;
	_bool m_isCollideOn = false;
};

END