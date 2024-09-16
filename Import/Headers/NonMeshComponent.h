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
	// ���̴��� �׸��ٴ� ���� �˸��ϴ�.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// ���̴��� ���ҽ��� ���ε��մϴ�.
	HRESULT BindShaderResource(const _char* _pConstantName, _uint _iIndex);
	HRESULT BindShaderResources(const _char* _pConstantName);

	// �ο����� ���̴��� ���ε��մϴ�.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// ����� ���̴��� ���ε��մϴ�.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

protected:
	// 12.28 �� ������ �������� ���� ��ǥ�� �����մϴ�.
	void UpdateVerticesTransform();

protected:
	// ���۸� �����մϴ�.
	virtual HRESULT InitializeBuffer() = 0;

	// �ؽ�ó�� �����մϴ�.
	virtual HRESULT CreateTexture();

	// ���̴��� �����մϴ�.
	HRESULT CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

/********************
	Getter/Setter
********************/
public:
	void SetTextureFileInfo(const wstring& _wstrTextureFileName, _uint _iBeginIndex, _uint _iEndIndex);

	void SetShaderFileInfo(const wstring& _wstrShaderFilePath);

	void SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

	// 12.28 ���������� ���� ��ġ �������� -> ���콺 ��ŷ�� ���� �߰�
	_vector GetVertexPos(_uint _iIndex) { return XMLoadFloat4(&m_vecVertexPos[_iIndex]); }
	_float3 GetVertexIndices(_uint _iTriangleIndex) { return m_vecVertexIndices[_iTriangleIndex]; }

	_uint GetNumTriangles() { return m_iNumTriangles; }

/********************
	Data Members
********************/
protected:
/////////////
/* �ؽ�ó */
/////////////
	// �ؽ�ó ���� �̸�
	wstring m_wstrTextureFileName = L"";

	// �ؽ�ó�� ��� ����
	vector<ID3D11ShaderResourceView*> m_vecSRV;

	// �ؽ�ó ���� ���� �ε���
	_uint m_iBeginIndex = 0;

	// �ؽ�ó ���� �� �ε���
	_uint m_iEndIndex = 0;

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

protected:
	// 12.28 ���� �浹�� ���� �߰�
	vector<_float4> m_vecVertexPos; // Primitive�� ������ ������ǥ��

	vector<_float4> m_vecVertexWorldPos; // Primitive�� ������ ������ǥ��

	vector<_float3> m_vecVertexIndices;// �������� �ε��� ������ ������ ����(���۴� �ֹߵǱ� ������ �ϳ� �� ����)

	_uint m_iNumTriangles = 0; // �ﰢ�� ����
};

END