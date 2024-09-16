#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CNonMeshComponent : public CPrimitiveComponent
{
public:
	explicit CNonMeshComponent() = default;
	virtual ~CNonMeshComponent() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	// 셰이더로 그린다는 것을 알립니다.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// 셰이더에 리소스를 바인딩합니다.
	HRESULT BindShaderResource(const _char* _pConstantName, _uint _iIndex);
	HRESULT BindShaderResources(const _char* _pConstantName);

	// 로우밸류를 셰이더에 바인딩합니다.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// 행렬을 셰이더에 바인딩합니다.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

protected:
	// 12.28 매 프레임 정점들의 월드 좌표를 갱신합니다.
	void UpdateVerticesTransform();

protected:
	// 버퍼를 세팅합니다.
	virtual HRESULT InitializeBuffer() = 0;

	// 텍스처를 세팅합니다.
	virtual HRESULT CreateTexture();

	// 셰이더를 세팅합니다.
	HRESULT CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

/********************
	Getter/Setter
********************/
public:
	void SetTextureFileInfo(const wstring& _wstrTextureFileName, _uint _iBeginIndex, _uint _iEndIndex);

	void SetShaderFileInfo(const wstring& _wstrShaderFilePath);

	void SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

	// 12.28 정적버퍼의 정점 위치 가져오기 -> 마우스 피킹을 위해 추가
	_vector GetVertexPos(_uint _iIndex) { return XMLoadFloat4(&m_vecVertexPos[_iIndex]); }
	_float3 GetVertexIndices(_uint _iTriangleIndex) { return m_vecVertexIndices[_iTriangleIndex]; }

	_uint GetNumTriangles() { return m_iNumTriangles; }

/********************
	Data Members
********************/
protected:
/////////////
/* 텍스처 */
/////////////
	// 텍스처 파일 이름
	wstring m_wstrTextureFileName = L"";

	// 텍스처를 담는 벡터
	vector<ID3D11ShaderResourceView*> m_vecSRV;

	// 텍스처 파일 시작 인덱스
	_uint m_iBeginIndex = 0;

	// 텍스처 파일 끝 인덱스
	_uint m_iEndIndex = 0;

/////////////
/* 셰이더 */
/////////////
protected:
	// [ 이펙트 컴객체 ]
	ID3DX11Effect* m_pEffect = nullptr;

	// [ 인풋 레이아웃 ]
	vector<ID3D11InputLayout*> m_vecInputLayouts;

	// [ 셰이더 파일 이름 ]
	wstring m_wstrShaderFileName = L"";

	// [ 정점 요소 ]
	const D3D11_INPUT_ELEMENT_DESC* m_pElements = nullptr;

	// [ 정점 요소 개수 ]
	_uint m_iNumElements = 0;

protected:
	// 12.28 광선 충돌을 위해 추가
	vector<_float4> m_vecVertexPos; // Primitive의 정점의 로컬좌표들

	vector<_float4> m_vecVertexWorldPos; // Primitive의 정점의 월드좌표들

	vector<_float3> m_vecVertexIndices;// 정점들의 인덱스 순서를 영구히 보관(버퍼는 휘발되기 때문에 하나 더 만듦)

	_uint m_iNumTriangles = 0; // 삼각형 개수
};

END