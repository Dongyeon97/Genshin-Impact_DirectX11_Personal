#pragma once
#include "Actor.h"

BEGIN(Engine)
class CVIBuffer_UI;
END

class CUIActor_Base : public CActor
{
public:
	explicit CUIActor_Base() = default;
	virtual ~CUIActor_Base() = default;

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
protected:
	virtual HRESULT BindShaderResources() = 0;


/********************
	Getter / Setter
********************/
public:
	void SetShaderFileName(const wstring& _wstrShaderFileName) { m_wstrShaderFileName = _wstrShaderFileName; }
	void SetTextureFileName(const wstring& _wstrTextureFileName) { m_wstrTextureFileName = _wstrTextureFileName; }
	void SetPosition(const _float2& _vPosition);
	void SetSize(const _float2& _vSize);

	void SetEnable(_bool _bEnable) { m_bEnable = _bEnable; }
/********************
	Data Members
********************/
protected:
	weak_ptr<CVIBuffer_UI> m_wpUI;

	wstring m_wstrShaderFileName = L"";
	wstring m_wstrTextureFileName = L"";

protected:
	_float		m_fX, m_fY;
	_float2		m_vSize;
	_bool		m_bEnable = false;

	_float4x4	m_ViewMatrix, m_ProjMatrix;
};

