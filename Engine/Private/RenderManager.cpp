#include "Engine_pch.h"
#include "RenderManager.h"

#include "ObjectManager.h"
#include "DeviceManager.h"
#include "FontManager.h"
#include "RenderTargetManager.h"
#include "LightManager.h"
#include "CameraManager.h"

#include "Actor.h"

#include "Rect_RenderTarget.h"
#include "CameraComponent.h"

IMPLEMENT_SINGLETON(CRenderManager)

_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

HRESULT CRenderManager::Initialize()
{
	// 렌더매니저는 장치가 필요하기 때문에,
	// 반드시 디바이스 매니저가 초기화된 후에 초기화 되어야 합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	D3D11_VIEWPORT ViewPortDesc = {};

	_uint iNumViewports = 1;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	//////////////////////////
	// == 렌더타겟 추가 == ///
	//////////////////////////
	/* Shadow */
	// if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Shadow"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		// return E_FAIL;

#pragma region MRT_GAMEOBJECTS

	/* Diffuse */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Diffuse"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	/* Normal */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Normal"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	/* Depth */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Depth"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

#pragma endregion

#pragma region MRT_LIGHTACC

	/* Shade */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Shade"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	/* Specular */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_Specular"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

#pragma endregion

#pragma region MRT_BLOOM

	/* Bloom / Glow */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget(L"Target_Effect", (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget(L"Target_Bloom", (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget(L"Target_Blur_X", (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget(L"Target_Blur_Y", (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget(L"Target_Glow", (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

#pragma endregion

#pragma region MRT_OUTLINE

	/* OutLine */
	if (FAILED(CRenderTargetManager::GetInstance()->AddRenderTarget((L"Target_OutLine"), (_uint)ViewPortDesc.Width, (_uint)ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

#pragma endregion

	/* BackBuffer Copy Texture */
	if(FAILED(CRenderTargetManager::GetInstance()->Ready_BackBufferCopyTexture(m_pDevice, m_pDeviceContext, g_iWinSizeX, g_iWinSizeY)))
		return E_FAIL;

	//////////////////////////////
	// == 멀티 렌더타겟 추가 == //
	//////////////////////////////
	/* For.MRT_Shadow */
	// if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Shadow", L"Target_Shadow")))
		// return E_FAIL;

	/* MRT_GameObjects */
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT((L"MRT_GameObjects"), L"Target_Diffuse")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_GameObjects", L"Target_Normal")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_GameObjects", L"Target_Depth")))
		return E_FAIL;

	/* MRT_LightAcc */
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_LightAcc", L"Target_Shade")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_LightAcc", L"Target_Specular")))
		return E_FAIL;

	/* MRT_Bloom */
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Effect", L"Target_Effect")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Bloom", L"Target_Bloom")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Blur_X", L"Target_Blur_X")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Blur_Y", L"Target_Blur_Y")))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_Glow", L"Target_Glow")))
		return E_FAIL;

	/* MRT_OutLine */
	if (FAILED(CRenderTargetManager::GetInstance()->AddMRT(L"MRT_OutLine", L"Target_OutLine")))
		return E_FAIL;

	m_spRect_RenderTarget = make_shared<CRect_RenderTarget>();
	if (nullptr == m_spRect_RenderTarget)
		return E_FAIL;

	m_spRect_RenderTarget->SetShaderFileInfo(L"Shader_Deferred");
	m_spRect_RenderTarget->SetVertexInfo(VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	m_spRect_RenderTarget->Initialize();

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewPortDesc.Width;
	m_WorldMatrix._22 = ViewPortDesc.Height;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.0f, 1.0f));

	//ID3D11Texture2D* pDepthStencil = { nullptr };

	//D3D11_TEXTURE2D_DESC	TextureDesc;
	//ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//TextureDesc.Width = g_iSizeX;
	//TextureDesc.Height = g_iSizeY;
	//TextureDesc.MipLevels = 1;
	//TextureDesc.ArraySize = 1;
	//TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//TextureDesc.SampleDesc.Quality = 0;
	//TextureDesc.SampleDesc.Count = 1;

	//TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	//TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; /* | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; */
	//TextureDesc.CPUAccessFlags = 0;
	//TextureDesc.MiscFlags = 0;
	//if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencil)))
	//	return E_FAIL;
	//if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencil, nullptr, &m_pShadowDepthStencilView)))
	//	return E_FAIL;

#ifdef _DEBUG

	// if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Shadow", 250.f, 50.f, 100.f, 100.f)))
		// return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Diffuse", 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Shade", 150.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Specular", 150.f, 150.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Normal", 50.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Depth", 50.f, 250.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Effect", 250.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Bloom", 250.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Blur_X", 250.f, 250.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Blur_Y", 250.f, 350.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_Glow", 250.f, 450.f, 100.f, 100.f)))
		return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->Ready_Debug(L"Target_OutLine", 150.f, 250.f, 100.f, 100.f)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderManager::Render()
{
	if (!CObjectManager::GetInstance()->GetIsLoading())
	{

	}
	//if (FAILED(Render_Shadow()))
	//	return E_FAIL;

	// 원근 //
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_OutLine()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL;
	if (FAILED(Render_Final()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_Glow()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_PostProcessing()))
		return E_FAIL;

	// 직교 //
	if (FAILED(Render_Orthogonal()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CRenderManager::AddObjectToRenderGroup(RENDER_GROUP _eRenderGroup, shared_ptr<CActor> _spActor)
{
	if (_eRenderGroup >= RENDER_END || _eRenderGroup < RENDER_PRIORITY)
		return E_FAIL;

	m_lstRenderActors[_eRenderGroup].push_back(_spActor);

	return S_OK;
}

HRESULT CRenderManager::Render_Priority()
{
	for (auto& pActor : m_lstRenderActors[RENDER_PRIORITY])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_PRIORITY].clear();

	return S_OK;
}

//HRESULT CRenderManager::Render_Shadow()
//{
//	D3D11_VIEWPORT			ViewPortDesc;
//	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
//	ViewPortDesc.TopLeftX = 0;
//	ViewPortDesc.TopLeftY = 0;
//	ViewPortDesc.Width = (_float)g_iSizeX;
//	ViewPortDesc.Height = (_float)g_iSizeY;
//	ViewPortDesc.MinDepth = 0.f;
//	ViewPortDesc.MaxDepth = 1.f;
//
//	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);
//
//	CRenderTargetManager::GetInstance()->BeginMRT(TEXT("MRT_Shadow"), m_pShadowDepthStencilView);
//
//	for (auto& pActor : m_lstRenderActors[RENDER_SHADOW])
//	{
//		if (nullptr != pActor)
//			pActor->Render_Shadow();
//	}
//
//	m_lstRenderActors[RENDER_SHADOW].clear();
//
//	CRenderTargetManager::GetInstance()->EndMRT();
//
//	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
//	ViewPortDesc.TopLeftX = 0;
//	ViewPortDesc.TopLeftY = 0;
//	ViewPortDesc.Width = (_float)1280.0f;
//	ViewPortDesc.Height = (_float)720.0f;
//	ViewPortDesc.MinDepth = 0.f;
//	ViewPortDesc.MaxDepth = 1.f;
//
//	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);
//
//	return S_OK;
//}

HRESULT CRenderManager::Render_NonBlend()
{
	////////////////////////////
	// == Diffuse + Normal == //
	////////////////////////////
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_GameObjects")))
		return E_FAIL;

	for (auto& pActor : m_lstRenderActors[RENDER_NONBLEND])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_NONBLEND].clear();

	if (FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_Lights()
{
	/* Shade */
	if(FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_LightAcc")))
		return E_FAIL;

	/* 사각형을 직교 투영으로 화면에 꽉 채워서 그린다. */
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 스페큘러 연산을 위한 역행렬들과 캠 포지션을 쉐이더에 던진다. */
	_float4x4 matProjInv;
	XMStoreFloat4x4(&matProjInv, XMMatrixInverse(0, CCameraManager::GetInstance()->GetProjMatrix()));
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrixInv", &matProjInv)))
		return E_FAIL;

	_float4x4 matViewInv;
	XMStoreFloat4x4(&matViewInv, XMMatrixInverse(0, CCameraManager::GetInstance()->GetViewMatrix()));
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrixInv", &matViewInv)))
		return E_FAIL;

	_float4 vCamPosition;
	XMStoreFloat4(&vCamPosition, CCameraManager::GetInstance()->GetCamPosition());
	if (FAILED(m_spRect_RenderTarget->BindRawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	/* 노말 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Normal", m_spRect_RenderTarget, "g_NormalTexture")))
		return E_FAIL;
	/* 뎁스 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Depth", m_spRect_RenderTarget, "g_DepthTexture")))
		return E_FAIL;

	/* 빛들을 하나씩 그린다.(사각형버퍼를 셰이드 타겟에 그린다.) */
	CLightManager::GetInstance()->Render(m_spRect_RenderTarget);

	if (FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_Final()
{
	/* 사각형을 직교 투영으로 화면에 꽉 채워서 그린다. */
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 디퓨즈 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Diffuse", m_spRect_RenderTarget, "g_DiffuseTexture")))
		return E_FAIL;
	/* 노말 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Normal", m_spRect_RenderTarget, "g_NormalTexture")))
		return E_FAIL;
	/* 셰이드 렌더타겟을 쉐이더에 던진다. */
	if(FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Shade", m_spRect_RenderTarget, "g_ShadeTexture")))
		return E_FAIL;
	/* 스페큘러 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Specular", m_spRect_RenderTarget, "g_SpecularTexture")))
		return E_FAIL;
	/* 뎁스 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Depth", m_spRect_RenderTarget, "g_DepthTexture")))
		return E_FAIL;
	/* 외곽선 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_OutLine", m_spRect_RenderTarget, "g_OutLineTexture")))
		return E_FAIL;

	/* 블러_Y 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Blur_Y", m_spRect_RenderTarget, "g_BlurTexture")))
		return E_FAIL;
	/* 이펙트 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Effect", m_spRect_RenderTarget, "g_EffectTexture")))
		return E_FAIL;

	m_spRect_RenderTarget->BeginShader(3);

	m_spRect_RenderTarget->BindBuffers();

	m_spRect_RenderTarget->Render();

	return S_OK;
}

HRESULT CRenderManager::Render_NonLight()
{
	for (auto& pActor : m_lstRenderActors[RENDER_NONLIGHT])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_Glow()
{
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_Effect")))
		return E_FAIL;

	m_lstRenderActors[RENDER_GLOW].sort([&](shared_ptr<CActor> spDstActor, shared_ptr<CActor> spSrcActor) -> bool {
		if (spDstActor->CalculateDistanceFromCam() > spSrcActor->CalculateDistanceFromCam())
			return true;
		return false; });

	for (auto& pActor : m_lstRenderActors[RENDER_GLOW])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_GLOW].clear();

	if(FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

#pragma region Bloom
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_Bloom")))
		return E_FAIL;

	/* Effect 렌더 타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Effect", m_spRect_RenderTarget, "g_EffectTexture")))
		return E_FAIL;

	m_spRect_RenderTarget->BeginShader(8);
	m_spRect_RenderTarget->BindBuffers();
	m_spRect_RenderTarget->Render();

	if (FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

#pragma region Blur_X
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_Blur_X")))
		return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Bloom", m_spRect_RenderTarget, "g_EffectTexture")))
		return E_FAIL;

	m_spRect_RenderTarget->BeginShader(6);
	m_spRect_RenderTarget->BindBuffers();
	m_spRect_RenderTarget->Render();

	if (CRenderTargetManager::GetInstance()->EndMRT())
		return E_FAIL;


#pragma region Blur_Y
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_Blur_Y")))
		return E_FAIL;

	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Blur_X", m_spRect_RenderTarget, "g_EffectTexture")))
		return E_FAIL;

	m_spRect_RenderTarget->BeginShader(7);
	m_spRect_RenderTarget->BindBuffers();
	m_spRect_RenderTarget->Render();

	if (CRenderTargetManager::GetInstance()->EndMRT())
		return E_FAIL;

#pragma region Glow
	// 여기서 EffectTexture를 던지게 되면, HDRColor 와 동일한 역할을 하게 된다.
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_Glow")))
		return E_FAIL;

	// Bloom + Blur 처리 완료된 텍스처
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Blur_Y", m_spRect_RenderTarget, "g_BloomBlurTexture")))
		return E_FAIL;
	// Bloom만 처리되어 있는 텍스처
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Bloom", m_spRect_RenderTarget, "g_BloomTexture")))
		return E_FAIL;
	// 맨 처음 Effect만을 그린 장면(HDRColor)
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Effect", m_spRect_RenderTarget, "g_HDRTexture")))
		return E_FAIL;

	m_spRect_RenderTarget->BeginShader(9);
	m_spRect_RenderTarget->BindBuffers();
	m_spRect_RenderTarget->Render();

	if (FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_Blend()
{
	// 블렌드 그룹은 카메라와의 거리를 비교해서 소팅
	m_lstRenderActors[RENDER_BLEND].sort([&](shared_ptr<CActor> spDstActor, shared_ptr<CActor> spSrcActor) -> bool {
		if (spDstActor->CalculateDistanceFromCam() > spSrcActor->CalculateDistanceFromCam())
			return true;
		return false; });

	for (auto& pActor : m_lstRenderActors[RENDER_BLEND])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_OutLine()
{
	/* OutLine */
	if (FAILED(CRenderTargetManager::GetInstance()->BeginMRT(L"MRT_OutLine")))
		return E_FAIL;

	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 노멀 렌더타겟을 쉐이더에 던진다. */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Normal", m_spRect_RenderTarget, "g_NormalTexture")))
		return E_FAIL;

	/* Pass : OutLine */
	if (FAILED(m_spRect_RenderTarget->BeginShader(4)))
		return E_FAIL;

	m_spRect_RenderTarget->BindBuffers();

	m_spRect_RenderTarget->Render();

	if (FAILED(CRenderTargetManager::GetInstance()->EndMRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderManager::Render_PostProcessing()
{
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 백 버퍼를 카피해온다. */
	if (FAILED(CRenderTargetManager::GetInstance()->Copy_BackBufferTexture(m_pDevice, m_pDeviceContext)))
		return E_FAIL;

	/* Final까지 그린 텍스처를 쉐이더에 던진다. */
	if (FAILED(m_spRect_RenderTarget->BindShaderResource("g_FinalTexture", CRenderTargetManager::GetInstance()->Get_BackBufferCopySRV())))
		return E_FAIL;

	/* Glow 렌더 타겟을 쉐이더에 던진다.(Glow까지 완료됨) */
	if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_Glow", m_spRect_RenderTarget, "g_GlowTexture")))
		return E_FAIL;

	///* 외곽선 렌더타겟을 쉐이더에 던진다. */
	//if (FAILED(CRenderTargetManager::GetInstance()->BindSRV(L"Target_OutLine", m_spRect_RenderTarget, "g_OutLineTexture")))
	//	return E_FAIL;

	/* Pass : PostProcessing */
	if (FAILED(m_spRect_RenderTarget->BeginShader(5)))
		return E_FAIL;

	m_spRect_RenderTarget->BindBuffers();

	m_spRect_RenderTarget->Render();

	return S_OK;
}

HRESULT CRenderManager::Render_Orthogonal()
{
	for (auto& pActor : m_lstRenderActors[RENDER_ORTHOGONAL])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_ORTHOGONAL].clear();

	return S_OK;
}

HRESULT CRenderManager::Render_UI()
{
	for (auto& pActor : m_lstRenderActors[RENDER_UI])
	{
		if (nullptr != pActor)
			pActor->Render();
	}

	m_lstRenderActors[RENDER_UI].clear();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderManager::AddDebugComponent(shared_ptr<CPrimitiveComponent> _spPrimitiveComponent)
{
	m_lstDebugRenderComponents.push_back(_spPrimitiveComponent);

	return S_OK;
}
HRESULT CRenderManager::Render_Debug()
{
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_spRect_RenderTarget->BindMatrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_GameObjects", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_LightAcc", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_OutLine", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Effect", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Bloom", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Blur_X", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Blur_Y", m_spRect_RenderTarget);

	CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Glow", m_spRect_RenderTarget);

	// CRenderTargetManager::GetInstance()->RenderMRT(L"MRT_Shadow", m_spRect_RenderTarget);

	for (auto& spDebugComponent : m_lstDebugRenderComponents)
	{
		spDebugComponent->SetIsRender(true);
		spDebugComponent->Render();
		spDebugComponent->SetIsRender(false);
	}
	m_lstDebugRenderComponents.clear();

	return S_OK;
}
#endif // _DEBUG

void CRenderManager::Release()
{
	for (_uint i = 0; i < RENDER_END; ++i)
		m_lstRenderActors[i].clear();

	if (nullptr != m_spRect_RenderTarget)
	{
		m_spRect_RenderTarget->Release();
		m_spRect_RenderTarget.reset();
	}

	Safe_Release(m_pShadowDepthStencilView);

	// 렌더 매니저 파괴
	m_upInstance.reset();
}