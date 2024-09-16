#pragma once
#include "CameraComponent.h"

BEGIN(Engine)

class ENGINE_DLL CThirdPersonCamera : public CCameraComponent
{
public:
	explicit CThirdPersonCamera() = default;
	virtual ~CThirdPersonCamera() = default;

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
	virtual void UpdateThirdPersonCameraTransform(const _float _fTimeDelta);

protected:
	void SphericalCoordinates();
	void SphericalCoordinatesRotate(_float _fNewAzimuth, _float _fNewElevation);
	_vector ToCartesian();

/********************
	Getter / Setter
********************/
public:
	_float GetAzimuth(const _float _fMaxAzimuth, const _float _fMinAzimuth);
	_float GetElevation(const _float _fMaxElevation, const _float _fMinElevation);

public:
	void SetRadius(_float _fRadius) { m_fRadius = _fRadius; }
	_float GetRadius() { return m_fRadius; }

public:
	void SetElevation(_float _fMin, _float _fMax) { m_fMinElevation_Deg = _fMin; m_fMaxElevation_Deg = _fMax; }
	
public:
	void SetShoulderView(_float _fViewTime, _vector _vOffset);
	void SetShoulderViewOff(_bool _bEnable);

/********************
	Data Members
********************/
protected:
	_bool m_bShoulderView = false;

	_float m_fShoulderViewMinTime = 0.f;
	_float m_fShoulderViewTimeAcc = 0.f;

	_float4 m_vOriginPos = {};

protected:
	_float m_fRadius = 10.0f;

	_float m_azimuth = 0.0f;
	_float m_elevation = 0.0f;

	_float m_fPrevHorizontal = 0.0f;
	_float m_fPrevVertical = 0.0f;

protected:
	_float m_fMinAzimuth_Deg = 0.0f;
	_float m_fMaxAzimuth_Deg = 360.0f;

	_float m_fMinAzimuth_Rad = 0.0f;
	_float m_fMaxAzimuth_Rad = 0.0f;
	// ================================== //
	_float m_fMinElevation_Deg =  -20.0f;
	_float m_fMaxElevation_Deg =  40.0f;

	_float m_fMinElevation_Rad = 0.0f;
	_float m_fMaxElevation_Rad = 0.0f;
};

END