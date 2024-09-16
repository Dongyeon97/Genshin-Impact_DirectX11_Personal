#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class CMesh;

class ENGINE_DLL CModelComponent : public CPrimitiveComponent
{
public:
	explicit CModelComponent() = default;
	virtual ~CModelComponent() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	void Render(_uint _iMeshIndex);
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
public:
	// 셰이더로 그린다는 것을 알립니다.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// 로우밸류를 셰이더에 바인딩합니다.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// 행렬을 셰이더에 바인딩합니다.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

	// 재질을 셰이더에 바인딩합니다.
	HRESULT BindMaterialShaderResource (_uint _iMeshIndex, TEXTURE_TYPE _eMaterialType, const _char* _pConstantName);

private:
	// 셰이더를 세팅합니다.
	HRESULT CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

protected:
	// 모델의 구성 요소들을 준비합니다.
	virtual HRESULT ReadyMeshes() = 0;
	virtual HRESULT ReadyMaterials() = 0;

/********************
	Getter / Setter
********************/
public:
	void SetModelData(MODEL_DATA _ModelData) { m_ModelData = _ModelData; }
	void SetSpawnData(SPAWN_DATA _SpawnData) { m_SpawnData = _SpawnData; }

public:
	void SetModelFileInfo(MODEL_TYPE _eModelType, const wstring& _wstrModelFileName, _fmatrix PivotMatrix);
	void SetModelFileName(const wstring& _wstrModelFileName) { m_wstrModelFileName = _wstrModelFileName; }

	void SetShaderFileInfo(const wstring& _wstrShaderFilePath);

	void SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

	void SetAdditionalPivot(_matrix _matAdditionalPivot) { XMStoreFloat4x4(&m_AdditionalPivot, _matAdditionalPivot); }
	_float4x4 GetAdditionalPivot() { return m_AdditionalPivot; }

	_uint GetNumMeshes() { return m_iNumMeshes; }
	vector<shared_ptr<CMesh>>& GetVecMeshs() { return m_vecMeshes; }

/********************
	Data Members
********************/
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

/////////////
/* 모델 */
/////////////
protected:
	// [ 모델(FBX)파일 이름 ]
	wstring m_wstrModelFileName = L"";

	// [ 모델 데이터 ]
	MODEL_DATA m_ModelData;

	// [ 스폰 데이터 ]
	SPAWN_DATA m_SpawnData;

protected:
	// [ 모델 타입 ]
	MODEL_TYPE m_eModelType = MODEL_TYPE::MODEL_NONANIM;

	// [ 모델 생성시 변환을 적용할 행렬 ]
	_float4x4 m_PivotMatrix = {};
	_float4x4 m_AdditionalPivot = {};

	// [ 메시 개수 ]
	_uint m_iNumMeshes = 0;

	// [ 메시 ]
	vector<shared_ptr<CMesh>> m_vecMeshes;

	// [ 재질 개수 ]
	_uint m_iNumMaterials = 0;

	// [ 재질 텍스처 ]
	vector<vector<ID3D11ShaderResourceView*>> m_vecMaterials;

	// [ 재질이 가지고 있는 텍스처의 비트 플래그 ]
	vector<_ubyte> m_vecTextureBitFlags;
};

END