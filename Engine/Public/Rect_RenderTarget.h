#pragma once
#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CRect_RenderTarget final : public CPrimitiveComponent
{
public:
	explicit CRect_RenderTarget() = default;
	virtual ~CRect_RenderTarget() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

	static CRect_RenderTarget* Create();

/********************
	Methods
********************/
public:
	// 셰이더로 그린다는 것을 알립니다.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// 텍스처를 셰이더에 바인딩합니다.(렌더타겟 전용)
	HRESULT BindShaderResource(const _char* _pConstantName, ID3D11ShaderResourceView* pSRV);

	// 로우밸류를 셰이더에 바인딩합니다.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// 행렬을 셰이더에 바인딩합니다.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

protected:
	// 버퍼를 세팅합니다.
	HRESULT InitializeBuffer();

	// 셰이더를 세팅합니다.
	HRESULT CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

/********************
	Getter/Setter
********************/
public:
	void SetShaderFileInfo(const wstring& _wstrShaderFilePath);
	void SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

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
};

END