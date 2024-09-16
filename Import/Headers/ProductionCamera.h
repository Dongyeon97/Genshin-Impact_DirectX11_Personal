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
	// [ ���� on / off ]
	_bool m_bProduction = false;

	// [ ���� �ð� ]
	_float m_fProductionTime = 0.0f;
	_float m_fProductionAcc = 0.0f;

	// [ ����ŷ on / off ]
	_bool m_bShaking = false;

	// [ ����ŷ �ð� ]
	_float m_fShakingStartTime = 0.f;
	_float m_fShakingTime = 0.f;
	_float m_fShakingAcc = 0.f;

	// [ ����ŷ ���� ]
	_float m_fShakingRatio = 0.f;

protected:

#pragma region Origin
	// -> ���� ���� ������ ��׵鿡�� ���� ī�޶� �������� �ְ� �����Ѵ�.
	// [ ���� ���� ������ Radius ]
	_float m_fOriginRadius = 0.0f;

	// [ ���� ���� ������ Position ]
	_float4 m_vOriginPosition = {};

	// [ ���� ���� ������ At ]
	_float4 m_vOriginAt = {};


#pragma region Production
	// -> ���� ���߿��� �� ������ �ð��� �帧�� ����ġ�� ���� �����ϸ� �̵��Ѵ�.
	// [ ī�޶��� ��� ��ġ ]
	_float4 m_vStartCamPosition = {};

	// [ ī�޶��� ���� ��ġ ]
	_float4 m_vEndCamPosition = {};

	// [ ī�޶��� ��� at ]
	_float4 m_vStartAtPosition = {};

	// [ ī�޶��� ���� at ]
	_float4 m_vEndAtPosition = {};

	// [ ī�޶��� ��� Radius ]
	_float m_fStartRadius = 0.f;

	// [ ī�޶��� ���� Radius ]
	_float m_fEndRadius = 0.f;

#pragma region Ratio
	// [ Radius �̵� ����ġ ]
	_float m_fRadiusMoveRatio = 1.0f;

	// [ Position �̵� ����ġ ]
	_float m_fPositionMoveRatio = 1.0f;

	// [ At �̵� ����ġ ]
	_float m_fAtMoveRatio = 1.0f;
};

END