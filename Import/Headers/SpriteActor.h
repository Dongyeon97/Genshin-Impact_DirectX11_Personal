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

	// �� ��� �̰ɷ� Initialize ��Ű�� ��
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
	// [ ��, �� ���� ]
	_uint m_iRow = 1;
	_uint m_iCol = 1;

	// [ ���� �ѱ涧, ������ų UV ]
	_float2 m_vUVAddValue = {};

	// [ ���̴��� ������ UV ]
	_float2 m_vMinUV = {};
	_float2 m_vMaxUV = {};

	// [ UV �ѱ�� �ӵ� ����ġ ] -> fSpriteTimeAcc�� �� ���� �Ѿ��, ���� �ѱ�
	_float m_fSpeedRatio = 0.1f;
	_float m_fSpriteTimeAcc = 0.f;
};

END