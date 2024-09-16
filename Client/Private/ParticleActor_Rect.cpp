#include "pch.h"
#include "ParticleActor_Rect.h"

#include "CameraManager.h"

#include "Instancing.h"
#include "Particle_Rect.h"

void CParticleActor_Rect::Initialize()
{
	m_eRenderGroup = RENDER_NONBLEND;

	m_wpParticle_Rect = CreateActorComponent<CParticle_Rect>(L"Particle_Rect");
	m_wpParticle_Rect.lock()->SetNumInstance(100);
	m_wpParticle_Rect.lock()->SetShaderFileInfo(L"Shader_VtxRectInstance");
	m_wpParticle_Rect.lock()->SetTextureFileInfo(L"Snow", 0, 0);
	m_wpParticle_Rect.lock()->SetVertexInfo(VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements);
	m_wpParticle_Rect.lock()->SetLevelType(LEVEL_TYPE::LEVEL_DYNAMIC);

	INSTANCE_DESC InstanceDesc;

	InstanceDesc.vCenter = _float3(0.0f, 300.f, 0.f);
	InstanceDesc.vRange = _float3(100.0f, 20.f, 100.f);
	InstanceDesc.vSize = _float2(100.0f, 200.0f);
	InstanceDesc.vSpeed = _float2(2.f, 5.f);

	m_wpParticle_Rect.lock()->SetInstanceDesc(InstanceDesc);

	CActor::Initialize();
}

_int CParticleActor_Rect::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CParticleActor_Rect::Update(const _float& _fTimeDelta)
{
	m_wpParticle_Rect.lock()->Tick_Drop(_fTimeDelta);

	return CActor::Update(_fTimeDelta);
}

_int CParticleActor_Rect::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CParticleActor_Rect::Render()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpParticle_Rect.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpParticle_Rect.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpParticle_Rect.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	m_wpParticle_Rect.lock()->BindShaderResource("g_Texture", 0);

	m_wpParticle_Rect.lock()->BeginShader(0);

	m_wpParticle_Rect.lock()->BindBuffers();

	CActor::Render();
}

void CParticleActor_Rect::EndPlay()
{
	CActor::EndPlay();
}

void CParticleActor_Rect::Release()
{
	CActor::Release();
}
