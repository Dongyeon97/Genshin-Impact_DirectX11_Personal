#pragma once
#include "EffectMeshActor.h"

class CMonsterKeyUI;

class CMonsterKey_1 : public CEffectMeshActor
{
public:
	explicit CMonsterKey_1() = default;
	virtual ~CMonsterKey_1() = default;

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
	void CheckTrigger();

/********************
	Getter / Setter
********************/
public:
	void SetCurLevelIndex(_uint _iIndex) { m_iLevelIndex = _iIndex; }

/********************
	Data Members
********************/
protected:
	_uint m_iLevelIndex = 0;

	weak_ptr<CMonsterKeyUI> m_wpMonsterKeyUI;
};
