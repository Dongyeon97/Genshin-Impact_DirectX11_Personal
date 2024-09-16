#pragma once
#include "EffectMeshActor.h"
class CCaptureEffect : public CEffectMeshActor
{
public:
	explicit CCaptureEffect() = default;
	virtual ~CCaptureEffect() = default;

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

	// [ JSON �Ľ� ��]
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName) override;

	// [ ������Ʈ Ǯ �� ]
	virtual void RecycleBegin() override;

/********************
	Methods
********************/
public:
	void SetOffset(_vector _vOffset) { XMStoreFloat4(&m_vOffset, _vOffset); }

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:
	_float4 m_vOffset = {};
};

