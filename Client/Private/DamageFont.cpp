#include "pch.h"
#include "DamageFont.h"

#include "CameraManager.h"
#include "FontManager.h"

#include "VIBuffer_UI.h"
#include "Charactor.h"
#include "CameraComponent.h"

void CDamageFont::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	CActor::Initialize();
}

void CDamageFont::RecycleBegin()
{
	m_bUse = true;
	m_fRenderTimeAcc = 0.f;

	CActor::RecycleBegin();
}

_int CDamageFont::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CDamageFont::Update(const _float& _fTimeDelta)
{
	m_fRenderTimeAcc += _fTimeDelta;
	if (2.f < m_fRenderTimeAcc)
	{
		m_fRenderTimeAcc = 0.f;
		m_bUse = false;
	}

	// ��Ʈ ������ ����
	m_fScaleValue = lerp(m_fMaxScaleValue, m_fMinScaleValue, 3.f * m_fRenderTimeAcc);
	if (m_fMinScaleValue >= m_fScaleValue)
		m_fScaleValue = m_fMinScaleValue;

	XMStoreFloat4(&m_vViewPortPos, CalculateToViewPort());

	m_vViewPortPos.y -= 20.f * m_fRenderTimeAcc;

	SetActorTranslation_NoNavigation(XMVectorSet(XMVectorGetX(XMLoadFloat4(&m_vViewPortPos)),XMVectorGetY(XMLoadFloat4(&m_vViewPortPos)), 0.f, 1.f));

	return CActor::Update(_fTimeDelta);
}

_int CDamageFont::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CDamageFont::Render()
{
	_float4 vPos = {};

	XMStoreFloat4(&vPos, GetActorTranslation());

	// �Ӽ��� ���� ���� ����
	_float4 vColor = {};
	switch (m_AttackDesc.eElementalType)
	{
	case NONE_ELEMENTAL:
	{
		vColor = _float4(1.f, 1.f, 1.f, 1.f);
		break;
	}
	case PYRO:
	{
		vColor = _float4(254.f / 255.f, 147.f / 255.f, 0.f / 255.f, 1.f);
		break;
	}
	case ANEMO:
	{
		vColor = _float4(105.f / 255.f, 252.f / 255.f, 205.f / 255.f, 1.f);
		break;
	}
	case CRYO:
	{
		vColor = _float4(152.f / 255.f, 250.f / 255.f, 255.f / 255.f, 1.f);
		break;
	}
	case DENDRO:
	{
		vColor = _float4(5.f / 255.f, 232.f / 255.f, 79.f / 255.f, 1.f);
		break;
	}
	case ELECTRO:
	{
		vColor = _float4(229.f / 255.f, 162.f / 255.f, 255.f / 255.f, 1.f);
		break;
	}
	case GEO:
	{
		vColor = _float4(251.f / 255.f, 162.f / 218.f, 100.f / 255.f, 1.f);
		break;
	}
	case HYDRO:
	{
		vColor = _float4(53.f / 255.f, 206.f / 218.f, 255.f / 255.f, 1.f);
		break;
	}
	}

	// ������ ��Ʈ ���
	if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont", 
		to_wstring((_uint)m_AttackDesc.eDamage),
		_float2(vPos.x, vPos.y), 
		XMLoadFloat4(&vColor),
		_float(0.f), 
		_float2(0.f, 0.f), 
		m_fScaleValue)))
		return;

	CActor::Render();
}

void CDamageFont::EndPlay()
{
	CActor::EndPlay();
}

void CDamageFont::Release()
{
	CActor::Release();
}

_vector CDamageFont::CalculateToViewPort()
{
	// 1. �ڽ��� ������ǥ�� ���Ѵ�.
	_vector vWorldPos = XMLoadFloat4(&m_vSpawnPos);

	// 2. VP�� ���ؼ� ������ȯ���� �Ϸ�� ��ǥ�� ���Ѵ�.
	vWorldPos = XMVector3TransformCoord(vWorldPos, CCameraManager::GetInstance()->GetViewMatrix());
	vWorldPos = XMVector3TransformCoord(vWorldPos, CCameraManager::GetInstance()->GetProjMatrix());

	// 3. ���� ��ǥ�� ����ȭ�� ����̽� ��ǥ�� ��ȯ
	_float4 vNormalizedDeviceCoord = {};
	vNormalizedDeviceCoord.x = XMVectorGetX(vWorldPos);
	vNormalizedDeviceCoord.y = XMVectorGetY(vWorldPos);
	vNormalizedDeviceCoord.z = XMVectorGetZ(vWorldPos);
	vNormalizedDeviceCoord.w = 1.0f;

	if (vNormalizedDeviceCoord.z < 0 || vNormalizedDeviceCoord.z >1)
		return _vector();

	// 4. ����ȭ�� ����̽� ��ǥ�� ȭ�� ��ǥ�� ��ȯ
	_float4 vViewportPosition = {};
	vViewportPosition.x = (vNormalizedDeviceCoord.x + 1.0f) * 0.5f * Client::g_iWinSizeX;
	vViewportPosition.y = (1.0f - vNormalizedDeviceCoord.y) * 0.5f * Client::g_iWinSizeY;
	vViewportPosition.z = vNormalizedDeviceCoord.z;
	vViewportPosition.w = 1.0f;

	return XMLoadFloat4(&vViewportPosition);
}