#pragma once
#include "Actor.h"

BEGIN(Engine)
class CVIBuffer_Cube;
END

class CSkybox2 : public CActor
{
public:
	explicit CSkybox2() = default;
	virtual ~CSkybox2() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render();
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
public:
	HRESULT BindShaderResources();

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:
	weak_ptr<CVIBuffer_Cube> m_wpCubeBuffer;
};

