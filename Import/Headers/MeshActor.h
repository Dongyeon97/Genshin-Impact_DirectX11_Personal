#pragma once
#include "Actor.h"

BEGIN(Engine)

class ENGINE_DLL CMeshActor : public CActor
{
public:
	explicit CMeshActor() = default;
	virtual ~CMeshActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void Render_Shadow() override;
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual HRESULT BindShaderResources() = 0;

/********************
	Getter / Setter
********************/
public:
	void SetModelData(MODEL_DATA& _ModelData) { m_ModelData = _ModelData; }
	void SetSpawnData(SPAWN_DATA& _SpawnData) { m_SpawnData = _SpawnData; }

	void SetModelFileName(const wstring _wstrModelFileName) { m_wstrModelFileName = _wstrModelFileName; }
	const wstring& GetModelFileName() { return m_wstrModelFileName;}

/********************
	Data Members
********************/
protected:
	////////////////////////////////////////////////////
	/* 셰이더로 던져주기 위한 변수들의 ConstantName들 */
	////////////////////////////////////////////////////
	// -> 나중에 클라에서 지정할 수 있게끔 바꾸면 좋을 것 같음 - 01.08 -
	string strWorldMatrix = "g_WorldMatrix";
	string strViewMatrix = "g_ViewMatrix";
	string strProjMatrix = "g_ProjMatrix";

	string strCamPosition = "g_vCamPosition";

	string strLightDiffuse = "g_vLightDiffuse";
	string strLightAmbient = "g_vLightAmbient";
	string strLightSpecular = "g_vLightSpecular";
	string strLightDir = "g_vLightDir";

	// 이 중에서 있는 텍스처만 바인딩 하는 식으로 코드를 구성해야 함(비트플래그?)
	string strDiffuseTexture = "g_DiffuseTexture1";
	string strNoiseTexture = "g_NoiseTexture";
	string strMaskTexture = "g_MaskTexture";
	string strAmbientTexture = "g_AmbientTexture";
	string strSpecularTexture = "g_SpecularTexture";

	string strBoneMatrices = "g_BoneMatrices";

public:
	wstring m_wstrModelFileName = L"";

	MODEL_DATA m_ModelData;
	SPAWN_DATA m_SpawnData;
};

END