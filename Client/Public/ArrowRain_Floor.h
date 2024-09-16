#pragma once
#include "PlayerAttackEffect.h"

BEGIN(Engine)
class CParticle_Point;
END

class CArrowRain_Floor : public CPlayerAttackEffect
{
public:
	explicit CArrowRain_Floor() = default;
	virtual ~CArrowRain_Floor() = default;

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


/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
protected:
	_float m_fAttackTimeAcc = 0.f;
};
