#pragma once
#include "NonAnimModel.h"

BEGIN(Engine)

class ENGINE_DLL CProjectile : public CNonAnimModel
{
public:
	explicit CProjectile() = default;
	virtual ~CProjectile() = default;

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


/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:
	
};

END