#include "Engine_pch.h"
#include "ParticleActor.h"

#include "CameraManager.h"

#include "Particle_Point.h"

// 툴 용
void CParticleActor::Initialize()
{
	m_eRenderGroup = RENDER_NONLIGHT;
	m_vScale = _float4{ 1.0f, 1.0f, 1.0f, 0.1f };

	// 외부에서 INSTANCE_DESC의 정보를 다 채워준 다음, Set 해줄 것이기 때문에 아무것도 필요없음.
	m_wpParticle_Point = CreateActorComponent<CParticle_Point>(L"Particle_Point Component");

	m_wpParticle_Point.lock()->SetInstanceDesc(m_InstanceDesc);
	m_eRenderGroup = m_InstanceDesc.eRenderGroup;

	m_wpParticle_Point.lock()->SetVertexInfo(VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements);
	m_wpParticle_Point.lock()->SetLevelType(LEVEL_STATIC);

	CActor::Initialize();
}

// JSON 파싱 용
HRESULT CParticleActor::Initialize(const wstring& _wstrJsonFileName)
{
	m_vScale = _float4{ 1.0f, 1.0f, 1.0f, 0.1f };
	 
	// 외부에서 INSTANCE_DESC의 정보를 다 채워준 다음, Set 해줄 것이기 때문에 아무것도 필요없음.
	m_wpParticle_Point = CreateActorComponent<CParticle_Point>(L"Particle_Point Component");

	if (FAILED(ReadParticleFile_JSON(m_wstrJSONFileName)))
		return E_FAIL;

	m_eRenderGroup = m_InstanceDesc.eRenderGroup;
	m_wpParticle_Point.lock()->SetInstanceDesc(m_InstanceDesc);

	m_wpParticle_Point.lock()->SetVertexInfo(VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements);
	m_wpParticle_Point.lock()->SetLevelType(LEVEL_STATIC);

	CActor::Initialize();
	return S_OK;
}

void CParticleActor::RecycleBegin()
{
	m_bUse = true;
	m_wpParticle_Point.lock()->RecycleBegin();
}

_int CParticleActor::PreUpdate(const _float& _fTimeDelta)
{
	return CActor::PreUpdate(_fTimeDelta);
}

_int CParticleActor::Update(const _float& _fTimeDelta)
{
	switch (m_InstanceDesc.eParticleType)
	{
	case PARTICLE_TYPE::DROP:
	{
		m_wpParticle_Point.lock()->Tick_Drop(_fTimeDelta);
		break;
	}
	case PARTICLE_TYPE::SPREAD:
	{
		m_wpParticle_Point.lock()->Tick_Spread(_fTimeDelta);
		break;
	}
	case PARTICLE_TYPE::SPREAD_RADIUS:
	{
		m_wpParticle_Point.lock()->Tick_Spread_Radius(_fTimeDelta);
		break;
	}
	case PARTICLE_TYPE::SPRITE:
	{
		m_wpParticle_Point.lock()->Tick_Sprite(_fTimeDelta);
		break;
	}
	}

	if (m_wpParticle_Point.lock()->GetIsFinish())
		m_bUse = false;

	return CActor::Update(_fTimeDelta);
}

_int CParticleActor::LateUpdate(const _float& _fTimeDelta)
{
	return CActor::LateUpdate(_fTimeDelta);
}

void CParticleActor::Render()
{
	if (FAILED(BindDefaultShaderResources()))
		return;

	if (FAILED(m_wpParticle_Point.lock()->BeginShader(0)))
		return;

	m_wpParticle_Point.lock()->BindBuffers();

	CActor::Render();
}

void CParticleActor::EndPlay()
{
	CActor::EndPlay();
}

void CParticleActor::Release()
{
	CActor::Release();
}

HRESULT CParticleActor::ReadParticleFile_JSON(const wstring& _wstrJsonFileName)
{
	ifstream json_path(L"../../Resources/Data/Effect/Particle/" + _wstrJsonFileName + L".json");

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value Particle;

	JSONCPP_STRING errs;
	bool bOk = parseFromStream(builder, json_path, &Particle, &errs);

	if (bOk)
	{
		strcpy_s(m_InstanceDesc.szDiffuseTextureFileName, Particle["02. Texture FileName"].asCString());
		strcpy_s(m_InstanceDesc.szShaderFileName, Particle["03. Shader FileName"].asCString());
		m_InstanceDesc.eParticleType = (PARTICLE_TYPE)(Particle["04. Particle Type"].asInt());
		m_InstanceDesc.eRenderGroup = (RENDER_GROUP)(Particle["05. Render Group"].asInt());
		m_InstanceDesc.isLoop = Particle["06. Loop"].asBool();
		m_InstanceDesc.isBillBoard = Particle["07. BillBoard"].asBool();

		m_InstanceDesc.iNumInstance = Particle["08. NumInstance"].asInt();

		m_InstanceDesc.vPivot.x = Particle["09. Pivot"][0].asFloat();
		m_InstanceDesc.vPivot.y = Particle["09. Pivot"][1].asFloat();
		m_InstanceDesc.vPivot.z = Particle["09. Pivot"][2].asFloat();

		m_InstanceDesc.vCenter.x = Particle["10. Center"][0].asFloat();
		m_InstanceDesc.vCenter.y = Particle["10. Center"][1].asFloat();
		m_InstanceDesc.vCenter.z = Particle["10. Center"][2].asFloat();

		m_InstanceDesc.vRange.x = Particle["11. Range"][0].asFloat();
		m_InstanceDesc.vRange.y = Particle["11. Range"][1].asFloat();
		m_InstanceDesc.vRange.z = Particle["11. Range"][2].asFloat();

		m_InstanceDesc.vRadius.x = Particle["18. Radius"][0].asFloat();
		m_InstanceDesc.vRadius.y = Particle["18. Radius"][1].asFloat();

		m_InstanceDesc.vRotate.x = Particle["19. Rotate"][0].asFloat();
		m_InstanceDesc.vRotate.y = Particle["19. Rotate"][1].asFloat();
		m_InstanceDesc.vRotate.z = Particle["19. Rotate"][2].asFloat();

		m_InstanceDesc.vOffset.x = Particle["12. Offset"][0].asFloat();
		m_InstanceDesc.vOffset.y = Particle["12. Offset"][1].asFloat();
		m_InstanceDesc.vOffset.z = Particle["12. Offset"][2].asFloat();

		m_InstanceDesc.vSize.x = Particle["13. Size"][0].asFloat();
		m_InstanceDesc.vSize.y = Particle["13. Size"][1].asFloat();

		m_InstanceDesc.vSpeed.x = Particle["14. Speed"][0].asFloat();
		m_InstanceDesc.vSpeed.y = Particle["14. Speed"][1].asFloat();

		m_InstanceDesc.vColor.x = Particle["15. Color"][0].asFloat();
		m_InstanceDesc.vColor.y = Particle["15. Color"][1].asFloat();
		m_InstanceDesc.vColor.z = Particle["15. Color"][2].asFloat();
		m_InstanceDesc.vColor.w = Particle["15. Color"][3].asFloat();

		m_InstanceDesc.vLifeTime.x = Particle["16. LifeTime"][0].asFloat();
		m_InstanceDesc.vLifeTime.y = Particle["16. LifeTime"][1].asFloat();

		m_InstanceDesc.fDuration = Particle["17. Duration"].asFloat();
	}
	else
	{
		MSG_BOX("Json 파일 로드에 실패했습니다. : CParticleActor");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CParticleActor::BindDefaultShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpParticle_Point.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpParticle_Point.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpParticle_Point.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	_float4 vCamPosition;
	XMStoreFloat4(&vCamPosition, CCameraManager::GetInstance()->GetCamPosition());

	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_wpParticle_Point.lock()->BindShaderResource("g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_vColor", &m_InstanceDesc.vColor, sizeof(_float4))))
		return E_FAIL;

	_float fTimeAcc = m_wpParticle_Point.lock()->GetTimeAcc();
	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_fTimeAcc", &fTimeAcc, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_vLifeTime", &m_InstanceDesc.vLifeTime, sizeof(_float2))))
		return E_FAIL;

	return S_OK;
}
