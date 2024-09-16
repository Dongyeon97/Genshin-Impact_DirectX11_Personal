#pragma once
#include "Actor.h"

BEGIN(Engine)

class CVIBuffer_Rect;

END

class CLogo_LoadingText2 : public CActor
{
public:
	explicit CLogo_LoadingText2() = default;
	virtual ~CLogo_LoadingText2() = default;

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
	virtual HRESULT BindShaderResources();

/********************
	Data Members
********************/
private:
	weak_ptr<CVIBuffer_Rect> m_wpRectBuffer;

private:
	_float		m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4	m_ViewMatrix, m_ProjMatrix;
};
