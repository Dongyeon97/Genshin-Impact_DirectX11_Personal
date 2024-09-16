#pragma once
#include "Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CParticle_Rect : public CInstancing
{
public:
	explicit CParticle_Rect() = default;
	virtual ~CParticle_Rect() = default;

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
	virtual HRESULT InitializeBuffer() override;

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