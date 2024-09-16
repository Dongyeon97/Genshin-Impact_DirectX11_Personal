#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_UI : public CNonMeshComponent
{
public:
	explicit CVIBuffer_UI() = default;
	virtual ~CVIBuffer_UI() = default;

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
	virtual HRESULT InitializeBuffer() override; // 순수 가상 함수

/********************
	Getter/Setter
********************/
public:


/********************
	Data Members
********************/
protected:

};

END