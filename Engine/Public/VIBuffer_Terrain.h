#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CNonMeshComponent
{
public:
	explicit CVIBuffer_Terrain() = default;
	virtual ~CVIBuffer_Terrain() = default;

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
	void SetHeightMapFileInfo(const wstring& _wstrHeightMapFilePath);

private:
	virtual HRESULT InitializeBuffer() override { return S_OK; }

	virtual HRESULT InitializeBuffer(const wstring& _wstrHeightMapFilePath);
/********************
	Data Members
********************/
private:
	// [ 정점 개수 ]
	_uint			m_iNumVerticesX = { 0 };
	_uint			m_iNumVerticesZ = { 0 };

	// [ 높이맵 파일 경로 ]
	wstring m_wstrHeightMapFilePath = L"";
};

END