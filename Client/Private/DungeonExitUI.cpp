#include "pch.h"
#include "DungeonExitUI.h"

#include "CameraManager.h"
#include "FontManager.h"

#include "VIBuffer_UI.h"
#include "Charactor.h"
#include "CameraComponent.h"

void CDungeonExitUI::Initialize()
{
	CIngameUI::Initialize();
}

_int CDungeonExitUI::PreUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::PreUpdate(_fTimeDelta);
}

_int CDungeonExitUI::Update(const _float& _fTimeDelta)
{
	_vector vViewportPos = CalculateToViewPort();
	SetActorTranslation_NoNavigation(XMVectorSet(XMVectorGetX(vViewportPos) - Client::g_iWinSizeX * 0.5f,
		-XMVectorGetY(vViewportPos) + Client::g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4(&m_vViewPortPos, vViewportPos);

	return CIngameUI::Update(_fTimeDelta);
}

_int CDungeonExitUI::LateUpdate(const _float& _fTimeDelta)
{
	return CIngameUI::LateUpdate(_fTimeDelta);
}

void CDungeonExitUI::Render()
{
	if (!m_bEnable)
		return;

	BindShaderResources();

	CIngameUI::Render();

	if (3 != m_iCurStageIndex)
	{
		if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont",
			L"비경 나가기",
			_float2(m_vViewPortPos.x - 90.f, m_vViewPortPos.y - 15.f),
			XMVectorSet(0.8f, 0.8f, 0.8f, 1.f),
			_float(0.f),
			_float2(0.f, 0.f),
			0.9f)))
			return;
	}
	else
	{
		if (FAILED(CFontManager::GetInstance()->Render(L"GenshinFont",
			L"졸업 포폴실",
			_float2(m_vViewPortPos.x - 90.f, m_vViewPortPos.y - 15.f),
			XMVectorSet(0.2f, 0.2f, 0.2f, 1.f),
			_float(0.f),
			_float2(0.f, 0.f),
			0.9f)))
			return;
	}
}

void CDungeonExitUI::EndPlay()
{
	CIngameUI::EndPlay();
}

void CDungeonExitUI::Release()
{
	CIngameUI::Release();
}

HRESULT CDungeonExitUI::BindShaderResources()
{
	if (m_wpOwnerActor.expired())
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindMatrix("g_WorldMatrix", &m_matWorld)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_wpUI.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindRawValue("g_vSize", &m_vSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BeginShader(0)))
		return E_FAIL;

	if (FAILED(m_wpUI.lock()->BindBuffers()))
		return E_FAIL;

	return S_OK;
}

_vector CDungeonExitUI::CalculateToViewPort()
{
	_vector vOwnerPos = {};

	// 1. 부모나 자신의 월드좌표를 구한다.
	if (!m_wpOwnerActor.expired())
		vOwnerPos = GetOwnerActor().lock()->GetActorTranslation();
	else
		vOwnerPos = GetActorTranslation();

	// vOwnerPos = XMVectorSetY(vOwnerPos, XMVectorGetY(vOwnerPos));

	// 2. VP를 곱해서 투영변환까지 완료된 좌표를 구한다.
	vOwnerPos = XMVector3TransformCoord(vOwnerPos, CCameraManager::GetInstance()->GetViewMatrix());
	vOwnerPos = XMVector3TransformCoord(vOwnerPos, CCameraManager::GetInstance()->GetProjMatrix());

	// 3. 투영 좌표를 정규화된 디바이스 좌표로 변환
	_float4 vNormalizedDeviceCoord = {};
	vNormalizedDeviceCoord.x = XMVectorGetX(vOwnerPos);
	vNormalizedDeviceCoord.y = XMVectorGetY(vOwnerPos);
	vNormalizedDeviceCoord.z = XMVectorGetZ(vOwnerPos);
	vNormalizedDeviceCoord.w = 1.0f;

	if (vNormalizedDeviceCoord.z < 0 || vNormalizedDeviceCoord.z >1)
		return _vector();

	// 4. 정규화된 디바이스 좌표를 화면 좌표로 변환
	_float4 vViewportPosition;
	vViewportPosition.x = (vNormalizedDeviceCoord.x + 1.0f) * 0.5f * Client::g_iWinSizeX;
	vViewportPosition.y = (1.0f - vNormalizedDeviceCoord.y) * 0.5f * Client::g_iWinSizeY;
	vViewportPosition.z = vNormalizedDeviceCoord.z;
	vViewportPosition.w = 1.0f;

	return XMLoadFloat4(&vViewportPosition);
}