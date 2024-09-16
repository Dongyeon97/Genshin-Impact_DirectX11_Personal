#include "pch.h"
#include "IngameUI.h"

#include "CameraManager.h"

#include "VIBuffer_UI.h"

void CIngameUI::Initialize()
{
	m_eRenderGroup = RENDER_UI;

	m_wpUI = CreateActorComponent<CVIBuffer_UI>(L"Component_VIBuffer_UI");

	// 자식 클래스에서 셰이더파일, 텍스처 파일, 위치, 크기 세팅해주기
	m_wpUI.lock()->SetShaderFileInfo(m_wstrShaderFileName);
	m_wpUI.lock()->SetTextureFileInfo(m_wstrTextureFileName, 0, 0);
	m_wpUI.lock()->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_wpUI.lock()->SetLevelType(Engine::LEVEL_STATIC);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(Client::g_iWinSizeX, Client::g_iWinSizeY, 0.f, 1.f));

	CActor::Initialize();
}

_int CIngameUI::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CIngameUI::Update(const _float& _fTimeDelta)
{
	m_fRenderTimeAcc += _fTimeDelta;
	if (m_fRenderTimeAcc >= 3.f && m_bEnable)
	{
		m_bEnable = false;
		m_fRenderTimeAcc = 0.f;
	}

	return CActor::Update(_fTimeDelta);
}

_int CIngameUI::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CIngameUI::Render()
{
	if(m_bEnable)
		CActor::Render();
}

void CIngameUI::EndPlay()
{
	CActor::EndPlay();
}

void CIngameUI::Release()
{
	CActor::Release();
}

_vector CIngameUI::CalculateToViewPort()
{
	_vector vOwnerPos = {};

	// 1. 부모나 자신의 월드좌표를 구한다.
	if (!m_wpOwnerActor.expired())
		vOwnerPos = GetOwnerActor().lock()->GetActorTranslation();
	else
		vOwnerPos = GetActorTranslation();

	vOwnerPos = XMVectorSetY(vOwnerPos, XMVectorGetY(vOwnerPos) + m_fOffsetY);

	// 2. VP를 곱해서 투영변환까지 완료된 좌표를 구한다.
	vOwnerPos = XMVector3TransformCoord(vOwnerPos, CCameraManager::GetInstance()->GetViewMatrix());
	vOwnerPos = XMVector3TransformCoord(vOwnerPos, CCameraManager::GetInstance()->GetProjMatrix());

	// 3. 투영 좌표를 정규화된 디바이스 좌표로 변환
	_float4 vNormalizedDeviceCoord = {};
	vNormalizedDeviceCoord.x = XMVectorGetX(vOwnerPos);
	vNormalizedDeviceCoord.y = XMVectorGetY(vOwnerPos);
	vNormalizedDeviceCoord.z = XMVectorGetZ(vOwnerPos);
	vNormalizedDeviceCoord.w = 1.0f;

	if (vNormalizedDeviceCoord.z < 0 || vNormalizedDeviceCoord.z > 1)
		return _vector();

	// 4. 정규화된 디바이스 좌표를 화면 좌표로 변환
	_float4 vViewportPosition;
	vViewportPosition.x = (vNormalizedDeviceCoord.x + 1.0f) * 0.5f * Client::g_iWinSizeX;
	vViewportPosition.y = (1.0f - vNormalizedDeviceCoord.y) * 0.5f * Client::g_iWinSizeY;
	vViewportPosition.z = vNormalizedDeviceCoord.z;
	vViewportPosition.w = 1.0f;

	return XMLoadFloat4(&vViewportPosition);
}

void CIngameUI::SetSize(const _float2& _vSize)
{
	m_vSize.x = _vSize.x;
	m_vSize.y = _vSize.y;
}