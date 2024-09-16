#pragma once
#include "MeshActor.h"

BEGIN(Engine)

class CNonAnimModel;

class ENGINE_DLL CNonAnimActor : public CMeshActor
{
public:
	explicit CNonAnimActor() = default;
	virtual ~CNonAnimActor() = default;

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
	virtual HRESULT BindShaderResources();

	// [ 마우스 피킹용입니다 ]
	void SetVertexPositionToWorld();

/********************
	Getter / Setter
********************/
public:
	void SetNonAnimModel(shared_ptr<CNonAnimModel> _spNonAnimModel) { m_wpNonAnimModel = _spNonAnimModel; }
	shared_ptr<CNonAnimModel> GetNonAnimModel() { return m_wpNonAnimModel.lock(); }

/********************
	Data Members
********************/
protected:
	weak_ptr<CNonAnimModel> m_wpNonAnimModel;
};

END