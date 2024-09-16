#pragma once
#include "Actor.h"

BEGIN(Engine)
class CParticle_Point;
END

class CParticleActor_Point final : public CActor
{
public:
	explicit CParticleActor_Point() = default;
	virtual ~CParticleActor_Point() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

	// [ JSON �Ľ� ��]
	HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ ������Ʈ Ǯ �� ]
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
	weak_ptr<CParticle_Point> m_wpParticle_Point;

};