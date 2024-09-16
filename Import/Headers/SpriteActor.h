#pragma once
#include "ParticleActor.h"
#include "Particle_Point.h"

BEGIN(Engine)

class ENGINE_DLL CSpriteActor : public CParticleActor
{
public:
	explicit CSpriteActor() = default;
	virtual ~CSpriteActor() = default;

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

	// ※ 얘는 이걸로 Initialize 시키자 ※
	HRESULT Initialize(const wstring& _wstrJsonFileName);

	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	HRESULT BindSpriteShaderResources();

	void ProgressSprite(const _float& _fTimeDelta);
	void ResetUV();

/********************
	Getter / Setter
********************/
public:
	void SetRow(_uint _iRow) { m_iRow = _iRow; }
	_uint GetRow() { return m_iRow; }

	void SetCol(_uint _iCol) { m_iCol = _iCol; }
	_uint GetCol() { return m_iCol; }

	void SetSpeedRatio(_float _fRatio) { m_fSpeedRatio = _fRatio; }
	_float GetSpeedRatio() { return m_fSpeedRatio; }

/********************
	Data Members
********************/
protected:
	// [ 행, 열 개수 ]
	_uint m_iRow = 1;
	_uint m_iCol = 1;

	// [ 한장 넘길때, 증가시킬 UV ]
	_float2 m_vUVAddValue = {};

	// [ 셰이더에 전달할 UV ]
	_float2 m_vMinUV = {};
	_float2 m_vMaxUV = {};

	// [ UV 넘기는 속도 가중치 ] -> fSpriteTimeAcc가 이 값을 넘어가면, 한장 넘김
	_float m_fSpeedRatio = 0.1f;
	_float m_fSpriteTimeAcc = 0.f;
};

END