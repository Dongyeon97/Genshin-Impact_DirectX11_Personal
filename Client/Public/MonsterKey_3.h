#pragma once
#include "EffectMeshActor.h"

class CMonsterKey_3 : public CEffectMeshActor
{
public:
	explicit CMonsterKey_3() = default;
	virtual ~CMonsterKey_3() = default;

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
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ 오브젝트 풀 용 ]
	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	void LookForPlayer();

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:
};
