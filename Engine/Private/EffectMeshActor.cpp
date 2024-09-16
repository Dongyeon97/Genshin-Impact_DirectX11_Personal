#include "Engine_pch.h"
#include "EffectMeshActor.h"

#include "CameraManager.h"

#include "EffectMesh.h"
#include "CameraComponent.h"

// 툴 용
void CEffectMeshActor::Initialize()
{
	m_eRenderGroup = m_EffectMeshDesc.eRenderGruop;
	m_wstrModelFileName = L"";
	for (_uint i = 0; i < MAX_PATH; ++i)
	{
		if (m_EffectMeshDesc.szModelFileName[i] == '\0') { break; }
		m_wstrModelFileName += m_EffectMeshDesc.szModelFileName[i];
	}

	// Scale / Rotation / Translate
	XMFLOAT3 vScale = m_EffectMeshDesc.vScale;
	XMStoreFloat4(&m_vScale, XMVectorSet(vScale.x, vScale.y, vScale.z, 0.0f));

	XMFLOAT3 vRotation = m_EffectMeshDesc.vRotation;
	XMStoreFloat4(&m_vRotation, XMVectorSet(vRotation.x, vRotation.y, vRotation.z, 0.0f));

	XMFLOAT3 vTranslate = m_EffectMeshDesc.vTranslate;
	XMStoreFloat4(&m_vTranslate, XMVectorSet(vTranslate.x, vTranslate.y, vTranslate.z, 0.0f));

	m_vOriginScale = m_vScale;
	m_vOriginRotation = m_vRotation;

	// 이펙트 메시 컴포넌트를 생성하고, 구조체를 전송
	m_wpEffectMesh = CreateActorComponent<CEffectMesh>(L"EffectMesh Component");
	m_wpEffectMesh.lock()->SetEffectMeshDesc(m_EffectMeshDesc);
	m_wpEffectMesh.lock()->SetModelFileName(m_wstrModelFileName);

	CMeshActor::Initialize();
}

// Json 파싱용
HRESULT CEffectMeshActor::Initialize(const wstring& _wstrJsonFileName)
{
	m_wpEffectMesh = CreateActorComponent<CEffectMesh>(L"EffectMesh Component");

	if (FAILED(ReadEffectMeshFile_JSON(_wstrJsonFileName)))
		return E_FAIL;

	m_wpEffectMesh.lock()->SetEffectMeshDesc(m_EffectMeshDesc);

	CMeshActor::Initialize();
	return S_OK;
}

void CEffectMeshActor::RecycleBegin()
{
	m_bUse = true;
	m_fLifeTimeAcc = 0.0f;
	m_vUVMoveDistance = { 0.0f, 0.0f };

	m_vScale = m_vOriginScale;
	m_vRotation = m_vOriginRotation;
}

void CEffectMeshActor::Replay()
{
	m_bUse = true;
	m_fLifeTimeAcc = 0.0f;
	m_vUVMoveDistance = { 0.0f, 0.0f };
}

_int CEffectMeshActor::PreUpdate(const _float& _fTimeDelta)
{
	return CMeshActor::PreUpdate(_fTimeDelta);
}

_int CEffectMeshActor::Update(const _float& _fTimeDelta)
{
	if (m_bPause)
		return S_OK;

	m_fLifeTimeAcc += _fTimeDelta;

	if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		m_bUse = false;
		return S_OK;
	}
	else if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.x)
	{
		if (m_EffectMeshDesc.bTransformChange)
			TransformChange(_fTimeDelta);
	}

	m_fTimeDelta = _fTimeDelta;

	return CMeshActor::Update(_fTimeDelta);
}

_int CEffectMeshActor::LateUpdate(const _float& _fTimeDelta)
{
	return CMeshActor::LateUpdate(_fTimeDelta);
}

void CEffectMeshActor::Render()
{
	if (m_fLifeTimeAcc < m_EffectMeshDesc.vLifeTime.x)
		return;

	if (FAILED(BindShaderResources()))
		return;

	_uint iNumMeshes = m_wpEffectMesh.lock()->GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (m_EffectMeshDesc.bUseDiffuseTexture)
		{
			if (FAILED(m_wpEffectMesh.lock()->BindEffectTexture(DIFFUSE_TEXTURE1, "g_DiffuseTexture1")))
				return;
			if (FAILED(m_wpEffectMesh.lock()->BindEffectTexture(DIFFUSE_TEXTURE2, "g_DiffuseTexture2")))
				return;
		}
		
		if (m_EffectMeshDesc.bUseNoiseTexture)
		{
			if (FAILED(m_wpEffectMesh.lock()->BindEffectTexture(NOISE_TEXTURE, strNoiseTexture.c_str())))
				return;
		}

		if (m_EffectMeshDesc.bUseMaskTexture)
		{ 
			if (FAILED(m_wpEffectMesh.lock()->BindEffectTexture(MASK_TEXTURE, strMaskTexture.c_str())))
				return;
		}

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUVMode", &m_EffectMeshDesc.bUVMoveMode, sizeof(_bool))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUseDiffuse", &m_EffectMeshDesc.bUseDiffuseTexture, sizeof(_bool))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUseNoise", &m_EffectMeshDesc.bUseNoiseTexture, sizeof(_bool))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUseMask", &m_EffectMeshDesc.bUseMaskTexture, sizeof(_bool))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_fLifeTimeAcc", &m_fLifeTimeAcc, sizeof(_float))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_fLifeTime", &m_EffectMeshDesc.vLifeTime, sizeof(_float2))))
			return;

		if (FAILED(m_wpEffectMesh.lock()->BeginShader(0)))
			return;

		m_wpEffectMesh.lock()->Render(i);
	}

	CMeshActor::Render();
}

void CEffectMeshActor::EndPlay()
{
	CMeshActor::EndPlay();
}

void CEffectMeshActor::Release()
{
	CMeshActor::Release();
}

HRESULT CEffectMeshActor::BindShaderResources()
{
	_float4x4		WorldMatrix, ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&WorldMatrix, GetWorldMatrix());
	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	m_wpEffectMesh.lock()->BindMatrix("g_WorldMatrix", &WorldMatrix);
	m_wpEffectMesh.lock()->BindMatrix("g_ViewMatrix", &ViewMatrix);
	m_wpEffectMesh.lock()->BindMatrix("g_ProjMatrix", &ProjMatrix);

	_float fFar = CCameraManager::GetInstance()->GetCurCamera()->GetFar();
	m_wpEffectMesh.lock()->BindRawValue("g_fFar", &fFar, sizeof(_float));
	
	if (!m_bPause && m_EffectMeshDesc.bUVMoveMode)
	{
		m_vUVMoveDistance.x += m_EffectMeshDesc.vUVMoveSpeed.x * m_fTimeDelta;
		m_vUVMoveDistance.y += m_EffectMeshDesc.vUVMoveSpeed.y * m_fTimeDelta;
	}

	// [ Diffuse Color ]
	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_vDiffuseColor1", &m_EffectMeshDesc.vDiffuseBaseColor, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_vDiffuseColor2", &m_EffectMeshDesc.vDiffuseLerpColor, sizeof(_float4))))
		return E_FAIL;

	// [ UV Move ]
	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUVReverse", &m_EffectMeshDesc.bUVMoveReverse, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_bUVLoop", &m_EffectMeshDesc.bUVMoveLoop, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_fUVSpeedX", &m_vUVMoveDistance.x, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_wpEffectMesh.lock()->BindRawValue("g_fUVSpeedY", &m_vUVMoveDistance.y, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectMeshActor::ReadEffectMeshFile_JSON(const wstring& _wstrJsonFileName)
{
	ifstream json_path(L"../../Resources/Data/Effect/EffectMesh/" + _wstrJsonFileName + L".json");

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value EffectMesh;

	JSONCPP_STRING errs;
	bool bOk = parseFromStream(builder, json_path, &EffectMesh, &errs);

	if (bOk)
	{
		/* 1. 기본 정보 */
		m_EffectMeshDesc.eModelType = (MODEL_TYPE)EffectMesh["02. ModelType"].asInt();
		m_EffectMeshDesc.eRenderGruop = (RENDER_GROUP)EffectMesh["03. RenderGroup"].asInt();
		m_eRenderGroup = m_EffectMeshDesc.eRenderGruop;

		XMStoreFloat4(&m_vScale, XMVectorSet(EffectMesh["04. Scale"][0].asFloat(), EffectMesh["04. Scale"][1].asFloat(), EffectMesh["04. Scale"][2].asFloat(), 0.0f));
		XMStoreFloat4(&m_vRotation, XMVectorSet(EffectMesh["05. Rotation"][0].asFloat(), EffectMesh["05. Rotation"][1].asFloat(), EffectMesh["05. Rotation"][2].asFloat(), 0.0f));
		XMStoreFloat4(&m_vTranslate, XMVectorSet(EffectMesh["06. Translate"][0].asFloat(), EffectMesh["06. Translate"][1].asFloat(), EffectMesh["06. Translate"][2].asFloat(), 1.0f));

		m_EffectMeshDesc.vScale.x = m_vScale.x;
		m_EffectMeshDesc.vScale.y = m_vScale.y;
		m_EffectMeshDesc.vScale.z = m_vScale.z;
		m_vOriginScale = m_vScale;
		
		m_EffectMeshDesc.vRotation.x = m_vRotation.x;
		m_EffectMeshDesc.vRotation.y = m_vRotation.y;
		m_EffectMeshDesc.vRotation.z = m_vRotation.z;
		m_vOriginRotation = m_vRotation;

		m_EffectMeshDesc.vTranslate.x = m_vTranslate.x;
		m_EffectMeshDesc.vTranslate.y = m_vTranslate.y;
		m_EffectMeshDesc.vTranslate.z = m_vTranslate.z;

		m_EffectMeshDesc.vLifeTime.x = EffectMesh["07. LifeTime"][0].asFloat();
		m_EffectMeshDesc.vLifeTime.y = EffectMesh["07. LifeTime"][1].asFloat();

		m_EffectMeshDesc.bUVMoveMode = EffectMesh["08. UV Move Mode"].asBool();
		m_EffectMeshDesc.bUVMoveReverse = EffectMesh["09. UV Move Reverse"].asBool();
		m_EffectMeshDesc.bUVMoveLoop = EffectMesh["10. UV Move Loop"].asBool();
		m_EffectMeshDesc.vUVMoveSpeed.x = EffectMesh["11. UV MoveSpeed"][0].asFloat();
		m_EffectMeshDesc.vUVMoveSpeed.y = EffectMesh["11. UV MoveSpeed"][1].asFloat();

		strcpy_s(m_EffectMeshDesc.szModelFileName, EffectMesh["12. Effect Mesh FileName"].asCString());
		strcpy_s(m_EffectMeshDesc.szModelFilePath, EffectMesh["13. Effect Mesh FilePath"].asCString());
		strcpy_s(m_EffectMeshDesc.szShaderFileName, EffectMesh["14. Shader FileName"].asCString());

		strcpy_s(m_EffectMeshDesc.szDiffuseTextureFileName1, EffectMesh["15. Diffuse Texture1"].asCString());
		strcpy_s(m_EffectMeshDesc.szDiffuseTextureFileName2, EffectMesh["16. Diffuse Texture2"].asCString());
		strcpy_s(m_EffectMeshDesc.szNoiseTextureFileName, EffectMesh["17. Noise Texture"].asCString());
		strcpy_s(m_EffectMeshDesc.szMaskTextureFileName, EffectMesh["18. Mask Texture"].asCString());

		m_EffectMeshDesc.vDiffuseBaseColor.x = EffectMesh["19. Diffuse Color1"][0].asFloat();
		m_EffectMeshDesc.vDiffuseBaseColor.y = EffectMesh["19. Diffuse Color1"][1].asFloat();
		m_EffectMeshDesc.vDiffuseBaseColor.z = EffectMesh["19. Diffuse Color1"][2].asFloat();
		m_EffectMeshDesc.vDiffuseBaseColor.w = EffectMesh["19. Diffuse Color1"][3].asFloat();

		m_EffectMeshDesc.vDiffuseLerpColor.x = EffectMesh["20. Diffuse Color2"][0].asFloat();
		m_EffectMeshDesc.vDiffuseLerpColor.y = EffectMesh["20. Diffuse Color2"][1].asFloat();
		m_EffectMeshDesc.vDiffuseLerpColor.z = EffectMesh["20. Diffuse Color2"][2].asFloat();
		m_EffectMeshDesc.vDiffuseLerpColor.w = EffectMesh["20. Diffuse Color2"][3].asFloat();
		
		m_EffectMeshDesc.bUseDiffuseTexture = EffectMesh["21. Use Diffuse Texture"].asBool();
		m_EffectMeshDesc.bUseNoiseTexture = EffectMesh["22. Use Noise Texture"].asBool();
		m_EffectMeshDesc.bUseMaskTexture = EffectMesh["23. Use Mask Texture"].asBool();

		m_EffectMeshDesc.bTransformChange = EffectMesh["25. Use Transform Change"].asBool();
		m_EffectMeshDesc.vDestScale.x = EffectMesh["26. Dest Scale"][0].asFloat();
		m_EffectMeshDesc.vDestScale.y = EffectMesh["26. Dest Scale"][1].asFloat();
		m_EffectMeshDesc.vDestScale.z = EffectMesh["26. Dest Scale"][2].asFloat();
		m_EffectMeshDesc.fScaleChangeRatio = EffectMesh["27. Scale Change Ratio"].asFloat();
		m_EffectMeshDesc.vRotatePerSecond.x = EffectMesh["28. Rotate Per Second"][0].asFloat();
		m_EffectMeshDesc.vRotatePerSecond.y = EffectMesh["28. Rotate Per Second"][1].asFloat();
		m_EffectMeshDesc.vRotatePerSecond.z = EffectMesh["28. Rotate Per Second"][2].asFloat();

		/* 2. 메시 */
		m_EffectMeshDesc.iNumMeshes = EffectMesh["24. Mesh"]["NumMeshs"].asInt();

		m_EffectMeshDesc.vecMeshs.reserve(m_EffectMeshDesc.iNumMeshes);
		m_EffectMeshDesc.vecMeshs.resize(m_EffectMeshDesc.iNumMeshes);
		for (_uint i = 0; i < m_EffectMeshDesc.iNumMeshes; ++i)
		{
			strcpy_s(m_EffectMeshDesc.vecMeshs[i].szName, EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["MeshName"].asCString());
			
			m_EffectMeshDesc.vecMeshs[i].iNumVertices = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["NumVertices"].asInt();
			m_EffectMeshDesc.vecMeshs[i].iNumFaces = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["NumFaces"].asInt();
			
			m_EffectMeshDesc.vecMeshs[i].vecVertices.reserve(m_EffectMeshDesc.vecMeshs[i].iNumVertices);
			m_EffectMeshDesc.vecMeshs[i].vecVertices.resize(m_EffectMeshDesc.vecMeshs[i].iNumVertices);
			for (_uint j = 0; j < m_EffectMeshDesc.vecMeshs[i].iNumVertices; ++j)
			{
				_float3 vVertexPos;
				vVertexPos.x = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexPos"][0].asFloat();
				vVertexPos.y = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexPos"][1].asFloat();
				vVertexPos.z = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexPos"][2].asFloat();
				m_EffectMeshDesc.vecMeshs[i].vecVertices[j].vPosition = vVertexPos;
				
				_float3 vVertexNor;
				vVertexNor.x = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexNor"][0].asFloat();
				vVertexNor.y = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexNor"][1].asFloat();
				vVertexNor.z = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexNor"][2].asFloat();
				m_EffectMeshDesc.vecMeshs[i].vecVertices[j].vNormal = vVertexNor;

				_float2 vVertexTexCoord;
				vVertexTexCoord.x = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexTexCoord"][0].asFloat();
				vVertexTexCoord.y = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexTexCoord"][1].asFloat();
				m_EffectMeshDesc.vecMeshs[i].vecVertices[j].vTexcoord = vVertexTexCoord;

				_float3 vVertexTangent;
				vVertexTangent.x = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexTangent"][0].asFloat();
				vVertexTangent.y = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexTangent"][1].asFloat();
				vVertexTangent.z = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecVertices"][j]["VertexTangent"][2].asFloat();
				m_EffectMeshDesc.vecMeshs[i].vecVertices[j].vTangent = vVertexTangent;
			}

			m_EffectMeshDesc.vecMeshs[i].vecFaces.reserve(m_EffectMeshDesc.vecMeshs[i].iNumFaces);
			m_EffectMeshDesc.vecMeshs[i].vecFaces.resize(m_EffectMeshDesc.vecMeshs[i].iNumFaces);
			for (_uint j = 0; j < m_EffectMeshDesc.vecMeshs[i].iNumFaces; ++j)
			{
				m_EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[0] = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecIndices"][j][0].asInt();
				m_EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[1] = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecIndices"][j][1].asInt();
				m_EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[2] = EffectMesh["24. Mesh"]["Mesh_" + to_string(i)]["vecIndices"][j][2].asInt();
			}
		}
	}
	else
	{
		MSG_BOX("Json 파일 로드에 실패했습니다. : CEffectMeshActor");
		return E_FAIL;
	}

	return S_OK;
}

void CEffectMeshActor::TransformChange(const _float& _fTimeDelta)
{
	if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.y && !m_EffectMeshDesc.bUVMoveLoop)
	{
		m_bUse = false;
		return;
	}
	else if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.x)
	{
		// X Scale
		if (m_vScale.x < m_EffectMeshDesc.vDestScale.x)
		{
			m_vScale.x += m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.x >= m_EffectMeshDesc.vDestScale.x)
				m_vScale.x = m_EffectMeshDesc.vDestScale.x;
		}
		else
		{
			m_vScale.x -= m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.x <= m_EffectMeshDesc.vDestScale.x)
				m_vScale.x = m_EffectMeshDesc.vDestScale.x;
		}

		// Y Scale
		if (m_vScale.y < m_EffectMeshDesc.vDestScale.y)
		{
			m_vScale.y += m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.y >= m_EffectMeshDesc.vDestScale.y)
				m_vScale.y = m_EffectMeshDesc.vDestScale.y;
		}
		else
		{
			m_vScale.y -= m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.y <= m_EffectMeshDesc.vDestScale.y)
				m_vScale.y = m_EffectMeshDesc.vDestScale.y;
		}

		// Z Scale
		if (m_vScale.z < m_EffectMeshDesc.vDestScale.z)
		{
			m_vScale.z += m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.z >= m_EffectMeshDesc.vDestScale.z)
				m_vScale.z = m_EffectMeshDesc.vDestScale.z;
		}
		else
		{
			m_vScale.z -= m_EffectMeshDesc.fScaleChangeRatio * _fTimeDelta;

			if (m_vScale.z <= m_EffectMeshDesc.vDestScale.z)
				m_vScale.z = m_EffectMeshDesc.vDestScale.z;
		}

		m_vRotation.x += m_EffectMeshDesc.vRotatePerSecond.x * _fTimeDelta;
		m_vRotation.y += m_EffectMeshDesc.vRotatePerSecond.y * _fTimeDelta;
		m_vRotation.z += m_EffectMeshDesc.vRotatePerSecond.z * _fTimeDelta;
	}
}
