#pragma once
#include "EffectMeshActor.h"

BEGIN(Engine)

class ENGINE_DLL CMovingEffect : public CEffectMeshActor
{
public:
	explicit CMovingEffect() = default;
	virtual ~CMovingEffect() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	void Render();
	virtual void EndPlay() override;
	virtual void Release() override;

	// [ JSON 파싱 용]
	HRESULT Initialize(const wstring& _wstrJsonFileName);

/********************
	Methods
********************/
public:
	// [ 오브젝트 풀 용 ]
	virtual void RecycleBegin();

	void UpdateMove(const _float& _fTimeDelta);


/********************
	Getter / Setter
********************/
public:
	void SetSpeed(_float _fMinSpeed, _float _fMaxSpeed) { m_vSpeed.x = _fMinSpeed; m_vSpeed.y = _fMaxSpeed; }
	const _float2 GetSpeed() { return m_vSpeed; }

	void SetIsGravity(_bool _bIsGravity) { m_bGravity = _bIsGravity; }
	void SetGravityValue(_float _fGravityValue) { m_fGravityValue = _fGravityValue; }

	void SetWidth(_float2 _vWidthRange) { m_vWidthRange = _vWidthRange; }
	void SetHeight(_float2 _vHeightRange) { m_vHeightRange = _vHeightRange; }
	void SetDepth(_float2 _vDepthRange) { m_vDepthRange = _vDepthRange; }

/********************
	Data Members
********************/
protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	// [ 최소, 최대속도(랜덤) ]
	_float2	m_vSpeed = {};

	// [ 실제 속도 ]
	_float m_fSpeed = 0.f;

	// [ 움직이는 방향 ]
	_float3 m_vMoveDir = {};
	_float2 m_vWidthRange = {};		// x축 랜덤 범위
	_float2 m_vHeightRange = {};	// y축 랜덤 범위
	_float2 m_vDepthRange = {};		// z축 랜덤 범위

	// [ 랜덤 회전값 ]
	_float3 m_vRandomRotation = {};

	// [ 중력 적용 여부 ]
	_bool m_bGravity = false;

	// [ 중력값 ]
	_float m_fGravityValue = 9.8f;
};

END