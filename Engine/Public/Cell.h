#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CPrimitiveComponent
{
public:
	enum POINTS { POINT_A, POINT_B, POINT_C, POINT_END };	// 셀의 세 점
	enum LINES { LINE_AB, LINE_BC, LINE_CA, LINE_END };		// 선(시계 방향)

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
	// [ 점과 점이 같은지 비교]
	_bool ComparePoints(const _float3& _vSourPoint, const _float3& _vDestPoint);

	_bool IsIn_Initialize(_fvector _vActorLocalPosition, _float* _pPlaneY);

	// [ 액터가 이 셀 안에 있나요 ]
	_bool IsIn(_fvector _vActorLocalPosition, _float* _pPlaneY, _int* _pNeighborIndex);

	// [ 자신의 평방에 플레이어의 x,z 를 대입하여 y를 뱉어줌 ]
	_float GetPlaneY(_float _fActorX, _float _fActorZ);
/********************
	Getter / Setter
********************/
public:
	// [ _ePoint에 속하는 점을 반환 ]
	_float3 GetPoint(POINTS _ePoint) { return m_vPoints[_ePoint]; }

	// [ 인덱스 반환 ]
	_uint GetCellIndex() { return m_iIndex; }

	// [ 자신의 _eLine 직선에 _spNeighbor를 이웃으로 등록(인덱스로 등록) ]
	void SetNeighbor(LINES _eLine, shared_ptr<CCell> _spNeighbor) { m_iNeighborIndices[_eLine] = _spNeighbor->m_iIndex; }

/********************
	Data Members
********************/
private:
	_float3					m_vPoints[POINT_END];	// 셀의 세 점
	_float4					m_vPlane;				// 셀의 평면의 방정식
	_uint					m_iIndex = { 0 };		// 셀의 인덱스
	_float3					m_vNormals[LINE_END];	// 선의 법선벡터

	_int					m_iNeighborIndices[3] = { -1, -1, -1 };	// 

};

END