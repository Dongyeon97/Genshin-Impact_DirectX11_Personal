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
	// ���̴��� �׸��ٴ� ���� �˸��ϴ�.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// �ؽ�ó�� ���̴��� ���ε��մϴ�.(����Ÿ�� ����)
	HRESULT BindShaderResource(const _char* _pConstantName, ID3D11ShaderResourceView* pSRV);

	// �ο����� ���̴��� ���ε��մϴ�.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// ����� ���̴��� ���ε��մϴ�.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

protected:
	// ���۸� �����մϴ�.
	HRESULT InitializeBuffer();

	// ���̴��� �����մϴ�.
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
/* ���̴� */
/////////////
protected:
	// [ ����Ʈ �İ�ü ]
	ID3DX11Effect* m_pEffect = nullptr;

	// [ ��ǲ ���̾ƿ� ]
	vector<ID3D11InputLayout*> m_vecInputLayouts;

	// [ ���̴� ���� �̸� ]
	wstring m_wstrShaderFileName = L"";

	// [ ���� ��� ]
	const D3D11_INPUT_ELEMENT_DESC* m_pElements = nullptr;

	// [ ���� ��� ���� ]
	_uint m_iNumElements = 0;
};

END