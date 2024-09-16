#pragma once
#include "UIActor_Base.h"

class CIngameUI : public CActor
{
public:
	explicit CIngameUI() = default;
	virtual ~CIngameUI() = default;

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
	virtual _vector CalculateToViewPort();

private:
	virtual HRESULT BindShaderResources() = 0;

/********************
	Getter / Setter
********************/
public:
	void SetShaderFileName(const wstring& _wstrShaderFileName) { m_wstrShaderFileName = _wstrShaderFileName; }
	void SetTextureFileName(const wstring& _wstrTextureFileName) { m_wstrTextureFileName = _wstrTextureFileName; }
	void SetSize(const _float2& _vSize);
	void SetOffsetY(_float _fOffset) { m_fOffsetY = _fOffset; }

public:
	void SetEnable(_bool _bEnable) { m_bEnable = _bEnable; m_fRenderTimeAcc = 0.f; }

/********************
	Data Members
********************/
protected:
	weak_ptr<CVIBuffer_UI> m_wpUI;

	wstring m_wstrShaderFileName = L"";
	wstring m_wstrTextureFileName = L"";

	_bool		m_bEnable = false;
	_float2		m_vSize;
	_float		m_fOffsetY;
	_float		m_fRenderTimeAcc = 0.f;

	_float4x4	m_ViewMatrix, m_ProjMatrix;
};

