#pragma once
#include "PlayerProjectileEffect.h"

class CJusin : public CPlayerProjectileEffect
{
public:
	explicit CJusin() = default;
	virtual ~CJusin() = default;

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

	// [ 폭발 전에 닿으면 사라지는 걸 방지하기 위한 깡통 ]
	virtual _int OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {}) override;
/********************
	Methods
********************/
public:
	void Explosion();
	void MakeParticle(const wstring& _wstrParticleName, _vector _vOffset);
	void MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset);

/********************
	Getter / Setter
********************/
public:
	void SetExplosionPos(_vector _vPos) { XMStoreFloat4(&m_vExplosionPos, _vPos); }

/********************
	Data Members
********************/
protected:
	_float4 m_vExplosionPos = {};

	_bool m_bExplosionReserve = false;
};

