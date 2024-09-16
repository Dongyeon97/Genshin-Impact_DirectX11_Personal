#pragma once
#include "Actor.h"

BEGIN(Engine)
class CParticle_Rect;
END

class CParticleActor_Rect final : public CActor
{
public:
	explicit CParticleActor_Rect() = default;
	virtual ~CParticleActor_Rect() = default;

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
	weak_ptr<CParticle_Rect> m_wpParticle_Rect;
};

