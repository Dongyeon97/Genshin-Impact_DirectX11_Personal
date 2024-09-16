#pragma once
#include "EffectMeshActor.h"
class CShield : public CEffectMeshActor
{
public:
	explicit CShield() = default;
	virtual ~CShield() = default;

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
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ ������Ʈ Ǯ �� ]
	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	void UpdateCurPlayerPos();

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:

};

