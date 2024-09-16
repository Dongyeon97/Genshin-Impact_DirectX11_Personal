#include "pch.h"
#include "Inspector.h"

/* Manager */
#include "ObjectManager.h"
#include "ResourceManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "AssimpManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "NavigationManager.h"

/* Engine */
#include "Level_Editor.h"
#include "Layer.h"
#include "FreeCam.h"
#include "Actor.h"
#include "AnimActor.h"
#include "NonAnimActor.h"
#include "Editor_Navi.h"
#include "ParticleActor.h"
#include "SpriteActor.h"
#include "EffectMeshActor.h"

#include "AnimModel.h"
#include "NonAnimModel.h"
#include "Navigation.h"
#include "Cell.h"
#include "Particle_Point.h"

_uint CInspector::g_iSpawnCount = 0;

HRESULT CInspector::Initialize(std::weak_ptr<CLevel> _wpLevel)
{
	m_wpEditorLevel = dynamic_pointer_cast<CLevel_Editor>(_wpLevel.lock());

	// ===============================================논애님 모델 경로======================================================= //
	const vector<string>& vecStrNonAnimModelFilePaths = CResourceManager::GetInstance()->GetStrFilePaths(RESOURCE_NONANIMMODEL);
	for (auto& strFilePath : vecStrNonAnimModelFilePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecNonAnimModelFilePaths.push_back(strFilePath.c_str());
	}

	// ===============================================애님 모델 경로======================================================== //
	const vector<string>& vecStrAnimModelFilePaths = CResourceManager::GetInstance()->GetStrFilePaths(RESOURCE_ANIMMODEL);
	for (auto& strFilePath : vecStrAnimModelFilePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecAnimModelFilePaths.push_back(strFilePath.c_str());
	}

	// ===============================================Diffuse 텍스처 파일 경로============================================== //
	const vector<string>& vecStrDiffuseTexturePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_DIFFUSETEXTURE);
	for (auto& strFilePath : vecStrDiffuseTexturePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecDiffuseTexturePaths.push_back(strFilePath.c_str());
	}

	// ===============================================Noise 텍스처 파일 경로============================================== //
	const vector<string>& vecStrNoiseTexturePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_NOISETEXTURE);
	for (auto& strFilePath : vecStrNoiseTexturePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecNoiseTexturePaths.push_back(strFilePath.c_str());
	}

	// ===============================================Mask 텍스처 파일 경로============================================== //
	const vector<string>& vecStrMaskTexturePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_MASKTEXTURE);
	for (auto& strFilePath : vecStrMaskTexturePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecMaskTexturePaths.push_back(strFilePath.c_str());
	}

	// ===============================================이펙트 메시 파일 경로============================================== //
	const vector<string>& vecStrEffectMeshFilePaths = CResourceManager::GetInstance()->GetStrFilePaths(RESOURCE_EFFECTMESH);
	for (auto& strFilePath : vecStrEffectMeshFilePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecEffectMeshFilePaths.push_back(strFilePath.c_str());
	}

	// ===============================================셰이더 파일 경로============================================== //
	const vector<string>& vecStrShaderFilePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_SHADER);
	for (auto& strFilePath : vecStrShaderFilePaths)
	{
		size_t iCursor = strFilePath.find_last_of(".");
		string strExtension = strFilePath.substr(iCursor + 1);

		m_vecShaderPaths.push_back(strFilePath.c_str());
	}

	return CWindow::Initialize(_wpLevel);
}

_int CInspector::PreUpdate(const _float& _fTimeDelta)
{
	return CWindow::PreUpdate(_fTimeDelta);
}

_int CInspector::Update(const _float& _fTimeDelta)
{
	if (ImGui::Begin("Inspector", &my_tool_active))
	{
		ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("TabBar", tabBarFlags))
		{
			
			if (ImGui::BeginTabItem("Actor"))
			{
				UpdateActorTab();
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Navi"))
			{
				UpdateNavigationTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Effect"))
			{
				UpdateEffectTab();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
	
	return CWindow::Update(_fTimeDelta);
}

_int CInspector::LateUpdate(const _float& _fTimeDelta)
{
	return CWindow::LateUpdate(_fTimeDelta);
}

void CInspector::EndPlay()
{
	CWindow::EndPlay();
}

void CInspector::Render()
{
	CWindow::Render();
}

void CInspector::Release()
{
	CWindow::Release();
}

/* Model */
void CInspector::UpdateActorTab()
{
	if (ImGui::CollapsingHeader("File"))
	{
		UpdateSaveButton();
		UpdateLoadButton();
	}
	UpdateCameraInfo();
	Picking();
	UpdateSRT();
	UpdatePrefabList();
	UpdateCreateButton();
}
void CInspector::UpdateCameraInfo()
{
	ImGui::SeparatorText("Camera Transform");

	_vector vPlayerPosXYZ = CObjectManager::GetInstance()->GetCurLevel()->FindLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->GetPositionVector();

	_float3 vPos;
	XMStoreFloat3(&vPos, vPlayerPosXYZ);

	m_vCameraPos.x = vPos.x;
	m_vCameraPos.y = vPos.y;
	m_vCameraPos.z = vPos.z;

	ImGui::InputFloat3("Cam Position", &m_vCameraPos.x);
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}
void CInspector::Picking()
{
	if (CInputManager::GetInstance()->IsMouseDown(EMouseButtonType::RIGHT_BUTTON))
	{
		shared_ptr<CActor> spCam = CObjectManager::GetInstance()->FindActor(m_wpEditorLevel.lock()->GetLevelIndex(), LAYER::LAYER_PLAYER, L"Actor_Player");

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		_vector vPickedPos = CCollisionManager::GetInstance()->IntersectRayToModel(spCam->GetActorTranslation(), pt, Tool::g_iWidth, Tool::g_iHeight);

		// 제대로 피킹이 됬다면
		if (0.f != XMVectorGetX(vPickedPos) && 0.f != XMVectorGetY(vPickedPos) && 0.f != XMVectorGetZ(vPickedPos))
			XMStoreFloat3(&m_vTranslate, vPickedPos);
	}
}
void CInspector::UpdateSRT()
{
	ImGui::SeparatorText("Create Transform");
	ImGui::Spacing();

	ImGui::InputFloat3("Translate", &m_vTranslate.x);
	ImGui::InputFloat3("Rotate", &m_vRotation.x);
	ImGui::InputFloat3("Scale", &m_vScale.x);
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}
void CInspector::UpdatePrefabList()
{
	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	ImGui::SeparatorText("Prefab Info");
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// =====================================액터 이름======================================= //
	ImGui::InputTextWithHint("Actor Name", "Actor Name", m_chActorName, IM_ARRAYSIZE(m_chActorName));
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// =====================================모델 타입================================== //
	ImGui::RadioButton("NonAnim", &m_iModelTypeRadio, 0); ImGui::SameLine();
	ImGui::RadioButton("Anim", &m_iModelTypeRadio, 1);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// =====================================모델 경로================================== //
	if (0 == m_iModelTypeRadio)
	{
		ImGui::Combo("ModelFile Path", &m_iCurModelItemIndex, m_vecNonAnimModelFilePaths.data(), (_int)m_vecNonAnimModelFilePaths.size());
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
	else if(1 == m_iModelTypeRadio)
	{
		ImGui::Combo("ModelFile Path", &m_iCurModelItemIndex, m_vecAnimModelFilePaths.data(), (_int)m_vecAnimModelFilePaths.size());
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
	// =====================================렌더 그룹================================== //
	array<const char*, 5> arrRenderGroup =
	{
	"RENDER_PRIORITY",
	"RENDER_NONBLEND",
	"RENDER_BLEND",
	"RENDER_ORTHOGONAL",
	"RENDER_UI",
	};

	ImGui::Combo("RenderGroup", &m_iRenderGroupIndex, arrRenderGroup.data(), (_uint)arrRenderGroup.size());

	switch (m_iRenderGroupIndex)
	{
	case 0: m_eRenderGroup = RENDER_GROUP::RENDER_PRIORITY; break;
	case 1: m_eRenderGroup = RENDER_GROUP::RENDER_NONBLEND; break;
	case 2: m_eRenderGroup = RENDER_GROUP::RENDER_BLEND; break;
	case 3: m_eRenderGroup = RENDER_GROUP::RENDER_ORTHOGONAL; break;
	case 4: m_eRenderGroup = RENDER_GROUP::RENDER_UI; break;
	default: break;
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// =======================================레이어====================================== //
	array<const char*, 4> arrLayer =
	{
	"LAYER_PLAYER",
	"LAYER_MONSTER",
	"LAYER_TERRAIN",
	"LAYER_OBJECT",
	};

	ImGui::Combo("Layer", &m_iLayerIndex, arrLayer.data(), (_uint)arrLayer.size());

	switch (m_iLayerIndex)
	{
	case 0: m_eLayer = LAYER::LAYER_PLAYER; break;
	case 1: m_eLayer = LAYER::LAYER_MONSTER; break;
	case 2: m_eLayer = LAYER::LAYER_TERRAIN; break;
	case 3: m_eLayer = LAYER::LAYER_OBJECT; break;
	default: break;
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	// ======================================몬스터 종류======================================= //
	array<const char*, 6> arrMonsterVariation =
	{
	"NONE_MONSTER",
	"SLIME_ICE",
	"SLIME_FIRE",
	"HILI_ICE",
	"HILI_FIRE",
	"DEFENDER",
	};

	ImGui::Combo("Monster Variation", &m_iMonsterVariationIndex, arrMonsterVariation.data(), (_uint)arrMonsterVariation.size());

	switch (m_iMonsterVariationIndex)
	{
	case 0: m_eMonVariation = MONSTER_VARIATION::NONE_MONSTER; break;
	case 1: m_eMonVariation = MONSTER_VARIATION::SLIME_ICE; break;
	case 2: m_eMonVariation = MONSTER_VARIATION::SLIME_FIRE; break;
	case 3: m_eMonVariation = MONSTER_VARIATION::HILI_ICE; break;
	case 4: m_eMonVariation = MONSTER_VARIATION::HILI_FIRE; break;
	case 5: m_eMonVariation = MONSTER_VARIATION::DEFENDER; break;
	default: break;
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void CInspector::UpdateCreateButton()
{
	if (ImGui::Button("Create Actor", ImVec2(100, 30)))
	{
		if (m_wpEditorLevel.expired())
		{
			MSG_BOX("m_wpEditorLevel is expired : CInspector");
			return;
		}

		switch (m_iModelTypeRadio)
		{
			// NonAnim Actor 생성
		case 0:
		{
			CreateNonAnimActor();
			break;
		}
		case 1:
		{
			// Anim Actor 생성
			CreateAnimActor();
			break;
		}
		}
		ImGui::Spacing();
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}
void CInspector::CreateAnimActor()
{
	wstring wstrActorName = L"";
	string strActorName = "";

	for (_uint i = 0; i < 64; ++i)
	{
		if (m_chActorName[i] == '\0') { break; }
		wstrActorName += m_chActorName[i];
		strActorName += m_chActorName[i];
	}

	if (nullptr != CObjectManager::GetInstance()->FindActor(m_wpEditorLevel.lock()->GetLevelIndex(), m_iLayerIndex, wstrActorName))
	{
		// 이름 중복이 발생할 경우
		wstrActorName += to_wstring(g_iSpawnCount);
		strActorName += to_string(g_iSpawnCount);
		g_iSpawnCount++;
	}

	_char szFileName[MAX_PATH] = "";
	_splitpath_s(m_vecAnimModelFilePaths[m_iCurModelItemIndex], nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

	// 모델 파일 이름을 wstring으로 추출
	wstring wstrFileName = L"";

	for (_uint i = 0; i < MAX_PATH; ++i)
	{
		if (szFileName[i] == '\0') { break; }
		wstrFileName += szFileName[i];
	}

	// 1. 툴에서 입력된 정보를 토대로 프리팹 데이터를 생성한다.
	SPAWN_DATA tMySpawnData = {};
	MY_PREFAB& tMyPrefab = tMySpawnData.tPrefab;

	tMyPrefab.iLayerIndex = m_iLayerIndex;
	strcpy_s(tMyPrefab.szActorName, strActorName.c_str());
	tMyPrefab.eRenderGroup = m_eRenderGroup;
	tMyPrefab.eModelType = (MODEL_TYPE)m_iModelTypeRadio;
	tMyPrefab.eMonsterVariation = (MONSTER_VARIATION)m_eMonVariation;
	strcpy_s(tMyPrefab.szShaderFileName, "Shader_VtxAnimMesh");
	strcpy_s(tMyPrefab.szModelFileName, szFileName);

	tMyPrefab.vScale = m_vScale;
	tMyPrefab.vRotation = m_vRotation;
	tMyPrefab.vTranslate = m_vTranslate;

	// 2. ★★★★★ <액터 이름, 스폰 데이터>를 SpawnManager에 추가한다. ★★★★★
	/*CSpawnManager::GetInstance()->AddSpawnData(strActorName, tMySpawnData);*/

	// 3. AssimpManager에 모델 정보(파일경로, 레벨타입, 모델타입)를 전달하여 aiScene을 생성한다.


	const aiScene* pAIScene = nullptr;
	MY_AISCENE tMyAISceneAnim;

	if (nullptr == CPrefabManager::GetInstance()->GetModelData(wstrFileName))
	{
		// 이미 생성된 프리팹이 없는 경우
		// 4.1 aiScene을 토대로 MY_AISCENE_ANIM을 생성한다.
		const aiScene* pAIScene = CAssimpManager::GetInstance()->CreateModelFromFile(m_vecAnimModelFilePaths[m_iCurModelItemIndex], MODEL_TYPE::MODEL_ANIM);

		tMyAISceneAnim = CreateAnimModelAIScene(pAIScene);
	}
	else
	{
		// 이미 생성된 프리팹이 있는 경우
		// 4.2 있는 프리팹 구조체를 가져온다.
		tMyAISceneAnim = CPrefabManager::GetInstance()->GetModelData(wstrFileName)->tAIScenes;
	}

	// 5. MODEL_DATA 구조체를 생성한다.
	MODEL_DATA tModelData_Anim;

	tModelData_Anim.eModelType = (MODEL_TYPE)m_iModelTypeRadio;
	tModelData_Anim.tAIScenes = tMyAISceneAnim;
	strcpy_s(tModelData_Anim.szModelFilePath, m_vecAnimModelFilePaths[m_iCurModelItemIndex]);
	strcpy_s(tModelData_Anim.szModelFileName, szFileName);

	// 6. ★★★★★ <모델 파일 이름, 모델 데이터>를 PrefabManager에 추가한다. ★★★★★
	// CPrefabManager::GetInstance()->AddModelData(wstrFileName, tModelData_Anim);

	// 7. 객체를 생성한다.
	shared_ptr<CAnimActor> spAnimActor = m_wpEditorLevel.lock()->GetLayer(m_eLayer)->CreateActor<CAnimActor>(wstrActorName);

	spAnimActor->SetModelData(tModelData_Anim);
	spAnimActor->SetSpawnData(tMySpawnData);
	spAnimActor->Initialize();

	CSpawnManager::GetInstance()->AddSpawnData(strActorName, tMySpawnData);
	CPrefabManager::GetInstance()->AddModelData(wstrFileName, tModelData_Anim);
}
MY_AISCENE CInspector::CreateAnimModelAIScene(const aiScene* _pAIScene1)
{
	MY_AISCENE tAnimAIScene;

	/* 본 */
	// 루트 본 하나를 넣으면
	// [ 재귀적으로 뼈를 생성 ] 
	ReadyBones(&tAnimAIScene.tRootBone, _pAIScene1->mRootNode);

	/* 애니메이션 */
	tAnimAIScene.iNumAnimations = _pAIScene1->mNumAnimations;
	tAnimAIScene.vecAnimations.resize(tAnimAIScene.iNumAnimations);

	for (_uint i = 0; i < _pAIScene1->mNumAnimations; ++i)
	{
		strcpy_s(tAnimAIScene.vecAnimations[i].szName, _pAIScene1->mAnimations[i]->mName.data);

		memcpy(&tAnimAIScene.vecAnimations[i].Duration, &_pAIScene1->mAnimations[i]->mDuration, sizeof(double));
		memcpy(&tAnimAIScene.vecAnimations[i].TickPerSecond, &_pAIScene1->mAnimations[i]->mTicksPerSecond, sizeof(double));

		memcpy(&tAnimAIScene.vecAnimations[i].iNumChannels, &_pAIScene1->mAnimations[i]->mNumChannels, sizeof(unsigned int));

		tAnimAIScene.vecAnimations[i].vecChannels.resize(tAnimAIScene.vecAnimations[i].iNumChannels);
		for (_uint j = 0; j < tAnimAIScene.vecAnimations[i].iNumChannels; ++j)
		{
			strcpy_s(tAnimAIScene.vecAnimations[i].vecChannels[j].szName, _pAIScene1->mAnimations[i]->mChannels[j]->mNodeName.data);

			memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys, &_pAIScene1->mAnimations[i]->mChannels[j]->mNumScalingKeys, sizeof(unsigned int));
			memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys, &_pAIScene1->mAnimations[i]->mChannels[j]->mNumRotationKeys, sizeof(unsigned int));
			memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys, &_pAIScene1->mAnimations[i]->mChannels[j]->mNumPositionKeys, sizeof(unsigned int));

			tAnimAIScene.vecAnimations[i].vecChannels[j].vecScalingKeys.resize(tAnimAIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys);
			for (_uint k = 0; k < tAnimAIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys; ++k)
			{
				memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].vecScalingKeys[k], &_pAIScene1->mAnimations[i]->mChannels[j]->mScalingKeys[k], sizeof(MY_VECTORKEY));
			}

			tAnimAIScene.vecAnimations[i].vecChannels[j].vecRotationKeys.resize(tAnimAIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys);
			for (_uint k = 0; k < tAnimAIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys; ++k)
			{
				memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].vecRotationKeys[k], &_pAIScene1->mAnimations[i]->mChannels[j]->mRotationKeys[k], sizeof(MY_QUATKEY));
			}

			tAnimAIScene.vecAnimations[i].vecChannels[j].vecPositionKeys.resize(tAnimAIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys);
			for (_uint k = 0; k < tAnimAIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys; ++k)
			{
				memcpy(&tAnimAIScene.vecAnimations[i].vecChannels[j].vecPositionKeys[k], &_pAIScene1->mAnimations[i]->mChannels[j]->mPositionKeys[k], sizeof(MY_VECTORKEY));
			}
		}
	}

	/* 메시 */
	tAnimAIScene.iNumMeshes = _pAIScene1->mNumMeshes;
	tAnimAIScene.vecMeshs.resize(tAnimAIScene.iNumMeshes);

	for (_uint i = 0; i < _pAIScene1->mNumMeshes; ++i)
	{
		MY_MESH tMyMesh;

		strcpy_s(tMyMesh.szName, _pAIScene1->mMeshes[i]->mName.data);

		tMyMesh.iMaterialIndex = _pAIScene1->mMeshes[i]->mMaterialIndex;

		tMyMesh.iNumVertices = _pAIScene1->mMeshes[i]->mNumVertices;
		tMyMesh.vecVertices.resize(tMyMesh.iNumVertices);

		tMyMesh.iNumFaces = _pAIScene1->mMeshes[i]->mNumFaces;
		tMyMesh.vecFaces.resize(tMyMesh.iNumFaces);

		for (_uint j = 0; j < tMyMesh.iNumVertices; ++j)
		{
			memcpy(&tMyMesh.vecVertices[j].vPosition, &_pAIScene1->mMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&tMyMesh.vecVertices[j].vNormal, &_pAIScene1->mMeshes[i]->mNormals[j], sizeof(_float3));
			memcpy(&tMyMesh.vecVertices[j].vTexcoord, &_pAIScene1->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
			memcpy(&tMyMesh.vecVertices[j].vTangent, &_pAIScene1->mMeshes[i]->mTangents[j], sizeof(_float3));
		}

		/* 이 메시의 정점들이 상태를 받아와야하는(이 메시에 영향을 주는) 뼈들의 전체 갯수 .*/
		tMyMesh.iNumBones = _pAIScene1->mMeshes[i]->mNumBones;
		tMyMesh.vecAIBone.resize(tMyMesh.iNumBones);

		for (_uint j = 0; j < tMyMesh.iNumBones; ++j)
		{
			aiBone* pAIBone = _pAIScene1->mMeshes[i]->mBones[j];

			strcpy_s(tMyMesh.vecAIBone[j].szBoneName, pAIBone->mName.data);
			memcpy(&tMyMesh.vecAIBone[j].matOffset, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

			tMyMesh.vecAIBone[j].iNumWeights = pAIBone->mNumWeights;
			tMyMesh.vecAIBone[j].vecWeights.resize(tMyMesh.vecAIBone[j].iNumWeights);

			for (_uint k = 0; k < tMyMesh.vecAIBone[j].iNumWeights; ++k)
			{
				tMyMesh.vecAIBone[j].vecWeights[k].iVertexId = pAIBone->mWeights[k].mVertexId;
				tMyMesh.vecAIBone[j].vecWeights[k].fWeight = pAIBone->mWeights[k].mWeight;
			}
		}

		_uint iNumIndices = 0;

		for (_uint j = 0; j < tMyMesh.iNumFaces; ++j)
		{
			tMyMesh.vecFaces[j].mIndices[0] = _pAIScene1->mMeshes[i]->mFaces[j].mIndices[0];
			tMyMesh.vecFaces[j].mIndices[1] = _pAIScene1->mMeshes[i]->mFaces[j].mIndices[1];
			tMyMesh.vecFaces[j].mIndices[2] = _pAIScene1->mMeshes[i]->mFaces[j].mIndices[2];
		}

		tAnimAIScene.vecMeshs[i] = tMyMesh;
	}

	/* 재질 */
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";

	string strFilePath = m_vecAnimModelFilePaths[m_iCurModelItemIndex];
	_splitpath_s(strFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	tAnimAIScene.iNumMaterials = _pAIScene1->mNumMaterials;
	tAnimAIScene.vecMaterials.resize(tAnimAIScene.iNumMaterials);

	for (_uint i = 0; i < tAnimAIScene.iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = _pAIScene1->mMaterials[i];
		// (tAnimAIScene.vecMaterials[i].vecFilePath).resize(AI_TEXTURE_TYPE_MAX);

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			// pAIMaterial->GetTextureCount(aiTextureType(j));
			aiString			strTextureFilePath;
			strcpy_s(tAnimAIScene.vecMaterials[i].szTextureFilePath[j], ""); // 문자열을 비워둔다.

			// 텍스처 파일 경로에서 [파일명]과 [확장자]만 추출한다.
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			// [드라이브] + [폴더] + [파일명] + [확장자] 를 조합해 텍스처 파일의 총 경로를 만든다.
			_char				szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			// 멀티바이트를 와이드캐릭터형으로 형변환
			_tchar				szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, (_uint)strlen(szTmp), szFullPath, MAX_PATH);

			strcpy_s(tAnimAIScene.vecMaterials[i].szTextureFilePath[j], szTmp);
		}
	}

	return tAnimAIScene;
}
HRESULT CInspector::ReadyBones(MY_AINODE* _pMyaiNode, aiNode* _pNode)
{
	strcpy_s(_pMyaiNode->szBoneName, _pNode->mName.data);
	memcpy(&_pMyaiNode->matTransformation, &_pNode->mTransformation, sizeof(_float4x4));

	_pMyaiNode->iNumChildren = _pNode->mNumChildren;
	_pMyaiNode->vecChildren.resize(_pMyaiNode->iNumChildren);

	for (_uint i = 0; i < _pMyaiNode->iNumChildren; ++i)
	{
		ReadyBones(&_pMyaiNode->vecChildren[i], _pNode->mChildren[i]);
	}

	return S_OK;
}

void CInspector::CreateNonAnimActor()
{
	wstring wstrActorName = L"";
	string strActorName = "";

	for (_uint i = 0; i < 64; ++i)
	{
		if (m_chActorName[i] == '\0') { break; }
		wstrActorName += m_chActorName[i];
		strActorName += m_chActorName[i];
	}

	if (nullptr != CObjectManager::GetInstance()->FindActor(m_wpEditorLevel.lock()->GetLevelIndex(), m_iLayerIndex, wstrActorName))
	{
		// 이름 중복이 발생할 경우
		wstrActorName += to_wstring(g_iSpawnCount);
		strActorName += to_string(g_iSpawnCount);
		g_iSpawnCount++;
	}

	_char szFileName[MAX_PATH] = "";
	_splitpath_s(m_vecNonAnimModelFilePaths[m_iCurModelItemIndex], nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

	wstring wstrFileName = L"";

	for (_uint i = 0; i < MAX_PATH; ++i)
	{
		if (szFileName[i] == '\0') { break; }
		wstrFileName += szFileName[i];
	}

	// 1. 입력된 정보를 토대로 프리팹 데이터를 생성한다.
	SPAWN_DATA tMySpawnData = {};
	MY_PREFAB& tMyPrefab = tMySpawnData.tPrefab;

	tMyPrefab.iLayerIndex = m_iLayerIndex;
	strcpy_s(tMyPrefab.szActorName, strActorName.c_str());
	tMyPrefab.eRenderGroup = m_eRenderGroup;
	tMyPrefab.eModelType = (MODEL_TYPE)m_iModelTypeRadio;
	tMyPrefab.eMonsterVariation = (MONSTER_VARIATION)m_eMonVariation;
	strcpy_s(tMyPrefab.szShaderFileName, "Shader_VtxMesh");
	strcpy_s(tMyPrefab.szModelFileName, szFileName);

	tMyPrefab.vScale = m_vScale;
	tMyPrefab.vRotation = m_vRotation;
	tMyPrefab.vTranslate = m_vTranslate;

	// 2. ★★★★★ <액터 이름, 스폰 데이터>를 SpawnManager에 추가한다. ★★★★★
	/*CSpawnManager::GetInstance()->AddSpawnData(strActorName, tMySpawnData);*/

	// 3. AssimpManager에 모델 정보(파일경로, 레벨타입, 모델타입)를 전달하여 aiScene을 생성한다.
	const aiScene* pAIScene = nullptr;
	MY_AISCENE tMyAISceneNonAnim;

	if (nullptr == CPrefabManager::GetInstance()->GetModelData(wstrFileName))
	{
		// 이미 생성된 프리팹이 없는 경우
		// 4.1 aiScene을 토대로 MY_AISCENE_ANIM을 생성한다.
		const aiScene* pAIScene = CAssimpManager::GetInstance()->CreateModelFromFile(m_vecNonAnimModelFilePaths[m_iCurModelItemIndex], MODEL_TYPE::MODEL_NONANIM);

		tMyAISceneNonAnim = CreateNonAnimModelAIScene(pAIScene);
	}
	else
	{
		// 이미 생성된 프리팹이 있는 경우
		// 4.2 있는 프리팹 구조체를 가져온다.
		tMyAISceneNonAnim = CPrefabManager::GetInstance()->GetModelData(wstrFileName)->tAIScenes;
	}

	// 5. MODEL_DATA 구조체를 생성한다.
	MODEL_DATA tModelData_NonAnim;

	tModelData_NonAnim.eModelType = (MODEL_TYPE)m_iModelTypeRadio;
	tModelData_NonAnim.tAIScenes = tMyAISceneNonAnim;
	strcpy_s(tModelData_NonAnim.szModelFilePath, m_vecNonAnimModelFilePaths[m_iCurModelItemIndex]);
	strcpy_s(tModelData_NonAnim.szModelFileName, szFileName);


	// 6. ★★★★★ <모델 파일 이름, 모델 데이터>를 PrefabManager에 추가한다. ★★★★★
	// CPrefabManager::GetInstance()->AddModelData(wstrFileName, tModelData_NonAnim);

	// 7. 객체를 생성한다.
	shared_ptr<CNonAnimActor> spNonAnimActor = m_wpEditorLevel.lock()->GetLayer(m_eLayer)->CreateActor<CNonAnimActor>(wstrActorName);

	spNonAnimActor->SetModelData(tModelData_NonAnim);
	spNonAnimActor->SetSpawnData(tMySpawnData);
	spNonAnimActor->Initialize();

	CSpawnManager::GetInstance()->AddSpawnData(strActorName, tMySpawnData);
	CPrefabManager::GetInstance()->AddModelData(wstrFileName, tModelData_NonAnim);
	CCollisionManager::GetInstance()->AddNonAnimModel(spNonAnimActor->GetNonAnimModel());
}
MY_AISCENE CInspector::CreateNonAnimModelAIScene(const aiScene* _pAIScene)
{
	MY_AISCENE tNonAnimAIScene;

	/* 본 - 빈 정보 */
	char szBoneName[64] = "None";
	strcpy_s(tNonAnimAIScene.tRootBone.szBoneName, szBoneName);
	tNonAnimAIScene.tRootBone.iNumChildren = 0;
	XMStoreFloat4x4(&tNonAnimAIScene.tRootBone.matTransformation, XMMatrixIdentity());

	/* 애니메이션 - 빈 정보 */
	tNonAnimAIScene.iNumAnimations = 0;

	/* 메시 */
	tNonAnimAIScene.iNumMeshes = _pAIScene->mNumMeshes;
	tNonAnimAIScene.vecMeshs.resize(tNonAnimAIScene.iNumMeshes);

	for (_uint i = 0; i < _pAIScene->mNumMeshes; ++i)
	{
		MY_MESH tMyMesh;

		strcpy_s(tMyMesh.szName, _pAIScene->mMeshes[i]->mName.data);

		tMyMesh.iMaterialIndex = _pAIScene->mMeshes[i]->mMaterialIndex;

		tMyMesh.iNumVertices = _pAIScene->mMeshes[i]->mNumVertices;
		tMyMesh.vecVertices.resize(tMyMesh.iNumVertices);

		tMyMesh.iNumFaces = _pAIScene->mMeshes[i]->mNumFaces;
		tMyMesh.vecFaces.resize(tMyMesh.iNumFaces);

		for (_uint j = 0; j < tMyMesh.iNumVertices; ++j)
		{
			memcpy(&tMyMesh.vecVertices[j].vPosition, &_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
			memcpy(&tMyMesh.vecVertices[j].vNormal, &_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));

			if (nullptr == _pAIScene->mMeshes[i]->mTextureCoords[0])
				continue;
			memcpy(&tMyMesh.vecVertices[j].vTexcoord, &_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));

			if (nullptr == _pAIScene->mMeshes[i]->mTangents)
				continue;
			memcpy(&tMyMesh.vecVertices[j].vTangent, &_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));
		}

		for (_uint j = 0; j < tMyMesh.iNumFaces; ++j)
		{
			tMyMesh.vecFaces[j].mIndices[0] = _pAIScene->mMeshes[i]->mFaces[j].mIndices[0];
			tMyMesh.vecFaces[j].mIndices[1] = _pAIScene->mMeshes[i]->mFaces[j].mIndices[1];
			tMyMesh.vecFaces[j].mIndices[2] = _pAIScene->mMeshes[i]->mFaces[j].mIndices[2];
		}

		tNonAnimAIScene.vecMeshs[i] = tMyMesh;
	}

	/* 재질 */
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";

	string strFilePath = m_vecNonAnimModelFilePaths[m_iCurModelItemIndex];
	_splitpath_s(strFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	tNonAnimAIScene.iNumMaterials = _pAIScene->mNumMaterials;
	tNonAnimAIScene.vecMaterials.resize(tNonAnimAIScene.iNumMaterials);

	for (_uint i = 0; i < tNonAnimAIScene.iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = _pAIScene->mMaterials[i];

		for (_uint j = 0; j < TEXTURE_TYPE::TRANSMISSION; ++j)
		{
			// pAIMaterial->GetTextureCount(aiTextureType(j));
			aiString			strTextureFilePath;
			strcpy_s(tNonAnimAIScene.vecMaterials[i].szTextureFilePath[j], ""); // 문자열을 비워둔다.

			// 텍스처 파일 경로에서 [파일명]과 [확장자]만 추출한다.
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			// [드라이브] + [폴더] + [파일명] + [확장자] 를 조합해 텍스처 파일의 총 경로를 만든다.
			_char				szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			// 멀티바이트를 와이드캐릭터형으로 형변환
			_tchar				szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, (_uint)strlen(szTmp), szFullPath, MAX_PATH);

			// tNonAnimAIScene.vecMaterials[i].vecFilePath[j] = szFullPath;
			strcpy_s(tNonAnimAIScene.vecMaterials[i].szTextureFilePath[j], szTmp);
		}
	}

	return tNonAnimAIScene;
}

void CInspector::UpdateSaveButton()
{
	ImGui::SeparatorText("Level DataFile");
	ImGui::Spacing();

	ImGui::InputTextWithHint("DataFile Name", "File Name", m_chFileName, IM_ARRAYSIZE(m_chFileName));
	if (ImGui::Button("Save", ImVec2(100, 30)))
	{
		SaveModelData();
		SaveSpawnData();
	}
}
void CInspector::SaveData()
{
	SaveModelData();
	SaveSpawnData();
}
void CInspector::SaveModelData()
{
	// 1. 저장하기 버튼을 누르면, 입력된 텍스트에서 파일 이름을 추출한다.
	wstring wstrDataFileName = L"";
	for (_uint i = 0; i < IM_ARRAYSIZE(m_chFileName); ++i)
	{
		if (m_chFileName[i] == '\0') { break; }
		else { wstrDataFileName += m_chFileName[i]; }
	}

	// 2. 리소스 매니저에서 데이터 파일이 들어있는 경로를 가져온다.
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	// 3. 프리팹 매니저와 스폰 매니저에서 데이터를 불러온다.
	const vector<pair<wstring, MODEL_DATA>>& vecModelData = CPrefabManager::GetInstance()->GetPrefabDatas();

	// 4. [ 파일경로 + 파일이름 + _Prafab.dat ]으로 파일을 연다.(없다면 생성한다.)
	ofstream ostrWriteFile(wstrDataPath + wstrDataFileName + L"_Prefab" + L".txt", ios::out | ios::binary);

	// 5. 파일이 열려 있다면, 프리팹 데이터를 1바이트씩 파일에 기록한다.
	if (ostrWriteFile.is_open())
	{
		for (const auto& ModelData : vecModelData)
		{
			///////////////////////////////////////
			// ★★★★★ 모델 데이터 ★★★★★ //
			///////////////////////////////////////

			// 1. 모델 데이터 구조체 생성한다.
			MODEL_DATA Data;
			Data = ModelData.second;

			// 2. [파일 경로]와 [모델타입]은 고정된 크기를 가지기 때문에, 구조체 통째로 기록한다.
			ostrWriteFile.write((char*)(&Data.szModelFilePath), MAX_PATH);
			ostrWriteFile.write((char*)(&Data.szModelFileName), MAX_PATH);
			ostrWriteFile.write((char*)(&Data.eModelType), sizeof(MODEL_TYPE));

			// 3. [AI_Scene]은 가변적인 데이터가 포함되어 있기 때문에, 모델마다 가변적으로 기록한다.
			MY_AISCENE& AIScene = Data.tAIScenes;

			// == 3.1 애니메이션 ==========================================================================
			ostrWriteFile.write((char*)(&AIScene.iNumAnimations), sizeof(unsigned int));

			for (_uint i = 0; i < AIScene.iNumAnimations; ++i)
			{
				ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].szName), MAX_PATH);

				ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].Duration), sizeof(double));
				ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].TickPerSecond), sizeof(double));
				ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].iNumChannels), sizeof(unsigned int));

				for (_uint j = 0; j < AIScene.vecAnimations[i].iNumChannels; ++j)
				{
					ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].szName), MAX_PATH);

					ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys), sizeof(unsigned int));
					ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys), sizeof(unsigned int));
					ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys), sizeof(unsigned int));

					for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys; ++k)
					{
						ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecScalingKeys[k]), sizeof(MY_VECTORKEY));
					}

					for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys; ++k)
					{
						ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecRotationKeys[k]), sizeof(MY_QUATKEY));
					}

					for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys; ++k)
					{
						ostrWriteFile.write((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecPositionKeys[k]), sizeof(MY_VECTORKEY));
					}
				}
			}

			// == 3.2 본 ==========================================================================
			MY_AINODE& RootNode = AIScene.tRootBone;

			if (MODEL_ANIM == Data.eModelType)
			{
				// 이름
				ostrWriteFile.write(RootNode.szBoneName, MAX_PATH);

				// TransformationMatrix
				ostrWriteFile.write((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));

				// iNumChildren(자식 본 개수)
				ostrWriteFile.write((char*)(&RootNode.iNumChildren), sizeof(unsigned int));

				// vecChildren 재귀적으로 저장
				CPrefabManager::GetInstance()->WriteBones(RootNode, ostrWriteFile);
			}
			else
			{
				// 논애님 모델의 경우 본 데이터가 없으므로 빈 값으로 초기화하여 기록한다.
				char szName[MAX_PATH] = "";
				XMFLOAT4X4 matTransformation = {};
				unsigned int iNumChildren = 0;

				ostrWriteFile.write(szName, MAX_PATH);
				ostrWriteFile.write((char*)(&matTransformation), sizeof(XMFLOAT4X4));
				ostrWriteFile.write((char*)(&iNumChildren), sizeof(unsigned int));
			}

			// == 3.3 메시 ==========================================================================

			// 메시 개수
			ostrWriteFile.write((char*)(&AIScene.iNumMeshes), sizeof(unsigned int));

			// 메시
			for (_uint i = 0; i < AIScene.iNumMeshes; ++i)
			{
				ostrWriteFile.write((char*)(&AIScene.vecMeshs[i].szName), MAX_PATH);

				_uint iMesh_MaterialIndex = AIScene.vecMeshs[i].iMaterialIndex;	// 메시가 사용하는 재질의 인덱스
				_uint iMesh_NumVertices = AIScene.vecMeshs[i].iNumVertices;		// 정점 개수
				_uint iMesh_NumFaces = AIScene.vecMeshs[i].iNumFaces;			// 삼각형 개수
				_uint iMesh_NumBones = AIScene.vecMeshs[i].iNumBones;			// 메시에 영향을 주는 본의 개수

				ostrWriteFile.write((char*)(&iMesh_MaterialIndex), sizeof(_uint));
				ostrWriteFile.write((char*)(&iMesh_NumVertices), sizeof(_uint));
				ostrWriteFile.write((char*)(&iMesh_NumFaces), sizeof(_uint));

				if (MODEL_NONANIM == Data.eModelType)
				{
					unsigned int iZeroBone = 0;
					iMesh_NumBones = iZeroBone;
					ostrWriteFile.write((char*)(&iZeroBone), sizeof(_uint));
				}
				else
					ostrWriteFile.write((char*)(&iMesh_NumBones), sizeof(_uint));

				// i번 째 메시의 j번 째 Vertex 기록
				for (_uint j = 0; j < iMesh_NumVertices; ++j)
				{
					ostrWriteFile.write((char*)(&AIScene.vecMeshs[i].vecVertices[j]), sizeof(VTXANIMMESH));
				}

				// i번 째 메시의 인덱스 기록
				for (_uint j = 0; j < iMesh_NumFaces; ++j)
				{
					ostrWriteFile.write((char*)(&AIScene.vecMeshs[i].vecFaces[j]), sizeof(MY_AIFACE));
				}

				// i번째 메시에 영향을 주는 j번 째 Bone 기록
				for (_uint j = 0; j < iMesh_NumBones; ++j)
				{
					MY_AIBONE& AIBone = AIScene.vecMeshs[i].vecAIBone[j];

					ostrWriteFile.write((char*)(&AIBone.szBoneName), MAX_PATH);

					ostrWriteFile.write((char*)(&AIBone.matOffset), sizeof(XMFLOAT4X4));
					ostrWriteFile.write((char*)(&AIBone.iNumWeights), sizeof(unsigned int));
					for (_uint Index = 0; Index < AIBone.iNumWeights; ++Index)
					{
						ostrWriteFile.write((char*)(&AIBone.vecWeights[Index]), sizeof(MY_WEIGHT));
					}
				}
			}


			// == 3.4 재질 ==========================================================================

			// 재질 개수
			ostrWriteFile.write((char*)(&AIScene.iNumMaterials), sizeof(unsigned int));

			// 재질
			_uint iNumMaterials = AIScene.iNumMaterials;
			for (_uint i = 0; i < iNumMaterials; ++i)
			{
				ostrWriteFile.write((char*)(&AIScene.vecMaterials[i]), sizeof(MY_AIMATERIAL));
			}
		}
		// 6. 열렸던 파일을 닫아준다.
		ostrWriteFile.close();
	}
}
void CInspector::SaveSpawnData()
{
	// 1. 저장하기 버튼을 누르면, 입력된 텍스트에서 파일 이름을 추출한다.
	wstring wstrDataFileName = L"";
	for (_uint i = 0; i < IM_ARRAYSIZE(m_chFileName); ++i)
	{
		if (m_chFileName[i] == '\0') { break; }
		else { wstrDataFileName += m_chFileName[i]; }
	}

	// 2. 리소스 매니저에서 데이터 파일이 들어있는 경로를 가져온다.
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	// 3. 스폰 매니저에서 데이터를 불러온다.
	const vector<pair<string, SPAWN_DATA>>& vecSpawnData = CSpawnManager::GetInstance()->GetSpawnData();

	// 4. [ 파일경로 + 파일이름 + _Spawn.dat ]으로 파일을 연다.(없다면 생성한다.)
	ofstream ostrWriteFile(wstrDataPath + wstrDataFileName + L"_Spawn" + L".txt", ios::out | ios::binary);

	// 5. 파일이 열려 있다면, 스폰 데이터를 1바이트씩 파일에 기록한다.
	if (ostrWriteFile.is_open())
	{
		for (const auto& SpawnData : vecSpawnData)
		{
			ostrWriteFile.write((char*)(&SpawnData.second), sizeof(SPAWN_DATA));
		}
		// 6. 열렸던 파일을 닫아준다.
		ostrWriteFile.close();
	}
}

void CInspector::UpdateLoadButton()
{
	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(100, 30)))
	{
		wstring wstrDataFileName = L"";
		for (_uint i = 0; i < IM_ARRAYSIZE(m_chFileName); ++i)
		{
			if (m_chFileName[i] == '\0') { break; }
			else { wstrDataFileName += m_chFileName[i]; }
		}

		LoadData(wstrDataFileName);
	}
}
void CInspector::LoadData(const wstring& _wstrDataFileName)
{
	// 기존 카메라의 위치정보를 저장한다.
// _vector vScale = m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->GetScale();
	_vector vRotation = m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->GetRotation();
	_vector vTranslate = m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->GetActorTranslation();
	CPrefabManager::GetInstance()->ClearCurrentScene(m_wpEditorLevel.lock()->GetLevelIndex());
	CCollisionManager::GetInstance()->ClearCurrentScene(m_wpEditorLevel.lock()->GetLevelIndex());
	CCameraManager::GetInstance()->RemoveCamera(L"Player_Camera");

	LoadModelData(_wstrDataFileName);
	LoadSpawnData(_wstrDataFileName);

	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->CreateActor<FreeCam>(L"Actor_Player");
	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->SetRotation(vRotation);
	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->SetActorTranslation_NoNavigation(vTranslate);
	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_PLAYER)->FindActor(L"Actor_Player")->Initialize();

	CNavigationManager::GetInstance()->RemoveNavigation(L"Navigation_Editor");
	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_TERRAIN)->CreateActor<CEditor_Navi>(L"Actor_Navigation");
	m_wpEditorLevel.lock()->GetLayer(LAYER::LAYER_TERRAIN)->FindActor(L"Actor_Navigation")->Initialize();
}
void CInspector::LoadModelData(const wstring& _wstrDataFileName)
{
	// 1. 에디터 레벨의 모든 오브젝트를 정리한다.
	m_wpEditorLevel.lock()->ReleaseObject();

	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	const wstring& wstrModelDataFile = wstrDataPath + _wstrDataFileName + L"_Prefab" + L".txt";

	// 프리팹 데이터 파일을 읽고 저장합니다.
	ifstream istrDataFile(wstrModelDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open PrefabDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())
	{
		// 1. 새로운 모델 데이터를 만들고, 거기에 데이터를 기록한다.
		MODEL_DATA Data = {};

		// 예외 처리
		if (istrDataFile.eof())
			break;

		// 2. [파일 경로]와 [프리팹]은 고정된 크기를 가지기 때문에, 구조체 통째로 불러온다.
		istrDataFile.read((char*)(&Data.szModelFilePath), MAX_PATH);
		istrDataFile.read((char*)(&Data.szModelFileName), MAX_PATH);
		istrDataFile.read((char*)(&Data.eModelType), sizeof(MODEL_TYPE));

		// 3. [AI_Scene]은 가변적인 데이터가 포함되어 있기 때문에, 모델마다 가변적으로 불러온다.
		MY_AISCENE& AIScene = Data.tAIScenes;

		// == 3.1 애니메이션 ==========================================================================
		istrDataFile.read((char*)(&AIScene.iNumAnimations), sizeof(unsigned int));

		// 애니메이션 벡터에 데이터를 기록하기 위해 resize
		AIScene.vecAnimations.resize(AIScene.iNumAnimations);
		for (_uint i = 0; i < AIScene.iNumAnimations; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].szName), MAX_PATH);

			istrDataFile.read((char*)(&AIScene.vecAnimations[i].Duration), sizeof(double));
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].TickPerSecond), sizeof(double));
			istrDataFile.read((char*)(&AIScene.vecAnimations[i].iNumChannels), sizeof(unsigned int));

			AIScene.vecAnimations[i].vecChannels.resize(AIScene.vecAnimations[i].iNumChannels);
			for (_uint j = 0; j < AIScene.vecAnimations[i].iNumChannels; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].szName), MAX_PATH);

				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys), sizeof(unsigned int));
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys), sizeof(unsigned int));
				istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys), sizeof(unsigned int));

				AIScene.vecAnimations[i].vecChannels[j].vecScalingKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumScalingKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecScalingKeys[k]), sizeof(MY_VECTORKEY));
				}

				AIScene.vecAnimations[i].vecChannels[j].vecRotationKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumRotationKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecRotationKeys[k]), sizeof(MY_QUATKEY));
				}

				AIScene.vecAnimations[i].vecChannels[j].vecPositionKeys.resize(AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys);
				for (_uint k = 0; k < AIScene.vecAnimations[i].vecChannels[j].iNumPositionKeys; ++k)
				{
					istrDataFile.read((char*)(&AIScene.vecAnimations[i].vecChannels[j].vecPositionKeys[k]), sizeof(MY_VECTORKEY));
				}
			}
		}

		// == 3.2 본 ==========================================================================
		MY_AINODE& RootNode = AIScene.tRootBone;

		if (MODEL_ANIM == Data.eModelType)
		{
			// 이름
			istrDataFile.read(RootNode.szBoneName, MAX_PATH);

			// TransformationMatrix
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));

			// iNumChildren(자식 본 개수)
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));

			// vecChildren 재귀적으로 저장
			CPrefabManager::GetInstance()->ReadBones(RootNode, istrDataFile);
		}
		else
		{
			// 논애님 모델의 경우 본 데이터가 없으므로 빈 값으로 초기화하여 기록한다.
			char szName[MAX_PATH] = "";
			XMFLOAT4X4 matTransformation = {};
			unsigned int iNumChildren = 0;

			istrDataFile.read(RootNode.szBoneName, MAX_PATH);
			istrDataFile.read((char*)(&RootNode.matTransformation), sizeof(XMFLOAT4X4));
			istrDataFile.read((char*)(&RootNode.iNumChildren), sizeof(unsigned int));
		}

		// == 3.3 메시 ==========================================================================

		// 메시 개수
		istrDataFile.read((char*)(&AIScene.iNumMeshes), sizeof(unsigned int));

		// 메시 벡터에 데이터를 기록하기 위해 resize
		AIScene.vecMeshs.resize(AIScene.iNumMeshes);

		// 메시
		for (_uint i = 0; i < AIScene.iNumMeshes; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].szName), MAX_PATH);

			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iMaterialIndex), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumVertices), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumFaces), sizeof(_uint));
			istrDataFile.read((char*)(&AIScene.vecMeshs[i].iNumBones), sizeof(_uint));

			_uint iMesh_MaterialIndex = AIScene.vecMeshs[i].iMaterialIndex;	// 메시가 사용하는 재질의 인덱스
			_uint iMesh_NumVertices = AIScene.vecMeshs[i].iNumVertices;		// 정점 개수
			_uint iMesh_NumFaces = AIScene.vecMeshs[i].iNumFaces;			// 삼각형 개수
			_uint iMesh_NumBones = AIScene.vecMeshs[i].iNumBones;			// 메시에 영향을 주는 본의 개수

			// i번 째 메시의 j번 째 Vertex 기록
			AIScene.vecMeshs[i].vecVertices.resize(iMesh_NumVertices);
			for (_uint j = 0; j < iMesh_NumVertices; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecVertices[j]), sizeof(VTXANIMMESH));
			}

			// i번 째 메시의 인덱스 기록
			AIScene.vecMeshs[i].vecFaces.resize(iMesh_NumFaces);
			for (_uint j = 0; j < iMesh_NumFaces; ++j)
			{
				istrDataFile.read((char*)(&AIScene.vecMeshs[i].vecFaces[j]), sizeof(MY_AIFACE));
			}

			// i번째 메시에 영향을 주는 j번 째 Bone 기록
			AIScene.vecMeshs[i].vecAIBone.resize(iMesh_NumBones);
			for (_uint j = 0; j < iMesh_NumBones; ++j)
			{
				// j번 째 Bone에 참조로 접근해서 기록
				MY_AIBONE& AIBone = AIScene.vecMeshs[i].vecAIBone[j];

				istrDataFile.read((char*)(&AIBone.szBoneName), MAX_PATH);

				istrDataFile.read((char*)(&AIBone.matOffset), sizeof(XMFLOAT4X4));
				istrDataFile.read((char*)(&AIBone.iNumWeights), sizeof(unsigned int));

				AIBone.vecWeights.resize(AIBone.iNumWeights);
				for (_uint Index = 0; Index < AIBone.iNumWeights; ++Index)
				{
					istrDataFile.read((char*)(&AIBone.vecWeights[Index]), sizeof(MY_WEIGHT));
				}
			}
		}


		// == 3.4 재질 ==========================================================================

		// 재질 개수
		istrDataFile.read((char*)(&AIScene.iNumMaterials), sizeof(unsigned int));

		// 재질
		_uint iNumMaterials = AIScene.iNumMaterials;
		AIScene.vecMaterials.resize(iNumMaterials);
		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			istrDataFile.read((char*)(&AIScene.vecMaterials[i]), sizeof(MY_AIMATERIAL));
		}

		wstring wstrModelFileName = L"";
		wstring wstrActorName = L"";

		for (_uint i = 0; i < 64; ++i)
		{
			if (Data.szModelFileName[i] == '\0') { break; }
			wstrModelFileName += Data.szModelFileName[i];
		}

		// 프리팹 매니저에 모델 정보 추가
		CPrefabManager::GetInstance()->AddModelData(wstrModelFileName, Data);

	}
	istrDataFile.close();
}
void CInspector::LoadSpawnData(const wstring& _wstrDataFileName)
{
	const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();

	const wstring& wstrSpawnDataFile = wstrDataPath + _wstrDataFileName + L"_Spawn" + L".txt";

	// 프리팹 데이터 파일을 읽고 저장합니다.
	ifstream istrDataFile(wstrSpawnDataFile, ios::in | ios::binary);

	if (!istrDataFile.is_open())
	{
		MSG_BOX("Failed to open SpawnDataFile : CLevel");
		return;
	}

	while (!istrDataFile.eof())
	{
		// 예외 처리
		if (istrDataFile.eof())
			break;

		SPAWN_DATA Spawn = {};

		istrDataFile.read((char*)(&Spawn), sizeof(SPAWN_DATA));

		CSpawnManager::GetInstance()->AddSpawnData(Spawn.tPrefab.szActorName, Spawn);

		wstring wstrFileName = L"";
		wstring wstrActorName = L"";

		for (_uint i = 0; i < MAX_PATH; ++i)
		{
			if (Spawn.tPrefab.szModelFileName[i] == '\0') { break; }
			wstrFileName += Spawn.tPrefab.szModelFileName[i];
		}
		for (_uint i = 0; i < MAX_PATH; ++i)
		{
			if (Spawn.tPrefab.szActorName[i] == '\0') { break; }
			wstrActorName += Spawn.tPrefab.szActorName[i];
		}

		MODEL_DATA Data = *CPrefabManager::GetInstance()->GetModelData(wstrFileName);

		switch (Spawn.tPrefab.iLayerIndex)
		{
			// 0. 플레이어는 클라이언트에서 직접 생성한다.
		case 0:
		{
			break;
		}
		// 1. 몬스터
		case 1:
		{
			// 몬스터가 논애님일 일이 있을까
			if (MODEL_NONANIM == Data.eModelType)
			{
				m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->CreateActor<CNonAnimActor>(wstrActorName);
				dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetModelData(Data);
				dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetSpawnData(Spawn);
				dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->Initialize();
			}
			else
			{
				m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->CreateActor<CAnimActor>(wstrActorName);
				dynamic_pointer_cast<CAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetModelData(Data);
				dynamic_pointer_cast<CAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetSpawnData(Spawn);
				dynamic_pointer_cast<CAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->Initialize();
			}
			break;
		}
		case 2:
		case 3:
		{
			m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->CreateActor<CNonAnimActor>(wstrActorName);
			dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetModelData(Data);
			dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->SetSpawnData(Spawn);
			dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->Initialize();
			CCollisionManager::GetInstance()->AddNonAnimModel(dynamic_pointer_cast<CNonAnimActor>(m_wpEditorLevel.lock()->GetLayer(Spawn.tPrefab.iLayerIndex)->FindActor(wstrActorName))->GetNonAnimModel());
		}
		}
	}
}

/* Navigation */
void CInspector::UpdateNavigationTab()
{
	Picking_Navi();
	UpdatePickedPosition();
	UpdateNavigationSaveButton();
	UpdateRemoveLatestCellButton();
}
void CInspector::Picking_Navi()
{
	if (CInputManager::GetInstance()->IsMouseDown(EMouseButtonType::RIGHT_BUTTON))
	{
		shared_ptr<CActor> spCam = CObjectManager::GetInstance()->FindActor(m_wpEditorLevel.lock()->GetLevelIndex(), LAYER::LAYER_PLAYER, L"Actor_Player");

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		_vector vPickedPos = CCollisionManager::GetInstance()->IntersectRayToModel(spCam->GetActorTranslation(), pt, Tool::g_iWidth, Tool::g_iHeight);

		// 제대로 피킹이 됬다면
		if (0.f != XMVectorGetX(vPickedPos) && 0.f != XMVectorGetY(vPickedPos) && 0.f != XMVectorGetZ(vPickedPos))
		{
			// 정점 보정해서 일치시키기
			if (m_iClickCount != 1)
			{
				if (FAILED(CNavigationManager::GetInstance()->CorrectingPoints(vPickedPos)))
					return;
			}

			// 알맞게 정점이 일치되었으면, 정점 정보를 기록한다.
			XMStoreFloat3(&m_vPickedPos[m_iClickCount++], vPickedPos);

			if (2 < m_iClickCount)
			{
				CNavigationManager::GetInstance()->GetCurNavigation()->AddCell(m_vPickedPos);

				m_iClickCount = 0;
				for (_uint i = 0; i < 3; ++i)
					m_vPickedPos[i] = _float3{ 0.0f, 0.0f, 0.0f };
			}
		}
	}
}
void CInspector::UpdatePickedPosition()
{
	ImGui::SeparatorText("Picked Position");
	ImGui::Spacing();

	ImGui::InputFloat3("Index 0", &m_vPickedPos[0].x);
	ImGui::InputFloat3("Index 1", &m_vPickedPos[1].x);
	ImGui::InputFloat3("Index 2", &m_vPickedPos[2].x);
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}
void CInspector::UpdateNavigationSaveButton()
{
	ImGui::InputTextWithHint("File Name", "Navigation File Name", m_chNaviFileName, IM_ARRAYSIZE(m_chNaviFileName));
	if (ImGui::Button("Navigation Save", ImVec2(150, 30)))
	{
		// 1. 저장하기 버튼을 누르면, 입력된 텍스트에서 파일 이름을 추출한다.
		wstring wstrNaviFileName = L"";
		for (_uint i = 0; i < IM_ARRAYSIZE(m_chNaviFileName); ++i)
		{
			if (m_chNaviFileName[i] == '\0') { break; }
			else { wstrNaviFileName += m_chNaviFileName[i]; }
		}

		// 2. 리소스 매니저에서 데이터 파일이 들어있는 경로를 가져온다.
		const wstring& wstrDataPath = CResourceManager::GetInstance()->GetDataPath();
		wstring wstrNaviFilePath = wstrDataPath + L"Navigation\\";

		// 3. 네비 매니저에서 현재 네비게이션의 정점들을 가져온다.
		const vector<shared_ptr<CCell>>& vecCells = CNavigationManager::GetInstance()->GetCurNavigation()->GetVecCells();

		// 4. [ 파일경로 + 파일이름 + _Prafab.dat ]으로 파일을 연다.(없다면 생성한다.)
		ofstream ostrWriteFile(wstrNaviFilePath + wstrNaviFileName + L"_Navigation" + L".txt", ios::out | ios::binary);

		// 5. 파일이 열려 있다면, 네비게이션 데이터를 파일에 기록한다.
		if (ostrWriteFile.is_open())
		{
			for (const auto& Cell : vecCells)
			{
				_float3 vPoints[CCell::POINT_END] = {};

				vPoints[0] = Cell->GetPoint(CCell::POINT_A);
				vPoints[1] = Cell->GetPoint(CCell::POINT_B);
				vPoints[2] = Cell->GetPoint(CCell::POINT_C);

				ostrWriteFile.write((char*)(&vPoints), sizeof(_float3) * CCell::POINT_END);
			}

			// 6. 열렸던 파일을 닫아준다.
			ostrWriteFile.close();
		}
	}
}
void CInspector::UpdateRemoveLatestCellButton()
{
	if (ImGui::Button("RemoveLatestCell", ImVec2(150, 30)))
	{
		CNavigationManager::GetInstance()->RemoveLatestCell();
	}
}

/* Particle */
void CInspector::UpdateEffectTab()
{
	ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Effect Tab", tabBarFlags))
	{
		if (ImGui::BeginTabItem("Particle"))
		{
			UpdateParticleTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Mesh"))
		{
			UpdateMeshTab();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}
void CInspector::UpdateParticleTab()
{
	UpdateParticleLoadFromJSON();
	UpdateParticleOption();
	UpdateParticleShaderFileTable();
	UpdateColorRelative();
	UpdateParticleSRT();
	UpdateTimeRelative();
	UpdateSpeedRelative();
	UpdateSpriteRelative();
	UpdateParticleCreateButton();
	UpdateSpriteCreateButton();
	UpdateParticleSaveButton();
}
void CInspector::UpdateParticleLoadFromJSON()
{
	if (ImGui::CollapsingHeader("Load"))
	{
		ImGui::SeparatorText("Load");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::InputTextWithHint("Json File", "Json FileName", m_chParticleJSONFileName, 64);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		if (ImGui::Button("Load From Json", ImVec2(120, 30)))
		{
			wstring wstrJsonFileName = L"";

			for (_uint i = 0; i < 64; ++i)
			{
				if (m_chParticleJSONFileName[i] == '\0') { break; }
				wstrJsonFileName += m_chParticleJSONFileName[i];
			}

			shared_ptr<CParticleActor> spParticleActor = m_wpEditorLevel.lock()->GetLayer(Client::LAYER_PARTICLE)->CreateActorToPool<CParticleActor>(wstrJsonFileName);
			if (nullptr == spParticleActor)
				return;

			spParticleActor->SetJsonFileName(wstrJsonFileName);

			if (FAILED(spParticleActor->Initialize(wstrJsonFileName)))
			{
				spParticleActor->Release();
				spParticleActor.reset();

				m_wpEditorLevel.lock()->GetLayer(Client::LAYER_PARTICLE)->RemoveActor_Pool(wstrJsonFileName);
			}

			spParticleActor->SetActorUse(true);
		}
	}
}
void CInspector::UpdateParticleOption()
{
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	ImGui::SeparatorText("Particle Option");
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	if (0 >= m_iNumInstance)
	{
		m_iNumInstance = 1;
		ImGui::InputInt("Num Instance", &m_iNumInstance);
	}
	else
		ImGui::InputInt("Num Instance", &m_iNumInstance);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendY | ImGuiTableFlags_ScrollY;
	_int iMaxColumn = 4;
	if (ImGui::BeginTable("Table Diffuse", iMaxColumn, tableFlags, ImVec2(350, 250)))
	{
		_bool bIndexFinished = false;

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecTextures_Tool = CResourceManager::GetInstance()->GetSRVs_Static();

		_int iNumRow = (_int)vecTextures_Tool.size() / 4;

		for (int row = 0; row < iNumRow + 1; ++row)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < iMaxColumn; column++)
			{
				if (vecTextures_Tool.size() - 1 < iMaxColumn * row + column)
				{
					bIndexFinished = true;
					break;
				}

				ImGui::TableSetColumnIndex(column);
				ImGui::Text(to_string(iMaxColumn * row + column).c_str()); ImGui::SameLine();
				ImGui::Image(vecTextures_Tool[iMaxColumn * row + column].second, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1));
			}

			if (bIndexFinished)
				break;
		}
		ImGui::EndTable();
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::InputInt("Texture Index", &m_iParticleImageSelector);
		if (_int(vecTextures_Tool.size() - 1) < m_iParticleImageSelector)
			m_iParticleImageSelector = _int(vecTextures_Tool.size() - 1);
		if (m_iParticleImageSelector < 0)
			m_iParticleImageSelector = 0;
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	array<const char*, 6> arrRenderGroup =
	{
	"RENDER_PRIORITY",
	"RENDER_NONBLEND",
	"RENDER_NONLIGHT",
	"RENDER_BLEND",
	"RENDER_ORTHOGONAL",
	"RENDER_UI",
	};

	ImGui::Combo("Particle RenderGroup", &m_iParticleRenderGroupIndex, arrRenderGroup.data(), (_uint)arrRenderGroup.size());

	switch (m_iParticleRenderGroupIndex)
	{
	case 0: m_eParticleRenderGroup = RENDER_GROUP::RENDER_PRIORITY; break;
	case 1: m_eParticleRenderGroup = RENDER_GROUP::RENDER_NONBLEND; break;
	case 2: m_eParticleRenderGroup = RENDER_GROUP::RENDER_NONLIGHT; break;
	case 3: m_eParticleRenderGroup = RENDER_GROUP::RENDER_BLEND; break;
	case 4: m_eParticleRenderGroup = RENDER_GROUP::RENDER_ORTHOGONAL; break;
	case 5: m_eParticleRenderGroup = RENDER_GROUP::RENDER_UI; break;
	default: break;
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Checkbox("Loop", &m_bParticleLoop);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::Checkbox("BillBoard", &m_bParticleBillBoard);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	ImGui::RadioButton("Drop", &m_iParticleTypeRadio, 0); ImGui::SameLine();
	ImGui::RadioButton("Spread", &m_iParticleTypeRadio, 1); ImGui::SameLine();
	ImGui::RadioButton("Spread_Radius", &m_iParticleTypeRadio, 2); ImGui::SameLine();
	ImGui::RadioButton("Sprite", &m_iParticleTypeRadio, 3);

	m_eParticleType = (PARTICLE_TYPE)m_iParticleTypeRadio;
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateParticleShaderFileTable()
{
	ImGui::SeparatorText("Shader File");

	if (ImGui::BeginListBox("Shader Files", ImVec2(250, 100)))
	{
		const vector<string>& vecShaderFilePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_SHADER);

		for (_uint i = 0; i < vecShaderFilePaths.size(); ++i)
		{
			string strFilePath = vecShaderFilePaths[i];

			_char szFileName[MAX_PATH] = "";

			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool isRectSelected = (m_iParticleShaderFileSelector == vecShaderFilePaths.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, isRectSelected))
			{
				m_iParticleShaderFileSelector = _int(i);
			}
		}

		ImGui::EndListBox();
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateColorRelative()
{
	if (ImGui::CollapsingHeader("Color"))
	{
		ImGui::SeparatorText("Color Relative");

		ImGui::ColorPicker4("Particle Color", &m_vParticleColor.x);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}
void CInspector::UpdateParticleSRT()
{
	if (ImGui::CollapsingHeader("Local Matrix"))
	{
		ImGui::SeparatorText("Particle Transform");

		ImGui::InputFloat3("Rotate", &m_vParticleRotation.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputFloat2("Size [Min / Max]", &m_vParticleSizeMinMax.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputFloat3("Range", &m_vParticleRange.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputFloat2("Radius", &m_vParticleRadius.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputFloat3("Offset", &m_vParticleOffset.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::InputFloat3("Pivot Offset", &m_vParticlePivot.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}
void CInspector::UpdateTimeRelative()
{
	if (ImGui::CollapsingHeader("Time"))
	{
		ImGui::SeparatorText("Time Relative");

		ImGui::InputFloat("Duration", &m_fParticleDuration);
		ImGui::InputFloat2("LifeTime", &m_vParticleLifeTime.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}
void CInspector::UpdateSpeedRelative()
{
	if (ImGui::CollapsingHeader("Speed"))
	{
		ImGui::SeparatorText("Speed Relative");

		ImGui::InputFloat2("Speed [Min / Max]", &m_vParticleSpeed.x);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}
void CInspector::UpdateSpriteRelative()
{
	if (ImGui::CollapsingHeader("Sprite"))
	{
		ImGui::SeparatorText("Sprite Relative");

		ImGui::InputFloat2("Row / Col", &m_vSpriteRowCol.x);
		ImGui::InputFloat("Sprite Speed", &m_fSpriteSpeed);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
}
void CInspector::UpdateParticleCreateButton()
{
	if (ImGui::Button("Particle Create", ImVec2(150, 30)))
	{
#pragma region 중복 검사 및 파일이름 추출

		if ('\0' == m_chParticleFileName[0])
		{
			MSG_BOX("파일 이름을 입력하세요.");
			return;
		}

		wstring wstrActorName = L"";
		string strActorName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (m_chParticleFileName[i] == '\0') { break; }
			wstrActorName += m_chParticleFileName[i];
			strActorName += m_chParticleFileName[i];
		}

		if (!CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER::LAYER_PARTICLE)->GetActors_Pool().empty())
		{
			// 파티클은 툴에서 1개만 띄웁니다.
			MSG_BOX("파티클은 한개로 충분합니다.");
			return;
		}

		// [ 텍스처 ]
		_char szDiffuseTextureFileName[MAX_PATH] = "";
		_splitpath_s(m_vecDiffuseTexturePaths[m_iParticleImageSelector], nullptr, 0, nullptr, 0, szDiffuseTextureFileName, MAX_PATH, nullptr, 0);

		// [ 셰이더 ]
		_char szShaderFileName[MAX_PATH] = "";
		_splitpath_s(m_vecShaderPaths[m_iParticleShaderFileSelector], nullptr, 0, nullptr, 0, szShaderFileName, MAX_PATH, nullptr, 0);

#pragma endregion 중복 검사 및 파일이름 추출

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecTextures_Tool = CResourceManager::GetInstance()->GetSRVs_Static();

		if (0 >= m_iNumInstance)
		{
			MSG_BOX("파티클은 0개일 수 없습니다.");
			return;
		}

		// 1. 객체를 생성하고, 기본 데이터들을 채워준다.
		shared_ptr<CParticleActor> spParticleActor = m_wpEditorLevel.lock()->GetLayer(Client::LAYER_PARTICLE)->CreateActorToPool<CParticleActor>(wstrActorName);
		if (nullptr == spParticleActor)
			return;

#pragma region 파일 이름 및 경로

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();

		// 디퓨즈 텍스처
		wstring wstrDiffuseTextureName;
		wstrDiffuseTextureName = vecDiffuseTextures[m_iParticleImageSelector].first;

		string	strDiffuseTextureName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName[i] == '\0') { break; }
			strDiffuseTextureName += wstrDiffuseTextureName[i];
		}

#pragma endregion 파일 이름 및 경로

		// 2. 인스턴스 데이터 구조체를 채운다.
		INSTANCE_DESC InstanceDesc = {};

		InstanceDesc.eParticleType = m_eParticleType;
		strcpy_s(InstanceDesc.szShaderFileName, szShaderFileName);
		strcpy_s(InstanceDesc.szDiffuseTextureFileName, szDiffuseTextureFileName);
		InstanceDesc.eRenderGroup = m_eParticleRenderGroup;
		InstanceDesc.isLoop = m_bParticleLoop;

		InstanceDesc.iNumInstance = m_iNumInstance;
		InstanceDesc.vPivot = m_vParticlePivot;
		// InstanceDesc.vCenter = 액터의 위치로 잡아주자
		InstanceDesc.vRange = m_vParticleRange;
		InstanceDesc.vRadius = m_vParticleRadius;
		InstanceDesc.vOffset = m_vParticleOffset;
		InstanceDesc.vRotate = m_vParticleRotation;

		InstanceDesc.vSize = m_vParticleSizeMinMax;
		InstanceDesc.vSpeed = m_vParticleSpeed;
		InstanceDesc.vColor = m_vParticleColor;
		InstanceDesc.vLifeTime = m_vParticleLifeTime;
		InstanceDesc.fDuration = m_fParticleDuration;

		// 3. 구조체를 액터에 전달한다.
		spParticleActor->SetInstanceDesc(InstanceDesc);
		spParticleActor->SetActorUse(true);

		// 4. Initialize를 호출한다.
		spParticleActor->Initialize();
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateSpriteCreateButton()
{
	if (ImGui::Button("Sprite Create", ImVec2(150, 30)))
	{
#pragma region 중복 검사 및 파일이름 추출

		if ('\0' == m_chParticleFileName[0])
		{
			MSG_BOX("파일 이름을 입력하세요.");
			return;
		}

		wstring wstrActorName = L"";
		string strActorName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (m_chParticleFileName[i] == '\0') { break; }
			wstrActorName += m_chParticleFileName[i];
			strActorName += m_chParticleFileName[i];
		}

		if (!CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER::LAYER_PARTICLE)->GetActors_Pool().empty())
		{
			// 파티클은 툴에서 1개만 띄웁니다.
			MSG_BOX("파티클은 한개로 충분합니다.");
			return;
		}

		// [ 텍스처 ]
		_char szDiffuseTextureFileName[MAX_PATH] = "";
		_splitpath_s(m_vecDiffuseTexturePaths[m_iParticleImageSelector], nullptr, 0, nullptr, 0, szDiffuseTextureFileName, MAX_PATH, nullptr, 0);

		// [ 셰이더 ]
		_char szShaderFileName[MAX_PATH] = "";
		_splitpath_s(m_vecShaderPaths[m_iParticleShaderFileSelector], nullptr, 0, nullptr, 0, szShaderFileName, MAX_PATH, nullptr, 0);

#pragma endregion 중복 검사 및 파일이름 추출

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecTextures_Tool = CResourceManager::GetInstance()->GetSRVs_Static();

		if (0 >= m_iNumInstance)
		{
			MSG_BOX("스프라이트는 0개일 수 없습니다.");
			return;
		}

		// 1. 객체를 생성하고, 기본 데이터들을 채워준다.
		shared_ptr<CSpriteActor> spSpriteActor = m_wpEditorLevel.lock()->GetLayer(Client::LAYER_PARTICLE)->CreateActorToPool<CSpriteActor>(wstrActorName);
		if (nullptr == spSpriteActor)
			return;

#pragma region 파일 이름 및 경로

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();

		// 디퓨즈 텍스처
		wstring wstrDiffuseTextureName;
		wstrDiffuseTextureName = vecDiffuseTextures[m_iParticleImageSelector].first;

		string	strDiffuseTextureName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName[i] == '\0') { break; }
			strDiffuseTextureName += wstrDiffuseTextureName[i];
		}

#pragma endregion 파일 이름 및 경로

		// 2. 인스턴스 데이터 구조체를 채운다.
		INSTANCE_DESC InstanceDesc = {};

		InstanceDesc.eParticleType = m_eParticleType;
		strcpy_s(InstanceDesc.szShaderFileName, szShaderFileName);
		strcpy_s(InstanceDesc.szDiffuseTextureFileName, szDiffuseTextureFileName);
		InstanceDesc.eRenderGroup = m_eParticleRenderGroup;
		InstanceDesc.isLoop = m_bParticleLoop;

		InstanceDesc.iNumInstance = m_iNumInstance;
		InstanceDesc.vPivot = m_vParticlePivot;
		// InstanceDesc.vCenter = 액터의 위치로 잡아주자
		InstanceDesc.vRange = m_vParticleRange;
		InstanceDesc.vRadius = m_vParticleRadius;
		InstanceDesc.vOffset = m_vParticleOffset;
		InstanceDesc.vRotate = m_vParticleRotation;

		InstanceDesc.vSize = m_vParticleSizeMinMax;
		InstanceDesc.vSpeed = m_vParticleSpeed;
		InstanceDesc.vColor = m_vParticleColor;
		InstanceDesc.vLifeTime = m_vParticleLifeTime;
		InstanceDesc.fDuration = m_fParticleDuration;

		// 3. 구조체를 액터에 전달한다.
		spSpriteActor->SetInstanceDesc(InstanceDesc);
		spSpriteActor->SetActorUse(true);

		spSpriteActor->SetRow((_uint)m_vSpriteRowCol.x);
		spSpriteActor->SetCol((_uint)m_vSpriteRowCol.y);
		spSpriteActor->SetSpeedRatio(m_fSpriteSpeed);

		// 4. Initialize를 호출한다.
		spSpriteActor->Initialize();
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateParticleSaveButton()
{
	ImGui::SeparatorText("Save");

	ImGui::InputTextWithHint("FileName", "Particle FileName", m_chParticleFileName, 64);

	if (ImGui::Button("Particle Save", ImVec2(150, 30)))
	{
		ofstream json_file;

		wstring wstrFileName = L"";
		string strFileName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (m_chParticleFileName[i] == '\0') { break; }

			wstrFileName += m_chParticleFileName[i];
			strFileName += m_chParticleFileName[i];
		}

		// [ 텍스처 ]
		_char szDiffuseTextureFileName[MAX_PATH] = "";
		_splitpath_s(m_vecDiffuseTexturePaths[m_iParticleImageSelector], nullptr, 0, nullptr, 0, szDiffuseTextureFileName, MAX_PATH, nullptr, 0);

		// [ 셰이더 ]
		_char szShaderFileName[MAX_PATH] = "";
		_splitpath_s(m_vecShaderPaths[m_iParticleShaderFileSelector], nullptr, 0, nullptr, 0, szShaderFileName, MAX_PATH, nullptr, 0);

		json_file.open(L"../../Resources/Data/Effect/Particle/" + wstrFileName + L".json");

		Json::Value Particle;

		Particle["01. FileName"] = strFileName;
		Particle["02. Texture FileName"] = szDiffuseTextureFileName;
		Particle["03. Shader FileName"] = szShaderFileName;

		Particle["04. Particle Type"] = m_eParticleType;
		Particle["05. Render Group"] = m_eParticleRenderGroup;
		Particle["06. Loop"] = m_bParticleLoop;
		Particle["07. BillBoard"] = m_bParticleBillBoard;

		Particle["08. NumInstance"] = m_iNumInstance;

		Particle["09. Pivot"].append(m_vParticlePivot.x);
		Particle["09. Pivot"].append(m_vParticlePivot.y);
		Particle["09. Pivot"].append(m_vParticlePivot.z);

		// 센터는 액터 위치로
		Particle["10. Center"].append(0.f);
		Particle["10. Center"].append(0.f);
		Particle["10. Center"].append(0.f);

		Particle["11. Range"].append(m_vParticleRange.x);
		Particle["11. Range"].append(m_vParticleRange.y);
		Particle["11. Range"].append(m_vParticleRange.z);

		Particle["12. Offset"].append(m_vParticleOffset.x);
		Particle["12. Offset"].append(m_vParticleOffset.y);
		Particle["12. Offset"].append(m_vParticleOffset.z);

		Particle["13. Size"].append(m_vParticleSizeMinMax.x);
		Particle["13. Size"].append(m_vParticleSizeMinMax.y);

		Particle["14. Speed"].append(m_vParticleSpeed.x);
		Particle["14. Speed"].append(m_vParticleSpeed.y);

		Particle["15. Color"].append(m_vParticleColor.x);
		Particle["15. Color"].append(m_vParticleColor.y);
		Particle["15. Color"].append(m_vParticleColor.z);
		Particle["15. Color"].append(m_vParticleColor.w);

		Particle["16. LifeTime"].append(m_vParticleLifeTime.x);
		Particle["16. LifeTime"].append(m_vParticleLifeTime.y);
		Particle["17. Duration"] = m_fParticleDuration;

		Particle["18. Radius"].append(m_vParticleRadius.x);
		Particle["18. Radius"].append(m_vParticleRadius.y);

		Particle["19. Rotate"].append(m_vParticleRotation.x);
		Particle["19. Rotate"].append(m_vParticleRotation.y);
		Particle["19. Rotate"].append(m_vParticleRotation.z);

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "	";	// Tab
		unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

		writer->write(Particle, &cout);
		writer->write(Particle, &json_file);

		json_file.close();
	}
}

/* Effect Mesh */
void CInspector::UpdateMeshTab()
{
	UpdateEffectMeshLoadFromJSON();
	UpdateModelTypeRadio();
	UpdateEffectMeshRenderGroupCombo();
	UpdateEffectMeshTable();
	UpdateEffectMeshShaderFileTable();
	UpdateDiffuseColorTable();
	UpdateDiffuseTextureTable();
	UpdateNoiseTextureTable();
	UpdateMaskTextureTable();
	UpdateEffectMeshUVMove();
	UpdateEffectMeshSRT();
	UpdateEffectMeshTransformChange();
	UpdateEffectMeshLifeTime();
	UpdateEffectMeshCreateButton();
	UpdateEffectMeshSaveButton();
}
void CInspector::UpdateEffectMeshLoadFromJSON()
{
	if(ImGui::CollapsingHeader("Load"))
	{
		ImGui::SeparatorText("Load");
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		ImGui::InputTextWithHint("Json File", "Json FileName", m_chEffectMeshJSONFileName, 64);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		if (ImGui::Button("Load From Json", ImVec2(120, 30)))
		{
			wstring wstrJsonFileName = L"";

			for (_uint i = 0; i < 64; ++i)
			{
				if (m_chEffectMeshJSONFileName[i] == '\0') { break; }
				wstrJsonFileName += m_chEffectMeshJSONFileName[i];
			}

			shared_ptr<CEffectMeshActor> spEffectMeshActor = m_wpEditorLevel.lock()->GetLayer(Client::LAYER_EFFECTMESH)->CreateActorToPool<CEffectMeshActor>(wstrJsonFileName);
			if (nullptr == spEffectMeshActor)
				return;

			spEffectMeshActor->SetJsonFileName(wstrJsonFileName);

			if (FAILED(spEffectMeshActor->Initialize(wstrJsonFileName)))
			{
				spEffectMeshActor->Release();
				spEffectMeshActor.reset();

				m_wpEditorLevel.lock()->GetLayer(Client::LAYER_EFFECTMESH)->RemoveActor(wstrJsonFileName);
			}

			EFFECTMESH_DESC LoadDesc = spEffectMeshActor->GetEffectMeshDesc();

			m_iEffectMeshRenderGroup = LoadDesc.eRenderGruop;
			m_iEffectMeshModelType = LoadDesc.eModelType;

			m_vEffectMeshScale = LoadDesc.vScale;
			m_vEffectMeshRotation = LoadDesc.vRotation;
			m_vEffectMeshOffset = LoadDesc.vTranslate;

			m_vEffectMeshDiffuseColor1 = LoadDesc.vDiffuseBaseColor;
			m_vEffectMeshDiffuseColor2 = LoadDesc.vDiffuseLerpColor;

			m_vEffectMeshLifeTime = LoadDesc.vLifeTime;

			m_bEffectMeshUVMove = LoadDesc.bUVMoveMode;
			m_bEffectMeshUVReverse = LoadDesc.bUVMoveReverse;
			m_bEffectMeshUVMoveLoop = LoadDesc.bUVMoveLoop;
			m_vEffectMeshUVSpeed = LoadDesc.vUVMoveSpeed;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateModelTypeRadio()
{
	ImGui::SeparatorText("Effect Mesh Option");
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	ImGui::RadioButton("NonAnim", &m_iEffectMeshModelType, 0); ImGui::SameLine();
	ImGui::RadioButton("Anim", &m_iEffectMeshModelType, 1);

	switch (m_iEffectMeshModelType)
	{
	case 0:
	{
		m_eEffectMeshModelType = MODEL_TYPE::MODEL_NONANIM;
		break;
	}
	case 1:
	{
		m_eEffectMeshModelType = MODEL_TYPE::MODEL_ANIM;
		break;
	}
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateEffectMeshRenderGroupCombo()
{
	array<const char*, 6> arrRenderGroup =
	{
	"RENDER_PRIORITY",
	"RENDER_NONBLEND",
	"RENDER_NONLIGHT",
	"RENDER_BLEND",
	"RENDER_ORTHOGONAL",
	"RENDER_UI",
	};

	ImGui::Combo("RenderGroup", &m_iEffectMeshRenderGroup, arrRenderGroup.data(), (_uint)arrRenderGroup.size());

	switch (m_iEffectMeshRenderGroup)
	{
	case 0: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_PRIORITY; break;
	case 1: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_NONBLEND; break;
	case 2: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_NONLIGHT; break;
	case 3: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_BLEND; break;
	case 4: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_ORTHOGONAL; break;
	case 5: m_eEffectMeshRenderGroup = RENDER_GROUP::RENDER_UI; break;
	default: break;
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}
void CInspector::UpdateEffectMeshTable()
{
	ImGui::SeparatorText("Effect Mesh");

	if (ImGui::BeginListBox("Effect Mesh", ImVec2(250, 100)))
	{
		const vector<string>& vecEffectMeshFilePaths = CResourceManager::GetInstance()->GetStrFilePaths(RESOURCE_EFFECTMESH);

		for (_uint i = 0; i < vecEffectMeshFilePaths.size(); ++i)
		{
			string strFilePath = vecEffectMeshFilePaths[i];

			_char szFileName[MAX_PATH] = "";

			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool isRectSelected = (m_iEffectMeshSelector == vecEffectMeshFilePaths.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, isRectSelected))
			{
				m_iEffectMeshSelector = _int(i);
			}
		}
		
		ImGui::EndListBox();
	}
}
void CInspector::UpdateEffectMeshShaderFileTable()
{
	ImGui::SeparatorText("Shader File");

	if (ImGui::BeginListBox("Shader File", ImVec2(250, 100)))
	{
		const vector<string>& vecShaderFilePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_SHADER);

		for (_uint i = 0; i < vecShaderFilePaths.size(); ++i)
		{
			string strFilePath = vecShaderFilePaths[i];

			_char szFileName[MAX_PATH] = "";

			_splitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);

			const bool isRectSelected = (m_iEffectMeshShaderFileSelector == vecShaderFilePaths.size() - 1);
			if (ImGui::Selectable(szFileName, ImGuiSelectableFlags_None, isRectSelected))
			{
				m_iEffectMeshShaderFileSelector = _int(i);
			}
		}

		ImGui::EndListBox();
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateDiffuseColorTable()
{
	ImGui::ColorPicker4("Diffuse Color1", &m_vEffectMeshDiffuseColor1.x);
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	ImGui::ColorPicker4("Diffuse Color2", &m_vEffectMeshDiffuseColor2.x);
	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY | ImGuiTableFlags_ScrollY;
	_int iMaxColumn = 5;

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	// 현재 이펙트 메시의 색상을 실시간으로 변경합니다.
	auto iter = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_EFFECTMESH)->GetActors_Pool().begin();

	if (iter == CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_EFFECTMESH)->GetActors_Pool().end())
		return;

	shared_ptr<CActor> spActor = (iter->second).front();
	if (nullptr == spActor)
		return;

	dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vDiffuseBaseColor = m_vEffectMeshDiffuseColor1;
	dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vDiffuseLerpColor = m_vEffectMeshDiffuseColor2;

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateDiffuseTextureTable()
{
	ImGui::Checkbox("Use Diffuse Texture", &m_bUseDiffuseTexture);

	if (m_bUseDiffuseTexture)
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY | ImGuiTableFlags_ScrollY;
		_int iMaxColumn = 5;
		if (ImGui::BeginTable("Table Diffuse1_EffectMesh", iMaxColumn, tableFlags, ImVec2(350, 100)))
		{
			_bool bIndexFinished = false;

			const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();

			_int iNumRow = (_int)vecDiffuseTextures.size() / 4;

			for (int row = 0; row < iNumRow + 1; ++row)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < iMaxColumn; column++)
				{
					if (vecDiffuseTextures.size() - 1 < iMaxColumn * row + column)
					{
						bIndexFinished = true;
						break;
					}

					ImGui::TableSetColumnIndex(column);
					ImGui::Text(to_string(iMaxColumn * row + column).c_str()); ImGui::SameLine();
					ImGui::Image(vecDiffuseTextures[iMaxColumn * row + column].second, ImVec2(35, 35), ImVec2(0, 0), ImVec2(1, 1));
				}

				if (bIndexFinished)
					break;
			}
			ImGui::EndTable();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::InputInt("Diffuse1 Index", &m_iDiffuseTextureSelector1);
			if (_int(vecDiffuseTextures.size() - 1) < m_iDiffuseTextureSelector1)
				m_iDiffuseTextureSelector1 = _int(vecDiffuseTextures.size() - 1);
			if (m_iDiffuseTextureSelector1 < 0)
				m_iDiffuseTextureSelector1 = 0;
		}

		if (ImGui::BeginTable("Table Diffuse2_EffectMesh", iMaxColumn, tableFlags, ImVec2(350, 100)))
		{
			_bool bIndexFinished = false;

			const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();

			_int iNumRow = (_int)vecDiffuseTextures.size() / 4;

			for (int row = 0; row < iNumRow + 1; ++row)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < iMaxColumn; column++)
				{
					if (vecDiffuseTextures.size() - 1 < iMaxColumn * row + column)
					{
						bIndexFinished = true;
						break;
					}

					ImGui::TableSetColumnIndex(column);
					ImGui::Text(to_string(iMaxColumn * row + column).c_str()); ImGui::SameLine();
					ImGui::Image(vecDiffuseTextures[iMaxColumn * row + column].second, ImVec2(45, 45), ImVec2(0, 0), ImVec2(1, 1));
				}

				if (bIndexFinished)
					break;
			}
			ImGui::EndTable();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::InputInt("Diffuse2 Index", &m_iDiffuseTextureSelector2);
			if (_int(vecDiffuseTextures.size() - 1) < m_iDiffuseTextureSelector2)
				m_iDiffuseTextureSelector2 = _int(vecDiffuseTextures.size() - 1);
			if (m_iDiffuseTextureSelector2 < 0)
				m_iDiffuseTextureSelector2 = 0;
		}
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateNoiseTextureTable()
{
	if (ImGui::CollapsingHeader("Noise"))
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY | ImGuiTableFlags_ScrollY;
		_int iMaxColumn = 5;
		if (ImGui::BeginTable("Table Noise_EffectMesh", iMaxColumn, tableFlags, ImVec2(350, 100)))
		{
			_bool bIndexFinished = false;

			const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecNoiseTextures = CResourceManager::GetInstance()->GetNoises_Static();

			_int iNumRow = (_int)vecNoiseTextures.size() / 4;

			for (int row = 0; row < iNumRow + 1; ++row)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < iMaxColumn; column++)
				{
					if (vecNoiseTextures.size() - 1 < iMaxColumn * row + column)
					{
						bIndexFinished = true;
						break;
					}

					ImGui::TableSetColumnIndex(column);
					ImGui::Text(to_string(iMaxColumn * row + column).c_str()); ImGui::SameLine();
					ImGui::Image(vecNoiseTextures[iMaxColumn * row + column].second, ImVec2(40, 40), ImVec2(0, 0), ImVec2(1, 1));
				}

				if (bIndexFinished)
					break;
			}
			ImGui::EndTable();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Checkbox("Use Noise Texture", &m_bUseNoiseTexture);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::InputInt("Noise Index", &m_iNoiseTextureSelector);
			if (_int(vecNoiseTextures.size() - 1) < m_iNoiseTextureSelector)
				m_iNoiseTextureSelector = _int(vecNoiseTextures.size() - 1);
			if (m_iNoiseTextureSelector < 0)
				m_iNoiseTextureSelector = 0;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateMaskTextureTable()
{
	if (ImGui::CollapsingHeader("Mask"))
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendY | ImGuiTableFlags_ScrollY;
		_int iMaxColumn = 5;
		if (ImGui::BeginTable("Table Mask_EffectMesh", iMaxColumn, tableFlags, ImVec2(350, 100)))
		{
			_bool bIndexFinished = false;

			const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecMaskTextures = CResourceManager::GetInstance()->GetMasks_Static();

			_int iNumRow = (_int)vecMaskTextures.size() / 4;

			for (int row = 0; row < iNumRow + 1; ++row)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < iMaxColumn; column++)
				{
					if (vecMaskTextures.size() - 1 < iMaxColumn * row + column)
					{
						bIndexFinished = true;
						break;
					}

					ImGui::TableSetColumnIndex(column);
					ImGui::Text(to_string(iMaxColumn * row + column).c_str()); ImGui::SameLine();
					ImGui::Image(vecMaskTextures[iMaxColumn * row + column].second, ImVec2(30, 30), ImVec2(0, 0), ImVec2(1, 1));
				}

				if (bIndexFinished)
					break;
			}
			ImGui::EndTable();
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::Checkbox("Use Mask Texture", &m_bUseMaskTexture);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::InputInt("Mask Index", &m_iMaskTextureSelector);
			if (_int(vecMaskTextures.size() - 1) < m_iMaskTextureSelector)
				m_iMaskTextureSelector = _int(vecMaskTextures.size() - 1);
			if (m_iMaskTextureSelector < 0)
				m_iMaskTextureSelector = 0;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateEffectMeshUVMove()
{
	if (ImGui::CollapsingHeader("UV Move"))
	{
		ImGui::Checkbox("UV Move Mode", &m_bEffectMeshUVMove);
		ImGui::SameLine();

		ImGui::Checkbox("UV Move Reverse", &m_bEffectMeshUVReverse);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::Checkbox("UV Move Loop", &m_bEffectMeshUVMoveLoop);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::InputFloat2("Mask UV Speed", &m_vEffectMeshUVSpeed.x);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateEffectMeshSRT()
{
	if (ImGui::CollapsingHeader("Local Matrix"))
	{
		ImGui::SeparatorText("Effect Mesh Transform");

		ImGui::InputFloat3("Scale", &m_vEffectMeshScale.x);
		ImGui::InputFloat3("Rotate", &m_vEffectMeshRotation.x);
		ImGui::InputFloat3("Translate", &m_vEffectMeshOffset.x);
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateEffectMeshTransformChange()
{
	if (ImGui::CollapsingHeader("Transform Change"))
	{
		ImGui::SeparatorText("Transform Change Mode");
		
		ImGui::Checkbox("Use Transform Change", &m_bEffectMeshTransformChange);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::InputFloat3("Dest Scale", &m_vEffectMeshDestScale.x);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::InputFloat("Scale Change Ratio", &m_fEffectMeshScaleChangeRatio);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		ImGui::InputFloat3("Rotate Per Second", &m_vEffectMeshRotatePerSecond.x);
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
	}
}
void CInspector::UpdateEffectMeshLifeTime()
{
	if (ImGui::CollapsingHeader("Time"))
	{
		ImGui::SeparatorText("Time Relative");

		ImGui::InputFloat2("LifeTime", &m_vEffectMeshLifeTime.x);
	}
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CInspector::UpdateEffectMeshCreateButton()
{
	if (ImGui::Button("Craete Effect Mesh", ImVec2(150, 30)))
	{
#pragma region 중복 검사 및 파일이름 추출

		if ('\0' == m_chEffectMeshFileName[0])
		{
			MSG_BOX("파일 이름을 입력하세요.");
			return;
		}

		wstring wstrActorName = L"";
		string strActorName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (m_chEffectMeshFileName[i] == '\0') { break; }
			wstrActorName += m_chEffectMeshFileName[i];
			strActorName += m_chEffectMeshFileName[i];
		}

		if (!CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER::LAYER_EFFECTMESH)->GetActors_Pool().empty())
		{
			// 이펙트 메시는 1개만 띄웁니다.
			MSG_BOX("이펙트 메시는 한개로 충분합니다.");
			return;
		}

		_char szModelFileName[MAX_PATH] = "";
		_splitpath_s(m_vecEffectMeshFilePaths[m_iEffectMeshSelector], nullptr, 0, nullptr, 0, szModelFileName, MAX_PATH, nullptr, 0);

		_char szShaderFileName[MAX_PATH] = "";
		_splitpath_s(m_vecShaderPaths[m_iEffectMeshShaderFileSelector], nullptr, 0, nullptr, 0, szShaderFileName, MAX_PATH, nullptr, 0);

#pragma endregion 중복 검사 및 파일이름 추출

		/* 본격적인 모델 생성 */
		wstring wstrFileName = L"";

		for (_uint i = 0; i < MAX_PATH; ++i)
		{
			if (szModelFileName[i] == '\0') { break; }
			wstrFileName += szModelFileName[i];
		}

#pragma region AIScene 생성

		const aiScene* pAIScene;
		MY_AISCENE tMyAISceneNonAnim;

		if (nullptr == CPrefabManager::GetInstance()->GetModelData(wstrFileName))
		{
			pAIScene = CAssimpManager::GetInstance()->CreateModelFromFile(m_vecEffectMeshFilePaths[m_iEffectMeshSelector], m_eEffectMeshModelType);

			tMyAISceneNonAnim = CreateNonAnimModelAIScene(pAIScene);
		}
		else
		{
			tMyAISceneNonAnim = CPrefabManager::GetInstance()->GetModelData(wstrFileName)->tAIScenes;
		}

#pragma endregion AIScene 생성

		// EFFECTMESH_DESC을 준비한다.
		EFFECTMESH_DESC EffectMeshDesc;

#pragma region 파일 이름 및 경로

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecNoiseTextures = CResourceManager::GetInstance()->GetNoises_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecMaskTextures = CResourceManager::GetInstance()->GetMasks_Static();

		// 디퓨즈 텍스처
		wstring wstrDiffuseTextureName1;
		if (m_bUseDiffuseTexture)
			wstrDiffuseTextureName1 = vecDiffuseTextures[m_iDiffuseTextureSelector1].first;
		else
			wstrDiffuseTextureName1 = L"";

		wstring wstrDiffuseTextureName2;
		if (m_bUseDiffuseTexture)
			wstrDiffuseTextureName2 = vecDiffuseTextures[m_iDiffuseTextureSelector2].first;
		else
			wstrDiffuseTextureName2 = L"";

		// 노이즈 텍스처
		wstring wstrNoiseTextureName;
		if (m_bUseNoiseTexture)
			wstrNoiseTextureName = vecNoiseTextures[m_iNoiseTextureSelector].first;
		else
			wstrNoiseTextureName = L"";

		// 마스크 텍스처
		wstring wstrMaskTextureName;
		if (m_bUseMaskTexture)
			wstrMaskTextureName = vecMaskTextures[m_iMaskTextureSelector].first;
		else
			wstrMaskTextureName = L"";

		string	strDiffuseTextureName1 = "";
		string	strDiffuseTextureName2 = "";
		string	strNoiseTextureName = "";
		string	strMaskTextureName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName1[i] == '\0') { break; }
			strDiffuseTextureName1 += wstrDiffuseTextureName1[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName2[i] == '\0') { break; }
			strDiffuseTextureName2 += wstrDiffuseTextureName2[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrNoiseTextureName[i] == '\0') { break; }
			strNoiseTextureName += wstrNoiseTextureName[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrMaskTextureName[i] == '\0') { break; }
			strMaskTextureName += wstrMaskTextureName[i];
		}
#pragma endregion 파일 이름 및 경로

		// 1. 구조체 준비
		strcpy_s(EffectMeshDesc.szModelFilePath, m_vecEffectMeshFilePaths[m_iEffectMeshSelector]);
		strcpy_s(EffectMeshDesc.szModelFileName, szModelFileName);
		strcpy_s(EffectMeshDesc.szShaderFileName, szShaderFileName);

		strcpy_s(EffectMeshDesc.szDiffuseTextureFileName1, strDiffuseTextureName1.c_str());
		strcpy_s(EffectMeshDesc.szDiffuseTextureFileName2, strDiffuseTextureName2.c_str());
		strcpy_s(EffectMeshDesc.szNoiseTextureFileName, strNoiseTextureName.c_str());
		strcpy_s(EffectMeshDesc.szMaskTextureFileName, strMaskTextureName.c_str());

		EffectMeshDesc.vDiffuseBaseColor = m_vEffectMeshDiffuseColor1;
		EffectMeshDesc.vDiffuseLerpColor = m_vEffectMeshDiffuseColor2;
		EffectMeshDesc.bUseDiffuseTexture = m_bUseDiffuseTexture;
		EffectMeshDesc.bUseNoiseTexture = m_bUseNoiseTexture;
		EffectMeshDesc.bUseMaskTexture = m_bUseMaskTexture;

		EffectMeshDesc.eModelType = m_eEffectMeshModelType;
		EffectMeshDesc.eRenderGruop = m_eEffectMeshRenderGroup;

		EffectMeshDesc.vScale = m_vEffectMeshScale;
		EffectMeshDesc.vRotation = m_vEffectMeshRotation;
		EffectMeshDesc.vTranslate = m_vEffectMeshOffset;

		EffectMeshDesc.vLifeTime = m_vEffectMeshLifeTime;

		EffectMeshDesc.bUVMoveMode = m_bEffectMeshUVMove;
		EffectMeshDesc.bUVMoveReverse = m_bEffectMeshUVReverse;
		EffectMeshDesc.bUVMoveLoop = m_bEffectMeshUVMoveLoop;
		EffectMeshDesc.vUVMoveSpeed = m_vEffectMeshUVSpeed;

		EffectMeshDesc.bTransformChange = m_bEffectMeshTransformChange;
		EffectMeshDesc.vDestScale = m_vEffectMeshDestScale;
		EffectMeshDesc.fScaleChangeRatio = m_fEffectMeshScaleChangeRatio;
		EffectMeshDesc.vRotatePerSecond = m_vEffectMeshRotatePerSecond;

		EffectMeshDesc.iNumMeshes = tMyAISceneNonAnim.iNumMeshes;
		EffectMeshDesc.vecMeshs = tMyAISceneNonAnim.vecMeshs;

		// 2. 객체 생성
		shared_ptr<CEffectMeshActor> spEffectMeshActor = m_wpEditorLevel.lock()->GetLayer(LAYER_EFFECTMESH)->CreateActorToPool<CEffectMeshActor>(wstrActorName);

		spEffectMeshActor->SetEffectMeshDesc(EffectMeshDesc);
		spEffectMeshActor->Initialize();
	}
}
void CInspector::UpdateEffectMeshSaveButton()
{
	ImGui::SeparatorText("Save");

	ImGui::InputTextWithHint("FileName", "Effect Mesh FileName", m_chEffectMeshFileName, 64);

	if (ImGui::Button("Effect Mesh Save", ImVec2(150, 30)))
	{
		ofstream json_file;

#pragma region 파일 이름 및 경로

		wstring wstrFileName = L"";
		string strFileName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (m_chEffectMeshFileName[i] == '\0') { break; }

			wstrFileName += m_chEffectMeshFileName[i];
			strFileName += m_chEffectMeshFileName[i];
		}

		const vector<string>& vecEffectMeshFilePaths = CResourceManager::GetInstance()->GetStrFilePaths(RESOURCE_EFFECTMESH);
		const vector<string>& vecShaderFilePaths = CResourceManager::GetInstance()->GetStrFilePaths_Static(RESOURCE_SHADER);

		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecDiffuseTextures = CResourceManager::GetInstance()->GetSRVs_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecNoiseTextures = CResourceManager::GetInstance()->GetNoises_Static();
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecMaskTextures = CResourceManager::GetInstance()->GetMasks_Static();

		wstring wstrDiffuseTextureName1 = vecDiffuseTextures[m_iDiffuseTextureSelector1].first;
		wstring wstrDiffuseTextureName2 = vecDiffuseTextures[m_iDiffuseTextureSelector2].first;
		wstring wstrNoiseTextureName = vecNoiseTextures[m_iNoiseTextureSelector].first;
		wstring wstrMaskTextureName = vecMaskTextures[m_iMaskTextureSelector].first;

		string	strDiffuseTextureName1 = "";
		string	strDiffuseTextureName2 = "";
		string	strNoiseTextureName = "";
		string	strMaskTextureName = "";

		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName1[i] == '\0') { break; }
			strDiffuseTextureName1 += wstrDiffuseTextureName1[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrDiffuseTextureName2[i] == '\0') { break; }
			strDiffuseTextureName2 += wstrDiffuseTextureName2[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrNoiseTextureName[i] == '\0') { break; }
			strNoiseTextureName += wstrNoiseTextureName[i];
		}
		for (_uint i = 0; i < 64; ++i)
		{
			if (wstrMaskTextureName[i] == '\0') { break; }
			strMaskTextureName += wstrMaskTextureName[i];
		}

#pragma endregion 파일 이름 및 경로

		json_file.open(L"../../Resources/Data/Effect/EffectMesh/" + wstrFileName + L".json");

		Json::Value EffectMesh;

		string strEffectMeshFilePath = vecEffectMeshFilePaths[m_iEffectMeshSelector];
		_char szEffectMeshFileName[MAX_PATH] = "";
		_splitpath_s(strEffectMeshFilePath.c_str(), nullptr, 0, nullptr, 0, szEffectMeshFileName, MAX_PATH, nullptr, 0);

		string strShaderFilePath = vecShaderFilePaths[m_iEffectMeshShaderFileSelector];
		_char szShaderFileName[MAX_PATH] = "";
		_splitpath_s(strShaderFilePath.c_str(), nullptr, 0, nullptr, 0, szShaderFileName, MAX_PATH, nullptr, 0);

		/* 1. 기본 정보 */
		EffectMesh["01. FileName"] = strFileName;

		EffectMesh["02. ModelType"] = m_eEffectMeshModelType;
		EffectMesh["03. RenderGroup"] = m_eEffectMeshRenderGroup;

		EffectMesh["04. Scale"].append(m_vEffectMeshScale.x);
		EffectMesh["04. Scale"].append(m_vEffectMeshScale.y);
		EffectMesh["04. Scale"].append(m_vEffectMeshScale.z);

		EffectMesh["05. Rotation"].append(m_vEffectMeshRotation.x);
		EffectMesh["05. Rotation"].append(m_vEffectMeshRotation.y);
		EffectMesh["05. Rotation"].append(m_vEffectMeshRotation.z);

		EffectMesh["06. Translate"].append(m_vEffectMeshOffset.x);
		EffectMesh["06. Translate"].append(m_vEffectMeshOffset.y);
		EffectMesh["06. Translate"].append(m_vEffectMeshOffset.z);

		EffectMesh["07. LifeTime"].append(m_vEffectMeshLifeTime.x);
		EffectMesh["07. LifeTime"].append(m_vEffectMeshLifeTime.y);

		EffectMesh["08. UV Move Mode"] = m_bEffectMeshUVMove;
		EffectMesh["09. UV Move Reverse"] = m_bEffectMeshUVReverse;
		EffectMesh["10. UV Move Loop"] = m_bEffectMeshUVMoveLoop;
		EffectMesh["11. UV MoveSpeed"].append(m_vEffectMeshUVSpeed.x);
		EffectMesh["11. UV MoveSpeed"].append(m_vEffectMeshUVSpeed.y);

		EffectMesh["12. Effect Mesh FileName"] = szEffectMeshFileName;
		EffectMesh["13. Effect Mesh FilePath"] = vecEffectMeshFilePaths[m_iEffectMeshSelector];
		EffectMesh["14. Shader FileName"] = szShaderFileName;

		EffectMesh["15. Diffuse Texture1"] = strDiffuseTextureName1;
		EffectMesh["16. Diffuse Texture2"] = strDiffuseTextureName2;
		EffectMesh["17. Noise Texture"] = strNoiseTextureName;
		EffectMesh["18. Mask Texture"] = strMaskTextureName;

		EffectMesh["19. Diffuse Color1"].append(m_vEffectMeshDiffuseColor1.x);
		EffectMesh["19. Diffuse Color1"].append(m_vEffectMeshDiffuseColor1.y);
		EffectMesh["19. Diffuse Color1"].append(m_vEffectMeshDiffuseColor1.z);
		EffectMesh["19. Diffuse Color1"].append(m_vEffectMeshDiffuseColor1.w);

		EffectMesh["20. Diffuse Color2"].append(m_vEffectMeshDiffuseColor2.x);
		EffectMesh["20. Diffuse Color2"].append(m_vEffectMeshDiffuseColor2.y);
		EffectMesh["20. Diffuse Color2"].append(m_vEffectMeshDiffuseColor2.z);
		EffectMesh["20. Diffuse Color2"].append(m_vEffectMeshDiffuseColor2.w);

		EffectMesh["21. Use Diffuse Texture"] = m_bUseDiffuseTexture;
		EffectMesh["22. Use Noise Texture"] = m_bUseNoiseTexture;
		EffectMesh["23. Use Mask Texture"] = m_bUseMaskTexture;

		EffectMesh["25. Use Transform Change"] = m_bEffectMeshTransformChange;
		EffectMesh["26. Dest Scale"].append(m_vEffectMeshDestScale.x);
		EffectMesh["26. Dest Scale"].append(m_vEffectMeshDestScale.y);
		EffectMesh["26. Dest Scale"].append(m_vEffectMeshDestScale.z);
		EffectMesh["27. Scale Change Ratio"] = m_fEffectMeshScaleChangeRatio;
		EffectMesh["28. Rotate Per Second"].append(m_vEffectMeshRotatePerSecond.x);
		EffectMesh["28. Rotate Per Second"].append(m_vEffectMeshRotatePerSecond.y);
		EffectMesh["28. Rotate Per Second"].append(m_vEffectMeshRotatePerSecond.z);

		/* 2. 메시 */
		// 메시 개수
		Json::Value Mesh;

		// 이펙트 메시 포인터를 가져온다.
		auto iter = m_wpEditorLevel.lock()->GetLayer(LAYER_EFFECTMESH)->GetActors_Pool().begin();

		if (iter == m_wpEditorLevel.lock()->GetLayer(LAYER_EFFECTMESH)->GetActors_Pool().end())
		{
			MSG_BOX("씬에 이펙트 메시가 없습니다. 파일 생성에 실패했습니다. 이펙트 메시를 생성 후에 저장 버튼을 눌러주세요.");
			json_file.close();
			return;
		}

		if (iter->second.empty())
		{
			MSG_BOX("씬에 이펙트 메시가 없습니다. 파일 생성에 실패했습니다. 이펙트 메시를 생성 후에 저장 버튼을 눌러주세요.");
			json_file.close();
			return;
		}

		shared_ptr<CEffectMeshActor> spEffectMeshActor = dynamic_pointer_cast<CEffectMeshActor>(iter->second.front());

		EFFECTMESH_DESC& EffectMeshDesc = spEffectMeshActor->GetEffectMeshDesc();

		/* 메시 */
		Mesh["NumMeshs"] = EffectMeshDesc.iNumMeshes;

		for (_uint i = 0; i < EffectMeshDesc.iNumMeshes; ++i)
		{
			Json::Value VecMeshs;

			// 메시 이름
			VecMeshs[to_string(i)]["MeshName"] = EffectMeshDesc.vecMeshs[i].szName;

			// 정점 개수
			VecMeshs[to_string(i)]["NumVertices"] = EffectMeshDesc.vecMeshs[i].iNumVertices;

			// 삼각형 개수
			VecMeshs[to_string(i)]["NumFaces"] = EffectMeshDesc.vecMeshs[i].iNumFaces;

			Json::Value Vertices;

			for (_uint j = 0; j < EffectMeshDesc.vecMeshs[i].iNumVertices; ++j)
			{
				Json::Value Vertex;

				Json::Value VertexPos;
				VertexPos.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vPosition.x);
				VertexPos.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vPosition.y);
				VertexPos.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vPosition.z);
				Vertex["VertexPos"] = VertexPos;

				Json::Value VertexNor;
				VertexNor.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vNormal.x);
				VertexNor.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vNormal.y);
				VertexNor.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vNormal.z);
				Vertex["VertexNor"] = VertexNor;

				Json::Value VertexTexCoord;
				VertexTexCoord.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vTexcoord.x);
				VertexTexCoord.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vTexcoord.y);
				Vertex["VertexTexCoord"] = VertexTexCoord;

				Json::Value VertexTangent;
				VertexTangent.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vTangent.x);
				VertexTangent.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vTangent.y);
				VertexTangent.append(EffectMeshDesc.vecMeshs[i].vecVertices[j].vTangent.z);
				Vertex["VertexTangent"] = VertexTangent;

				Vertices.append(Vertex);
			}
			VecMeshs[to_string(i)]["vecVertices"] = Vertices;


			Json::Value Indices;

			for (_uint j = 0; j < EffectMeshDesc.vecMeshs[i].iNumFaces; ++j)
			{
				Json::Value Face;
				Face.append(EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[0]);
				Face.append(EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[1]);
				Face.append(EffectMeshDesc.vecMeshs[i].vecFaces[j].mIndices[2]);

				Indices.append(Face);
			}
			VecMeshs[to_string(i)]["vecIndices"] = Indices;

			Mesh["Mesh_" + to_string(i)] = VecMeshs[to_string(i)];
		}
		EffectMesh["24. Mesh"] = Mesh;


		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "	";	// Tab
		unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

		writer->write(EffectMesh, &cout);
		writer->write(EffectMesh, &json_file);

		json_file.close();
	}
}
