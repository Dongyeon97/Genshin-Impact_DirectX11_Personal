#pragma once
#include "Window.h"

BEGIN(Engine)
class CActor;
END

class CLevel_Editor;

class CInspector : public CWindow
{
public:
	explicit CInspector() = default;
	virtual ~CInspector() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(std::weak_ptr<CLevel> _wpLevel);
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void EndPlay();
	void Render();
	void Release();

/********************
	Methods
********************/
private:
#pragma region Actor Tab

/* Actor �� */
	void UpdateActorTab();
		void UpdateCameraInfo();
		void Picking();
		void UpdateSRT();
		void UpdatePrefabList();

		void UpdateCreateButton();
			void CreateAnimActor();
			void CreateNonAnimActor();

		void UpdateSaveButton();
			void SaveData();
			void SaveModelData();
			void SaveSpawnData();

		void UpdateLoadButton();
			void LoadData(const wstring& _wstrDataFileName);
			void LoadModelData(const wstring& _wstrDataFileName);
			void LoadSpawnData(const wstring& _wstrDataFileName);

private:
	/* �� Create */
	MY_AISCENE CreateNonAnimModelAIScene(const aiScene* _pAIScene);
	MY_AISCENE CreateAnimModelAIScene(const aiScene* _pAIScene1);

	HRESULT ReadyBones(MY_AINODE* _pMyaiNode, aiNode* _pNode);

#pragma endregion Actor Tab

#pragma region Navigation Tab

/* Navigation �� */
private:
	void UpdateNavigationTab();
		void Picking_Navi();
		void UpdatePickedPosition();
		void UpdateNavigationSaveButton();
		void UpdateRemoveLatestCellButton();

public:
	void SetPickedPosition(_vector _vecPickedPos, _uint _iClickCount)
	{
		_float3 vPickedPosition;
		XMStoreFloat3(&vPickedPosition, _vecPickedPos);
		m_vPickedPos[_iClickCount] = vPickedPosition;
	}

#pragma endregion Navigation Tab

#pragma region Effect Tab

/* Effect �� */
private:
	void UpdateEffectTab();
		void UpdateParticleTab();
			void UpdateParticleLoadFromJSON();
			void UpdateParticleOption();
			void UpdateParticleShaderFileTable();
			void UpdateColorRelative();
			void UpdateParticleSRT();
			void UpdateTimeRelative();
			void UpdateSpeedRelative();
			void UpdateSpriteRelative();
			void UpdateParticleCreateButton();
			void UpdateSpriteCreateButton();
			void UpdateParticleSaveButton();

		void UpdateMeshTab();
			void UpdateEffectMeshLoadFromJSON();
			void UpdateModelTypeRadio();
			void UpdateEffectMeshRenderGroupCombo();
			void UpdateEffectMeshTable();
			void UpdateEffectMeshShaderFileTable();
			void UpdateDiffuseColorTable();
			void UpdateDiffuseTextureTable();
			void UpdateNoiseTextureTable();
			void UpdateMaskTextureTable();
			void UpdateEffectMeshUVMove();
			void UpdateEffectMeshSRT();
			void UpdateEffectMeshTransformChange();
			void UpdateEffectMeshLifeTime();
			void UpdateEffectMeshCreateButton();
			void UpdateEffectMeshSaveButton();

#pragma endregion Effect Tab

/********************
	Data Members
********************/
#pragma region Actor Tab
// ===================================== Actor Tab =================================== //
private:
	static _uint g_iSpawnCount;

	_bool my_tool_active = true;
	_float3 m_vCameraPos;

// ============================== Actor S / R / T ==================================== //
private:
	_float3 m_vScale = {};
	_float3 m_vRotation = {};
	_float3 m_vTranslate = {};

// =================================== ������ ������ ================================= //
private:
	_char m_chActorName[64];

	_int m_iModelTypeRadio;

	_int m_iRenderGroupIndex;
	RENDER_GROUP m_eRenderGroup = RENDER_GROUP::RENDER_END;

	_int m_iLayerIndex;
	LAYER m_eLayer = LAYER::LAYER_END;

// ===================================== ���� ��� ==================================== //
private:
	_int m_iCurModelItemIndex = 0;

	// char* Ÿ���� NonAnim �� ���� ���
	vector<const char*> m_vecNonAnimModelFilePaths;

	// char* Ÿ���� Anim �� ���� ���
	vector<const char*> m_vecAnimModelFilePaths;

// ==================================================================================== //
private:
	// ������ ������ ���� �̸�
	_char m_chFileName[64];

// ===================================���� ����======================================= //
private:
	_int m_iMonsterVariationIndex = 0;
	MONSTER_VARIATION m_eMonVariation = NONE_MONSTER;

#pragma endregion Actor Tab

#pragma region Navigation Tab
// ===================================�׺���̼�======================================= //
private:
	_uint m_iClickCount = 0;	// ������ ���� ���������� 0���� �ʱ�ȭ

	_float3 m_vPickedPos[3] = {};

	// �׺���̼� ������ ���� �̸�
	_char m_chNaviFileName[64];

#pragma endregion Navigation Tab

#pragma region Effect Tab
// ================================================================================ //
// =================================== Particle =================================== //
// ================================================================================ //
private:
	_char m_chParticleFileName[64] = {};

	vector<const char*> m_vecDiffuseTexturePaths;
	_int m_iNumInstance = 1;

	_int m_iParticleImageSelector = 0;
	_int m_iParticleShaderFileSelector = 0;

	_bool m_bParticleLoop = false;
	_bool m_bParticleBillBoard = false;

	_int m_iParticleRenderGroupIndex = 2;
	RENDER_GROUP m_eParticleRenderGroup = RENDER_GROUP::RENDER_END;
	
	_int m_iParticleTypeRadio = 0;
	PARTICLE_TYPE m_eParticleType = PARTICLE_TYPE::PARTICLE_END;

// =================================== Particle S / R / T =========================== //
private:
	_float3 m_vParticleRotation = {};
	_float3 m_vParticleOffset = {};

	_float2 m_vParticleSizeMinMax = {};

	_float3 m_vParticleRange = {};
	_float2 m_vParticleRadius = {};
	_float3 m_vParticlePivot = {};

// =================================== Time Relative ================================ //
private:
	_float	m_fParticleDuration = 0.0f;
	_float2 m_vParticleLifeTime = {};

// =================================== Speed Relative =============================== //
private:
	_float2 m_vParticleSpeed = {};

// =================================== Color Relative =============================== //
private:
	_float4 m_vParticleColor = { 0.0f, 0.0f, 0.0f, 1.0f };

// =================================== Color Relative =============================== //
private:
	_float2 m_vSpriteRowCol = { 1.f, 1.f };
	_float  m_fSpriteSpeed = 0.f;

// =================================== JSON ========================================= //
private:
	_char m_chParticleJSONFileName[64];



// ================================================================================ //
// =================================== EffectMesh ================================= //
// ================================================================================ //
private:
	// [ ����Ʈ �޽� ���� �̸� ]
	_char m_chEffectMeshFileName[64] = {};

	// [ ����Ʈ �޽� Load�� ���� JSON ���ϸ� ]
	_char m_chEffectMeshJSONFileName[64];

	// [ ����Ʈ �޽� ��� ]
	vector<const char*> m_vecEffectMeshFilePaths;

	// [ ���̴� ���� ��� ]
	vector<const char*> m_vecShaderPaths;

	// [ �ؽ�ó ��� ]
	// vector<const char*> m_vecDiffuseTexturePaths;
	vector<const char*> m_vecNoiseTexturePaths;
	vector<const char*> m_vecMaskTexturePaths;

	_int m_iEffectMeshSelector = 0;
	_int m_iEffectMeshShaderFileSelector = 0;

	_float4 m_vEffectMeshDiffuseColor1 = {0.0f, 0.0f, 0.0f, 1.0f};
	_float4 m_vEffectMeshDiffuseColor2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	_bool m_bUseDiffuseTexture = false;
	_int m_iDiffuseTextureSelector1 = 0;
	_int m_iDiffuseTextureSelector2 = 0;

	_bool m_bUseNoiseTexture = false;
	_int m_iNoiseTextureSelector = 0;

	_bool m_bUseMaskTexture = true;
	_int m_iMaskTextureSelector = 0;

	// [ ���� �׷� ]
	_int m_iEffectMeshRenderGroup = 0;
	RENDER_GROUP m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_END;

	// [ �� Ÿ�� ]
	_int m_iEffectMeshModelType = 0;
	MODEL_TYPE m_eEffectMeshModelType = MODEL_TYPE::MODEL_END;

	// [ SRT ]
	_float3 m_vEffectMeshScale = {};
	_float3 m_vEffectMeshRotation = {};
	_float3 m_vEffectMeshOffset = {};

	// [ Transform Change ]
	_bool	m_bEffectMeshTransformChange = false;
	_float3 m_vEffectMeshDestScale = { 1.f, 1.f, 1.f };
	_float	m_fEffectMeshScaleChangeRatio = 0.f;
	_float3 m_vEffectMeshRotatePerSecond = {};

	// [ LifeTime ]
	_float2 m_vEffectMeshLifeTime = {};

	// [ UV Move Mode on / off]
	_bool m_bEffectMeshUVMove = true;

	// [ UV Move Reverse on / off ]
	_bool m_bEffectMeshUVReverse = true;

	// [ UV Move Loop on / off ]
	_bool m_bEffectMeshUVMoveLoop = true;

	// [ UV Move Speed ]
	_float2 m_vEffectMeshUVSpeed = {};

#pragma endregion Effect Tab
};
