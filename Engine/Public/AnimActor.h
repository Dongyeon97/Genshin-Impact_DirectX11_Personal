
#pragma once
#include "MeshActor.h"

BEGIN(Engine)

class CAnimModel;

class ENGINE_DLL CAnimActor : public CMeshActor
{
public:
	explicit CAnimActor() = default;
	virtual ~CAnimActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void Render_Shadow() override;
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual HRESULT BindShaderResources();
	virtual HRESULT BindShaderResources_Shadow();

/********************
	Getter / Setter
********************/
public:
	void SetAnimModel(shared_ptr<CAnimModel> _spAnimModel) { m_wpAnimModel = _spAnimModel; }
	shared_ptr<CAnimModel> GetAnimModel() { return m_wpAnimModel.lock(); }

	void SetAdditionalPivot(_matrix _matAdditionalPivot) { XMStoreFloat4x4(&m_matAdditionalPivot, _matAdditionalPivot); }

/********************
	Data Members
********************/
protected:
	weak_ptr<CAnimModel> m_wpAnimModel;

	_float4x4 m_matAdditionalPivot = {};
};

END