#pragma once
#include "SceneComponent.h"

/* PrimitiveComponents�� �Ϲ������� �浹 �����ͷ� �������ǰų�
���Ǵ� ������ ������ �����ϰų� �����ϴ� SceneComponent�Դϴ�. */

BEGIN(Engine)

class ENGINE_DLL CPrimitiveComponent : public CSceneComponent
{
public:
	explicit CPrimitiveComponent() = default;
	virtual ~CPrimitiveComponent() = default;

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

/********************
	Methods
********************/
public:
	// ��ġ�� ������ ���� ���۸� ���ε��մϴ�.
	virtual HRESULT BindBuffers();

protected:
	// ����̽��� ����� ���۸� �����մϴ�.
	HRESULT CreateBuffer(_Out_ ID3D11Buffer** _ppBuffer);

/********************
	Getter/Setter
********************/
public:
	void SetLevelType(LEVEL_TYPE _eLevelType) { m_eLevelType = _eLevelType; }

	void SetIsRender(_bool _bRender) { m_bRender = _bRender; }
	_bool GetIsRender() { return m_bRender; }

/********************
	Data Members
********************/
protected:
	// [ ����̽� ]
	ID3D11Device* m_pDevice = nullptr;

	// [ ����̽� ���ؽ�Ʈ ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	// [ ���ؽ� ���� ]
	ID3D11Buffer* m_pVertexBuffer = nullptr;

	// [ �ε��� ���� ]
	ID3D11Buffer* m_pIndexBuffer = nullptr;

//////////
/* ���� */
//////////
protected:
	// [ ���� ��ũ���� ]
	D3D11_BUFFER_DESC m_BufferDesc = {};
	// [ ���� ���ҽ� ������ ]
	D3D11_SUBRESOURCE_DATA m_InitialData = {};
	// [ �ε��� ���� ]
	DXGI_FORMAT m_eIndexFormat = {};
	// [ ������Ƽ�� �������� ]
	D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology = {};

	// [ ���ؽ� �Ѱ��� ����Ʈ ������ ]
	_uint m_iVertexStride = 0;
	// [ ���ؽ� ���� ]
	_uint m_iNumVertices = 0;
	// [ ���ؽ� ���� ���� ]
	_uint m_iNumVertexBuffers = 0;

	// [ �ε��� �Ѱ��� ����Ʈ ������ ]
	_uint m_iIndexStride = 0;
	// [ �ε��� ���� ]
	_uint m_iNumIndices = 0;

protected:
	// ����ƽ or ���̳���(���� ��ȯ�� ���ҽ� ���� ����) default = LEVEL_DYNAMIC
	LEVEL_TYPE m_eLevelType = LEVEL_DYNAMIC;

	// ���� ����
	_bool m_bRender = true;
};

END