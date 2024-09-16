#include "pch.h"
#include "ElementalReaction.h"

#include "CameraManager.h"
#include "FontManager.h"

void CElementalReaction::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	CActor::Initialize();
}

void CElementalReaction::RecycleBegin()
{
	m_bUse = true;
	m_fRenderTimeAcc = 0.f;

	CActor::RecycleBegin();
}

_int CElementalReaction::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CElementalReaction::Update(const _float& _fTimeDelta)
{
	m_fRenderTimeAcc += _fTimeDelta;
	if (2.f < m_fRenderTimeAcc)
	{
		m_fRenderTimeAcc = 0.f;
		m_bUse = false;
	}

	// 폰트 스케일 갱신
	m_fScaleValue = lerp(m_fMaxScaleValue, m_fMinScaleValue, 3.f * m_fRenderTimeAcc);
	if (m_fMinScaleValue >= m_fScaleValue)
		m_fScaleValue = m_fMinScaleValue;

	XMStoreFloat4(&m_vViewPortPos, CalculateToViewPort());

	m_vViewPortPos.y -= 15.f * m_fRenderTimeAcc;

	SetActorTranslation_NoNavigation(XMVectorSet(XMVectorGetX(XMLoadFloat4(&m_vViewPortPos)), XMVectorGetY(XMLoadFloat4(&m_vViewPortPos)), 0.f, 1.f));

	return CActor::Update(_fTimeDelta);
}

_int CElementalReaction::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CElementalReaction::Render()
{
	_float4 vPos = {};

	XMStoreFloat4(&vPos, GetActorTranslation());

	// 반응 이름에 따라 색상 결정
	_float4 vColor = {};

	// 이거를 위해 m_AttackDesc에 피격당한 AttackDesc을 세팅해주어야 한다.
	vColor = CalculateFontColor(m_AttackDesc.eElementalType);

	// 원소 반응 폰트 출력
	if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont",
		m_wstrReactionName,
		_float2(vPos.x, vPos.y),
		XMLoadFloat4(&vColor),
		_float(0.f),
		_float2(0.f, 0.f),
		m_fScaleValue)))
		return;

	CActor::Render();
}

void CElementalReaction::EndPlay()
{
	CActor::EndPlay();
}

void CElementalReaction::Release()
{
	CActor::Release();
}

_vector CElementalReaction::CalculateToViewPort()
{
	// 1. 자신의 월드좌표를 구한다.
	_vector vWorldPos = XMLoadFloat4(&m_vSpawnPos);

	// 2. VP를 곱해서 투영변환까지 완료된 좌표를 구한다.
	vWorldPos = XMVector3TransformCoord(vWorldPos, CCameraManager::GetInstance()->GetViewMatrix());
	vWorldPos = XMVector3TransformCoord(vWorldPos, CCameraManager::GetInstance()->GetProjMatrix());

	// 3. 투영 좌표를 정규화된 디바이스 좌표로 변환
	_float4 vNormalizedDeviceCoord = {};
	vNormalizedDeviceCoord.x = XMVectorGetX(vWorldPos);
	vNormalizedDeviceCoord.y = XMVectorGetY(vWorldPos);
	vNormalizedDeviceCoord.z = XMVectorGetZ(vWorldPos);
	vNormalizedDeviceCoord.w = 1.0f;

	if (vNormalizedDeviceCoord.z < 0 || vNormalizedDeviceCoord.z >1)
		return _vector();

	// 4. 정규화된 디바이스 좌표를 화면 좌표로 변환
	_float4 vViewportPosition = {};
	vViewportPosition.x = (vNormalizedDeviceCoord.x + 1.0f) * 0.5f * Client::g_iWinSizeX;
	vViewportPosition.y = (1.0f - vNormalizedDeviceCoord.y) * 0.5f * Client::g_iWinSizeY;
	vViewportPosition.z = vNormalizedDeviceCoord.z;
	vViewportPosition.w = 1.0f;

	return XMLoadFloat4(&vViewportPosition);
}
_float4 CElementalReaction::CalculateFontColor(ELEMENTAL_TYPE _ElementalType)
{
	switch (_ElementalType)
	{
	case NONE_ELEMENTAL:	return _float4(1.f, 1.f, 1.f, 1.f);
	case PYRO:				return _float4(254.f / 255.f, 147.f / 255.f, 0.f / 255.f, 1.f);
	case ANEMO:				return _float4(105.f / 255.f, 252.f / 255.f, 205.f / 255.f, 1.f);
	case CRYO:				return _float4(152.f / 255.f, 250.f / 255.f, 255.f / 255.f, 1.f);
	case DENDRO:			return _float4(5.f / 255.f, 232.f / 255.f, 79.f / 255.f, 1.f);
	case ELECTRO:			return _float4(229.f / 255.f, 162.f / 255.f, 255.f / 255.f, 1.f);
	case GEO:				return _float4(251.f / 255.f, 162.f / 218.f, 100.f / 255.f, 1.f);
	case HYDRO:				return _float4(53.f / 255.f, 206.f / 218.f, 255.f / 255.f, 1.f);
		break;
	}

	return _float4();
}