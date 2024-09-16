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
	// ���̴��� �׸��ٴ� ���� �˸��ϴ�.
	HRESULT BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex = 0);

	// �ο����� ���̴��� ���ε��մϴ�.
	HRESULT BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength);

	// ����� ���̴��� ���ε��մϴ�.
	HRESULT BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix);
	HRESULT BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices);

	// ������ ���̴��� ���ε��մϴ�.
	HRESULT BindMaterialShaderResource (_uint _iMeshIndex, TEXTURE_TYPE _eMaterialType, const _char* _pConstantName);

private:
	// ���̴��� �����մϴ�.
	HRESULT CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements);

protected:
	// ���� ���� ��ҵ��� �غ��մϴ�.
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

/////////////
/* �� */
/////////////
protected:
	// [ ��(FBX)���� �̸� ]
	wstring m_wstrModelFileName = L"";

	// [ �� ������ ]
	MODEL_DATA m_ModelData;

	// [ ���� ������ ]
	SPAWN_DATA m_SpawnData;

protected:
	// [ �� Ÿ�� ]
	MODEL_TYPE m_eModelType = MODEL_TYPE::MODEL_NONANIM;

	// [ �� ������ ��ȯ�� ������ ��� ]
	_float4x4 m_PivotMatrix = {};
	_float4x4 m_AdditionalPivot = {};

	// [ �޽� ���� ]
	_uint m_iNumMeshes = 0;

	// [ �޽� ]
	vector<shared_ptr<CMesh>> m_vecMeshes;

	// [ ���� ���� ]
	_uint m_iNumMaterials = 0;

	// [ ���� �ؽ�ó ]
	vector<vector<ID3D11ShaderResourceView*>> m_vecMaterials;

	// [ ������ ������ �ִ� �ؽ�ó�� ��Ʈ �÷��� ]
	vector<_ubyte> m_vecTextureBitFlags;
};

END