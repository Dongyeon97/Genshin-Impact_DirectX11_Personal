#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CPrimitiveComponent
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };	// ���� �� ��
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };		// ��(�ð� ����)

public:
	explicit CCell() = default;
	virtual ~CCell() = default;

public:
	static shared_ptr<CCell> Create(const _float3* _pPoints, _uint _iCellIndex);
/********************
	Framework
********************/
public:
	HRESULT Initialize(const _float3* _pPoints, _uint iCellIndex);
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void EndPlay() override;
	virtual void Release() override;

#ifdef _DEBUG
	virtual void Render() override;
#endif

/********************
	Methods
********************/
public:
	// [ ���� ���� ������ ��]
	_bool ComparePoints(const _float3& _vSourPoint, const _float3& _vDestPoint);

	_bool IsIn_Initialize(_fvector _vActorLocalPosition, _float* _pPlaneY);

	// [ ���Ͱ� �� �� �ȿ� �ֳ��� ]
	_bool IsIn(_fvector _vActorLocalPosition, _float* _pPlaneY, _int* _pNeighborIndex);

	// [ �ڽ��� ��濡 �÷��̾��� x,z �� �����Ͽ� y�� ����� ]
	_float GetPlaneY(_float _fActorX, _float _fActorZ);
/********************
	Getter / Setter
********************/
public:
	// [ _ePoint�� ���ϴ� ���� ��ȯ ]
	_float3 GetPoint(POINTS _ePoint) { return m_vPoints[_ePoint]; }

	// [ �ε��� ��ȯ ]
	_uint GetCellIndex() { return m_iIndex; }

	// [ �ڽ��� _eLine ������ _spNeighbor�� �̿����� ���(�ε����� ���) ]
	void SetNeighbor(LINES _eLine, shared_ptr<CCell> _spNeighbor) { m_iNeighborIndices[_eLine] = _spNeighbor->m_iIndex; }

/********************
	Data Members
********************/
private:
	_float3					m_vPoints[POINT_END];	// ���� �� ��
	_float4					m_vPlane;				// ���� ����� ������
	_uint					m_iIndex = { 0 };		// ���� �ε���
	_float3					m_vNormals[LINE_END];	// ���� ��������

	_int					m_iNeighborIndices[3] = { -1, -1, -1 };	// 

};

END