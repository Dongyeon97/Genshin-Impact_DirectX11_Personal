#include "Engine_pch.h"
#include "ThirdPersonCamera.h"

#include "InputManager.h"

#include "Actor.h"

void CThirdPersonCamera::Initialize()
{
	SphericalCoordinates();

	SetActorTranslation(GetActorTranslation() + ToCartesian());
	XMStoreFloat4(&m_vOriginPos, GetActorTranslation() + ToCartesian());

	return CCameraComponent::Initialize();
}

_int CThirdPersonCamera::PreUpdate(const _float& _fTimeDelta)
{
	return CCameraComponent::PreUpdate(_fTimeDelta);
}

_int CThirdPersonCamera::Update(const _float& _fTimeDelta)
{
	if (m_bShoulderView)
	{
		m_fShoulderViewTimeAcc += _fTimeDelta;
		if (m_fShoulderViewMinTime < m_fShoulderViewTimeAcc)
		{
			m_bShoulderView = false;
			m_fShoulderViewTimeAcc = 0.f;
		}

		return CCameraComponent::Update(_fTimeDelta);
	}

	UpdateThirdPersonCameraTransform(_fTimeDelta);

	// 뷰 매트릭스를 갱신합니다.
	CCameraComponent::UpdateCameraTransform(_fTimeDelta);

	return CActorComponent::Update(_fTimeDelta);
}

_int CThirdPersonCamera::LateUpdate(const _float& _fTimeDelta)
{
	return CCameraComponent::LateUpdate(_fTimeDelta);
}

void CThirdPersonCamera::Render()
{
	CCameraComponent::Render();
}

void CThirdPersonCamera::EndPlay()
{
	CCameraComponent::EndPlay();
}

void CThirdPersonCamera::Release()
{
	CCameraComponent::Release();
}

void CThirdPersonCamera::UpdateThirdPersonCameraTransform(const _float _fTimeDelta)
{
	_int iMouseAxisX = CInputManager::GetInstance()->GetMouseDeltaX();
	_int iMouseAxisY = CInputManager::GetInstance()->GetMouseDeltaY();

	_float Horizontal	= lerp(m_fPrevHorizontal, (_float)iMouseAxisX * -1.0f / 10.0f, 0.1f);
	_float Vertical		= lerp(m_fPrevVertical, (_float)iMouseAxisY * 1.0f / 10.0f, 0.1f);

	_float3 vOwnerPos;
	XMStoreFloat3(&vOwnerPos, m_wpOwnerActor.lock()->GetActorTranslation());

	// 카메라의 위치를 잡는다.
	_float3 vLookPosition = { vOwnerPos.x + m_vTranslate.x, vOwnerPos.y + m_vTranslate.y, vOwnerPos.z + m_vTranslate.z };

	SphericalCoordinatesRotate(Horizontal * _fTimeDelta, Vertical * _fTimeDelta);

	// 플레이어 중심으로 구한 구면좌표를 카메라 위치에 적용
	_vector vCameraPosition = ToCartesian() + XMLoadFloat3(&vLookPosition);

	_matrix matView;
	_float3 vTemp = vOwnerPos;

	vTemp.y = vOwnerPos.y + 200.0f;

	matView = XMMatrixLookAtLH(vCameraPosition, XMLoadFloat3(&vTemp), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	XMStoreFloat4x4(&m_matWorld, XMMatrixInverse(0, matView));

	// 보간용으로 이전의 회전값을 저장한다.
	m_fPrevHorizontal = Horizontal;
	m_fPrevVertical = Vertical;
}

void CThirdPersonCamera::SphericalCoordinates()
{
	// 카메라 주인 액터의 위치벡터
	_vector vOwnerPos = m_wpOwnerActor.lock()->GetActorTranslation();
	_float3 fvOwnerPos;
	XMStoreFloat3(&fvOwnerPos, vOwnerPos);

	// 방위각의 라디안 값(최대, 최소)를 구한다.
	m_fMinAzimuth_Rad = XMConvertToRadians(m_fMinAzimuth_Deg);
	m_fMaxAzimuth_Rad = XMConvertToRadians(m_fMaxAzimuth_Deg);

	// 앙각의 라디안 값(최대, 최소)를 구한다.
	m_fMinElevation_Rad = XMConvertToRadians(m_fMinElevation_Deg);
	m_fMaxElevation_Rad = XMConvertToRadians(m_fMaxElevation_Deg);

	// 역함수로 방위각과 앙각을 구한다.
	m_azimuth = atan2(fvOwnerPos.z, fvOwnerPos.x);
	m_elevation = DirectX::XMScalarASin(fvOwnerPos.y / m_fRadius);
}

void CThirdPersonCamera::SphericalCoordinatesRotate(_float _fNewAzimuth, _float _fNewElevation)
{
	m_azimuth += _fNewAzimuth;
	m_elevation += _fNewElevation;

	m_azimuth = fmodf(m_azimuth, m_fMaxAzimuth_Rad - m_fMinAzimuth_Rad);
	if (m_azimuth < 0.0f)
	{
		m_azimuth += m_fMaxAzimuth_Rad - m_fMinAzimuth_Rad;
	}

	m_elevation = clamp(m_elevation, m_fMinElevation_Rad, m_fMaxElevation_Rad);
}

_vector CThirdPersonCamera::ToCartesian()
{
	_float t = m_fRadius * XMScalarCos(m_elevation);

	_float3 vCartesian = { XMScalarCos(m_azimuth)	* t,
						   XMScalarSin(m_elevation) * m_fRadius,
						   XMScalarSin(m_azimuth)	* t};

	return XMLoadFloat3(&vCartesian);
}

// x,z 좌표를 이용해 방위각을 뱉는 함수
_float CThirdPersonCamera::GetAzimuth(const _float _fMaxAzimuth, const _float _fMinAzimuth)
{
	_float4 vOwnerPos;
	XMStoreFloat4(&vOwnerPos, m_wpOwnerActor.lock()->GetActorTranslation());

	_float fAzimuth = atan2(vOwnerPos.z, vOwnerPos.x);

	return fAzimuth;
}

// 고도를 이용해 앙각을 얻는 함수
_float CThirdPersonCamera::GetElevation(const _float _fMaxElevation, const _float _fMinElevation)
{
	_float4 vOwnerPos;
	XMStoreFloat4(&vOwnerPos, m_wpOwnerActor.lock()->GetActorTranslation());

	return DirectX::XMScalarASin(vOwnerPos.y / m_fRadius);
}

void CThirdPersonCamera::SetShoulderView(_float _fViewTime, _vector _vOffset)
{
	m_bShoulderView = true;
	m_fShoulderViewMinTime = _fViewTime;

	XMStoreFloat4(&m_vTranslate, _vOffset);
}

void CThirdPersonCamera::SetShoulderViewOff(_bool _bEnable)
{
	m_bShoulderView = _bEnable;

	XMStoreFloat4(&m_vTranslate, XMLoadFloat4(&m_vOriginPos));
}
