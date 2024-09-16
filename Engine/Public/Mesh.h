#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class CModelComponent;
class CBone;

class ENGINE_DLL CMesh final : public CPrimitiveComponent
{
public:
	explicit CMesh() = default;
	virtual ~CMesh() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix);
	virtual void Release() override;

/********************
	Methods
********************/
public:
	static shared_ptr<CMesh> Create(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix);

public:
	HRESULT InitializeBuffer(MODEL_TYPE _eModelType, const MY_MESH* pAIMesh, shared_ptr<CModelComponent> spModel, _fmatrix PivotMatrix);

public:
	HRESULT ReadyVertexBuffer_NonAnim(const MY_MESH* _pAIMesh, _fmatrix _matPivot);
	HRESULT ReadyVertexBuffer_Anim(const MY_MESH* _pAIMesh, shared_ptr<CModelComponent> spModel);

public:
	void SetUpBoneMatrices(_float4x4* _pBoneMatrices, vector<shared_ptr<CBone>>& _vecBones);

/********************
	Getter / Setter
********************/
public:
	_uint GetMaterialIndex() const { return m_iMaterialIndex; }

	_uint GetNumTriangles() { return m_iNumTriangles; }
	_vector GetVertexPos(_uint _iIndex) { return XMLoadFloat3(&m_vecVertexPos[_iIndex]); }
	_float3 GetVertexIndices(_uint _iTriangleIndex) { return m_vecVertexIndices[_iTriangleIndex]; }

	vector<_float3>& GetVecVertexPos() { return m_vecVertexPos; }

/********************
	Data Members
********************/
private:
	/* 메시 이름 */
	_char m_szName[MAX_PATH] = "";

	/* 이 메시가 사용하는 재질의 인덱스 */
	_uint m_iMaterialIndex = 0;

private:
	/* 메시별로 이용하고 있는 뼈들. */
	_uint m_iNumBones = { 0 };

	/* 메시에 영향을 주는 뼈가 전체 뼈중에서 몇번째 뼈인지 */
	vector<_uint> m_vecBoneIndices;

	/* 메시에 영향을 주는 n번 째 뼈의 OffsetMatrix */
	vector<_float4x4> m_vecOffsetMatrices;

private:
	vector<_float3> m_vecVertexPos;		// 메시의 정점의 로컬좌표들
	vector<_float3> m_vecVertexIndices; // 메시의 정점의 인덱스
	_uint m_iNumTriangles = 0;			// 메시의 삼각형 개수
};

END