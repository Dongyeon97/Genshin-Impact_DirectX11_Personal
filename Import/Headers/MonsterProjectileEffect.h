#pragma once
#include "MonsterAttackEffect.h"

BEGIN(Engine)

class CAttackCollider;

class ENGINE_DLL CMonsterProjectileEffect : public CMonsterAttackEffect
{
public:
	explicit CMonsterProjectileEffect() = default;
	virtual ~CMonsterProjectileEffect() = default;

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
	void ProgressToDirrection(const _float& _fTimeDelta);

/********************
	Getter / Setter
********************/
public:
	void SetShootDirrection(_vector _vShootDirrection) { XMStoreFloat3(&m_vShootDirrection, _vShootDirrection); }
	_vector GetShootDirrection() { return XMLoadFloat3(&m_vShootDirrection); }

	void SetShootSpeed(_float _fShootSpeed) { m_fShootSpeed = _fShootSpeed; }
	_float GetShootSpeed() { return m_fShootSpeed; }

	void SetShootAcc(_float _fShootAcc) { m_fShootAcc = _fShootAcc; }
	_float GetShootAcc() { return m_fShootAcc; }

/********************
	Data Members
********************/
protected:
	// [ 발사 방향 ]
	_float3 m_vShootDirrection = {};

	// [ 발사 속도 ]
	_float m_fShootSpeed = 0.0f;

	// [ 발사 가속도 ]
	_float m_fShootAcc = 1.0f;
};

END