#pragma once
#include "SceneComponent.h"

/* PrimitiveComponents는 일반적으로 충돌 데이터로 렌더링되거나
사용되는 일종의 형상을 포함하거나 생성하는 SceneComponent입니다. */

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
	// 장치에 세팅이 끝난 버퍼를 바인딩합니다.
	virtual HRESULT BindBuffers();

protected:
	// 디바이스를 사용해 버퍼를 생성합니다.
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
	// [ 디바이스 ]
	ID3D11Device* m_pDevice = nullptr;

	// [ 디바이스 컨텍스트 ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

	// [ 버텍스 버퍼 ]
	ID3D11Buffer* m_pVertexBuffer = nullptr;

	// [ 인덱스 버퍼 ]
	ID3D11Buffer* m_pIndexBuffer = nullptr;

//////////
/* 버퍼 */
//////////
protected:
	// [ 버퍼 디스크립션 ]
	D3D11_BUFFER_DESC m_BufferDesc = {};
	// [ 서브 리소스 데이터 ]
	D3D11_SUBRESOURCE_DATA m_InitialData = {};
	// [ 인덱스 포맷 ]
	DXGI_FORMAT m_eIndexFormat = {};
	// [ 프리미티브 토폴로지 ]
	D3D11_PRIMITIVE_TOPOLOGY m_ePrimitiveTopology = {};

	// [ 버텍스 한개의 바이트 사이즈 ]
	_uint m_iVertexStride = 0;
	// [ 버텍스 개수 ]
	_uint m_iNumVertices = 0;
	// [ 버텍스 버퍼 개수 ]
	_uint m_iNumVertexBuffers = 0;

	// [ 인덱스 한개의 바이트 사이즈 ]
	_uint m_iIndexStride = 0;
	// [ 인덱스 개수 ]
	_uint m_iNumIndices = 0;

protected:
	// 스태틱 or 다이나믹(레벨 전환시 리소스 정리 여부) default = LEVEL_DYNAMIC
	LEVEL_TYPE m_eLevelType = LEVEL_DYNAMIC;

	// 렌더 여부
	_bool m_bRender = true;
};

END