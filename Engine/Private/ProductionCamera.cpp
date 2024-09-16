#include "Engine_pch.h"
#include "ProductionCamera.h"

#include "InputManager.h"

#include "Actor.h"

void CProductionCamera::Initialize()
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	CThirdPersonCamera::Initialize();
}

_int CProductionCamera::PreUpdate(const _float& _fTimeDelta)
{
	return CThirdPersonCamera::PreUpdate(_fTimeDelta);
}

_int CProductionCamera::Update(const _float& _fTimeDelta)
{
	// 카메라 쉐이킹
	if(m_bShaking)
		PlayShaking(_fTimeDelta);

	// 1. 연출 중이 아닐 땐, 일반적인 3인칭 카메라로 동작한다.
	if (!m_bProduction)
		return CThirdPersonCamera::Update(_fTimeDelta);

	// 2. 연출이 켜지면, 연출 동작을 수행한다.
	m_fProductionAcc += _fTimeDelta;

	PlayProduction(_fTimeDelta);

	if (m_fProductionTime <= m_fProductionAcc)
	{
		m_bProduction = false;
		m_fProductionAcc = 0.f;
	}

	CCameraComponent::UpdateCameraTransform(_fTimeDelta);
	return CActorComponent::Update(_fTimeDelta);
}

_int CProductionCamera::LateUpdate(const _float& _fTimeDelta)
{
	return CThirdPersonCamera::LateUpdate(_fTimeDelta);
}

void CProductionCamera::Render()
{
	CThirdPersonCamera::Render();
}

void CProductionCamera::EndPlay()
{
	CThirdPersonCamera::EndPlay();
}

void CProductionCamera::Release()
{
	CThirdPersonCamera::Release();
}

void CProductionCamera::BeginProduction(_vector _vCamStartPos, _vector _vCamEndPos, _vector _vCamStartAt, _gvector _vCamEndAt, _float _fProductionTime)
{
	// Eye
	XMStoreFloat4(&m_vStartCamPosition, _vCamStartPos);
	XMStoreFloat4(&m_vEndCamPosition, _vCamEndPos);

	// At
	XMStoreFloat4(&m_vStartAtPosition, _vCamStartAt);
	XMStoreFloat4(&m_vEndAtPosition, _vCamEndAt);

	// Time
	m_fProductionTime = _fProductionTime;
	m_fProductionAcc = 0.0f;

	m_bProduction = true;
}

void CProductionCamera::PlayProduction(const _float& _fTimeDelta)
{
	_float fCamMoveRatio = (m_fPositionMoveRatio * m_fProductionAcc) / m_fProductionTime;
	if (1.f < fCamMoveRatio)
		fCamMoveRatio = 1.f;

	_float fAtMoveRatio = (m_fAtMoveRatio * m_fProductionAcc) / m_fProductionTime;
	if (1.f < fAtMoveRatio)
		fAtMoveRatio = 1.f;

	_vector vLerpedCamPos = XMVectorLerp(XMLoadFloat4(&m_vStartCamPosition), XMLoadFloat4(&m_vEndCamPosition), fCamMoveRatio);
	_vector vLerpedAtPos = XMVectorLerp(XMLoadFloat4(&m_vStartAtPosition), XMLoadFloat4(&m_vEndAtPosition), fAtMoveRatio);

	_matrix matView;

	matView = XMMatrixLookAtLH(vLerpedCamPos, vLerpedAtPos, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	XMStoreFloat4x4(&m_matWorld, XMMatrixInverse(0, matView));
}

void CProductionCamera::BeginShaking(_float _fShakingStartTime, _float _fShakingTime, _float _fShakingRatio)
{
	// Time
	m_fShakingStartTime = _fShakingStartTime;
	m_fShakingTime = _fShakingTime;
	m_fShakingRatio = _fShakingRatio;
	m_fShakingAcc = 0.0f;

	m_bShaking = true;
}

void CProductionCamera::PlayShaking(const _float& _fTimeDelta)
{
	m_fShakingAcc += _fTimeDelta;
	if (m_fShakingTime < m_fShakingAcc)
	{
		m_fShakingAcc = 0.f;
		m_bShaking = false;

		m_fFovY = XMConvertToRadians(60.f);
		return;
	}

	if (m_fShakingAcc < m_fShakingStartTime)
		return;

	uniform_real_distribution<float>	ShakingRange(-m_fShakingRatio, m_fShakingRatio);

	m_fFovY = XMConvertToRadians(60.f + ShakingRange(m_RandomNumber));
}

void CProductionCamera::SetProductionRatio(_float _fRadiusRatio, _float _fPositionRatio, _float _fAtRatio)
{
	m_fRadiusMoveRatio = _fRadiusRatio;
	m_fPositionMoveRatio = _fPositionRatio;
	m_fAtMoveRatio = m_fAtMoveRatio;
}
