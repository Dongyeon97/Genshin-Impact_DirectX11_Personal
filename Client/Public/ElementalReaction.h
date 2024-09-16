#pragma once
#include "Actor.h"

class CElementalReaction : public CActor
{
public:
	explicit CElementalReaction() = default;
	virtual ~CElementalReaction() = default;

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
	virtual void RecycleBegin() override;

	_vector CalculateToViewPort();
	_float4 CalculateFontColor(ELEMENTAL_TYPE _ElementalType);

/********************
	Getter / Setter
********************/
public:
	void SetReactionName(const wstring& _wstrReactionName) { m_wstrReactionName = _wstrReactionName; }
	void SetAttackDesc(ATTACK_DESC _AttackDesc) { m_AttackDesc = _AttackDesc; }
	void SetSpawnPos(_vector _vSpawnPos) { XMStoreFloat4(&m_vSpawnPos, _vSpawnPos); }
	void SetMinMaxScale(_float2 _vMinMaxScale) { m_fMinScaleValue = _vMinMaxScale.x; m_fMaxScaleValue = _vMinMaxScale.y; }

/********************
	Data Members
********************/
protected:
	ATTACK_DESC m_AttackDesc = {};
	wstring m_wstrReactionName = L"";

protected:
	_float4 m_vViewPortPos = {};
	_float4 m_vSpawnPos = {};

	_float	m_fRenderTimeAcc = 0.f;

	_float	m_fMaxScaleValue = 2.f;
	_float	m_fMinScaleValue = 1.f;
	_float	m_fScaleValue = 1.f;
};

