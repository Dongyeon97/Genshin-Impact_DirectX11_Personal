#include "pch.h"
#include "ParticleActor_Point.h"

#include "CameraManager.h"

#include "Instancing.h"
#include "Particle_Point.h"

void CParticleActor_Point::Initialize()
{
	m_eRenderGroup = RENDER_NONLIGHT;

	m_wpParticle_Point = CreateActorComponent<CParticle_Point>(L"Particle_Point Component");
	m_wpParticle_Point.lock()->SetNumInstance(500);
	m_wpParticle_Point.lock()->SetShaderFileInfo(L"Shader_VtxPointInstance");
	m_wpParticle_Point.lock()->SetTextureFileInfo(L"Snow", 0, 0);
	m_wpParticle_Point.lock()->SetVertexInfo(VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements);
	m_wpParticle_Point.lock()->SetLevelType(LEVEL_TYPE::LEVEL_STATIC);

	INSTANCE_DESC InstanceDesc;

	InstanceDesc.vPivot = _float3(0.0f, 9.0f, 0.0f);
	InstanceDesc.vCenter = _float3(0.0f, 50.0f, 0.0f);
	InstanceDesc.vRange = _float3(300.0f, 5.0f, 300.0f);
	InstanceDesc.vSize = _float2(2.0f, 7.0f);
	InstanceDesc.vSpeed = _float2(10.f, 30.f);
	InstanceDesc.vLifeTime = _float2(20.f, 50.f);
	InstanceDesc.isLoop = true;
	InstanceDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);

	m_wpParticle_Point.lock()->SetInstanceDesc(InstanceDesc);

	CActor::Initialize();
}

_int CParticleActor_Point::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CParticleActor_Point::Update(const _float& _fTimeDelta)
{
	m_wpParticle_Point.lock()->Tick_Drop(_fTimeDelta);

	return CActor::Update(_fTimeDelta);
}

_int CParticleActor_Point::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CParticleActor_Point::Render()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpParticle_Point.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpParticle_Point.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpParticle_Point.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	m_wpParticle_Point.lock()->BindShaderResource("g_Texture", 0);

	m_wpParticle_Point.lock()->BeginShader(0);

	m_wpParticle_Point.lock()->BindBuffers();

	CActor::Render();
}

void CParticleActor_Point::EndPlay()
{
	CActor::EndPlay();
}

void CParticleActor_Point::Release()
{
	CActor::Release();
}

HRESULT CParticleActor_Point::Initialize(const wstring& _wstrJsonFileName)
{
	return E_NOTIMPL;
}

void CParticleActor_Point::RecycleBegin()
{
}
