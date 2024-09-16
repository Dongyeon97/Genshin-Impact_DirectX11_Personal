#pragma once
#include "ThirdPersonCamera.h"

BEGIN(Engine)

class ENGINE_DLL CProductionCamera : public CThirdPersonCamera
{
public:
	explicit CProductionCamera() = default;
	virtual ~CProductionCamera() = default;

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
	void BeginProduction(_vector _vCamStartPos, _vector _vCamEndPos, _vector _vCamStartAt, _gvector _vCamEndAt, _float _fProductionTime);
	void PlayProduction(const _float& _fTimeDelta);

public:
	void BeginShaking(_float _fShakingStartTime, _float _fShakingTime, _float _fShakingRatio);
	void PlayShaking(const _float& _fTimeDelta);

/********************
	Getter / Setter
********************/
public:
	void SetProductionRatio(_float _fRadiusRatio, _float _fPositionRatio, _float _fAtRatio);
	void SetProductionTime(_float _fProductionTime) { m_fProductionTime = _fProductionTime; }

/********************
	Data Members
********************/
protected:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

protected:
	// [ 연출 on / off ]
	_bool m_bProduction = false;

	// [ 연출 시간 ]
	_float m_fProductionTime = 0.0f;
	_float m_fProductionAcc = 0.0f;

	// [ 셰이킹 on / off ]
	_bool m_bShaking = false;

	// [ 셰이킹 시간 ]
	_float m_fShakingStartTime = 0.f;
	_float m_fShakingTime = 0.f;
	_float m_fShakingAcc = 0.f;

	// [ 셰이킹 강도 ]
	_float m_fShakingRatio = 0.f;

protected:

#pragma region Origin
	// -> 연출 시작 시점에 얘네들에게 원래 카메라 정보들을 넣고 시작한다.
	// [ 연출 시작 시점의 Radius ]
	_float m_fOriginRadius = 0.0f;

	// [ 연출 시작 시점의 Position ]
	_float4 m_vOriginPosition = {};

	// [ 연출 시작 시점의 At ]
	_float4 m_vOriginAt = {};


#pragma region Production
	// -> 연출 도중에는 이 값들을 시간에 흐름과 가중치에 따라 보간하며 이동한다.
	// [ 카메라의 출발 위치 ]
	_float4 m_vStartCamPosition = {};

	// [ 카메라의 도착 위치 ]
	_float4 m_vEndCamPosition = {};

	// [ 카메라의 출발 at ]
	_float4 m_vStartAtPosition = {};

	// [ 카메라의 도착 at ]
	_float4 m_vEndAtPosition = {};

	// [ 카메라의 출발 Radius ]
	_float m_fStartRadius = 0.f;

	// [ 카메라의 도착 Radius ]
	_float m_fEndRadius = 0.f;

#pragma region Ratio
	// [ Radius 이동 가중치 ]
	_float m_fRadiusMoveRatio = 1.0f;

	// [ Position 이동 가중치 ]
	_float m_fPositionMoveRatio = 1.0f;

	// [ At 이동 가중치 ]
	_float m_fAtMoveRatio = 1.0f;
};

END