#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cube : public CNonMeshComponent
{
public:
	explicit CVIBuffer_Cube() = default;
	virtual ~CVIBuffer_Cube() = default;

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
	virtual HRESULT InitializeBuffer() override; // ���� ���� �Լ�

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