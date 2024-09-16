#pragma once
#include "Actor.h"

BEGIN(Engine)

class CNonAnimModel;

END

class CTest_Object_1 final : public CActor
{
public:
	explicit CTest_Object_1() = default;
	virtual ~CTest_Object_1() = default;

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
	HRESULT BindShaderResources();

	/********************
		Data Members
	********************/
private:
	weak_ptr<CNonAnimModel> m_wpNonAnimModel;
};