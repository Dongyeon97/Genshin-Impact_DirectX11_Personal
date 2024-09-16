#pragma once
#include "EffectMeshActor.h"
class CSeal : public CEffectMeshActor
{
public:
	explicit CSeal() = default;
	virtual ~CSeal() = default;

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

	// [ JSON 파싱 용]
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName) override;

	// [ 오브젝트 풀 용 ]
	virtual void RecycleBegin() override;

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

