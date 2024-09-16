#include "pch.h"
#include "SelectedActorWindow.h"

#include "ObjectManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"

#include "Level_Editor.h"
#include "Layer.h"
#include "Actor.h"
#include "MeshActor.h"
#include "EffectMeshActor.h"
#include "ParticleActor.h"

HRESULT CSelectedActorWindow::Initialize(std::weak_ptr<CLevel> _wpLevel)
{
	m_wpEditorLevel = dynamic_pointer_cast<CLevel_Editor>(_wpLevel.lock());

	return CWindow::Initialize(_wpLevel);
}

_int CSelectedActorWindow::PreUpdate(const _float& _fTimeDelta)
{
	return CWindow::PreUpdate(_fTimeDelta);
}

_int CSelectedActorWindow::Update(const _float& _fTimeDelta)
{
	if (ImGui::Begin("Scene Hierarchy", &my_tool_active))
	{
		UpdateSelectedActorInfo();

		ImGui::End();
	}

	return CWindow::Update(_fTimeDelta);
}

_int CSelectedActorWindow::LateUpdate(const _float& _fTimeDelta)
{
	return CWindow::LateUpdate(_fTimeDelta);
}

void CSelectedActorWindow::EndPlay()
{
	CWindow::EndPlay();
}

void CSelectedActorWindow::Render()
{
	CWindow::Render();
}

void CSelectedActorWindow::Release()
{
	CWindow::Release();
}

void CSelectedActorWindow::UpdateSelectedActorInfo()
{
	if (ImGui::CollapsingHeader("Navigation", ImGuiTreeNodeFlags_None))
	{
		UpdateNaviActorList();
		UpdateNaviActorRemoveButton();
	}

	if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_None))
	{
		UpdateTerrainActorList();
		UpdateTerrainActorRemoveButton();
	}

	UpdateModelActorList();
	UpdateModelActorRemoveButton();

	UpdateParticleActorList();
	UpdateParticleActorPlayButton();
	UpdateParticleActorPauseButton();
	UpdateParticleActorRemoveButton();

	UpdateEffectMeshActorList();
	UpdateEffectMeshActorPlayButton();
	UpdateEffectMeshActorPauseButton();
	UpdateEffectMeshActorRemoveButton();
}

void CSelectedActorWindow::UpdateModelActorList()
{
	const shared_ptr<CLevel>& spCurLevel = CObjectManager::GetInstance()->GetCurLevel();

	ImGui::SeparatorText("Model Actor");

	if (ImGui::BeginListBox("Model Actor", ImVec2(250, 100)))
	{
		m_vecLayerActors.clear();

		// Level
		for (_uint i = 0; i < Client::LAYER_END; ++i)
		{
			// ※ 이펙트와 네비게이션은 따로 다른 리스트에서 표기한다. ※
			if (i == Client::LAYER_PARTICLE || i == Client::LAYER_EFFECTMESH || i == Client::LAYER_NAVIGATION)
				continue;

			// Layer
			for (auto& Actor : spCurLevel->GetLayer(i)->GetActors())
			{
				// Actor
				m_wstrActorName = Actor.second->GetActorName();
				m_strActorName = "";

				for (_uint i = 0; i < 64; ++i)
				{
					if (m_wstrActorName[i] == '\0') { break; }
					m_strActorName += m_wstrActorName[i];
				}

				string strTemp = m_strActorName;

				m_vecLayerActors.push_back(strTemp);

				const bool isRectSelected = (m_iActorListSelectedIndex == m_vecLayerActors.size() - 1);
				if (ImGui::Selectable(m_vecLayerActors[m_vecLayerActors.size() - 1].c_str(), ImGuiSelectableFlags_None, isRectSelected))
				{
					m_iActorListSelectedIndex = (_int)(m_vecLayerActors.size() - 1);

					m_iSelectedActorLayerIndex = Actor.second->GetLayer().lock()->GetLayerIndex();
				}
			}
		}

		ImGui::EndListBox();
	}
}
void CSelectedActorWindow::UpdateModelActorRemoveButton()
{
	if (ImGui::Button("Remove Model", ImVec2(150, 30)))
	{
		if (m_vecLayerActors.empty())
			return;

		wstring wstrActorName = L"";
		string strActorName = "";
		for (_uint i = 0; i < m_vecLayerActors[m_iActorListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerActors[m_iActorListSelectedIndex][i] == '\0') { break; }
			else 
			{ 
				strActorName += m_vecLayerActors[m_iActorListSelectedIndex][i];
				wstrActorName += m_vecLayerActors[m_iActorListSelectedIndex][i];
			}
		}

		// 플레이어는 삭제될 수 없다.
		if (wstrActorName == L"Actor_Player")
			return;

		// 프리팹 삭제용으로 모델파일명을 기록해놓자.
		shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(m_iSelectedActorLayerIndex)->FindActor(wstrActorName);
		wstring wstrModelFileName = dynamic_pointer_cast<CMeshActor>(spActor)->GetModelFileName();

		CPrefabManager::GetInstance()->RemoveModelData(wstrModelFileName);
		CSpawnManager::GetInstance()->RemoveSpawnData(strActorName);
		CObjectManager::GetInstance()->GetCurLevel()->GetLayer(m_iSelectedActorLayerIndex)->RemoveActor(wstrActorName);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void CSelectedActorWindow::UpdateNaviActorList()
{
	const shared_ptr<CLevel>& spCurLevel = CObjectManager::GetInstance()->GetCurLevel();

	ImGui::SeparatorText("Navigation");

	if (ImGui::BeginListBox("Navigation", ImVec2(250, 30)))
	{
		m_vecLayerNavigations.clear();

		/// Layer
		for (auto& Actor : spCurLevel->GetLayer(Client::LAYER_NAVIGATION)->GetActors())
		{
			// Actor
			m_wstrNavigationName = Actor.second->GetActorName();
			m_strNavigationName = "";

			for (_uint i = 0; i < 64; ++i)
			{
				if (m_wstrNavigationName[i] == '\0') { break; }
				m_strNavigationName += m_wstrNavigationName[i];
			}

			string strTemp = m_strNavigationName;

			m_vecLayerNavigations.push_back(strTemp);

			const bool isRectSelected = (m_iNavigationListSelectedIndex == m_vecLayerNavigations.size() - 1);

			if (ImGui::Selectable(m_vecLayerNavigations[m_vecLayerNavigations.size() - 1].c_str(), ImGuiSelectableFlags_None, isRectSelected))
			{
				m_iNavigationListSelectedIndex = (_int)(m_vecLayerNavigations.size() - 1);

				m_iSelectedNavigationLayerIndex = Actor.second->GetLayer().lock()->GetLayerIndex();
			}
		}

		ImGui::EndListBox();
	}
}
void CSelectedActorWindow::UpdateNaviActorRemoveButton()
{
	if (ImGui::Button("Remove Navi", ImVec2(150, 30)))
	{
		if (m_vecLayerNavigations.empty())
			return;

		wstring wstrNavigationName = L"";
		for (_uint i = 0; i < m_vecLayerNavigations[m_iNavigationListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerNavigations[m_iNavigationListSelectedIndex][i] == '\0') { break; }
			else { wstrNavigationName += m_vecLayerNavigations[m_iNavigationListSelectedIndex][i]; }
		}

		CObjectManager::GetInstance()->GetCurLevel()->GetLayer(m_iSelectedNavigationLayerIndex)->RemoveActor(wstrNavigationName);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void CSelectedActorWindow::UpdateTerrainActorList()
{
	const shared_ptr<CLevel>& spCurLevel = CObjectManager::GetInstance()->GetCurLevel();

	ImGui::SeparatorText("Terrain");

	if (ImGui::BeginListBox("Terrain", ImVec2(250, 100)))
	{
		m_vecLayerTerrains.clear();

		/// Layer
		for (auto& Actor : spCurLevel->GetLayer(Client::LAYER_TERRAIN)->GetActors())
		{
			// Actor
			m_wstrTerrainName = Actor.second->GetActorName();
			m_strTerrainName = "";

			for (_uint i = 0; i < 64; ++i)
			{
				if (m_wstrTerrainName[i] == '\0') { break; }
				m_strTerrainName += m_wstrTerrainName[i];
			}

			string strTemp = m_strTerrainName;

			m_vecLayerTerrains.push_back(strTemp);

			const bool isRectSelected = (m_iTerrainListSelectedIndex == m_vecLayerTerrains.size() - 1);

			if (ImGui::Selectable(m_vecLayerTerrains[m_vecLayerTerrains.size() - 1].c_str(), ImGuiSelectableFlags_None, isRectSelected))
			{
				m_iTerrainListSelectedIndex = (_int)(m_vecLayerTerrains.size() - 1);

				m_iSelectedTerrainLayerIndex = Actor.second->GetLayer().lock()->GetLayerIndex();
			}
		}

		ImGui::EndListBox();
	}
}
void CSelectedActorWindow::UpdateTerrainActorRemoveButton()
{
	if (ImGui::Button("Remove Terrain", ImVec2(150, 30)))
	{
		if (m_vecLayerTerrains.empty())
			return;

		wstring wstrTerrainName = L"";
		string strTerrainName = "";
		for (_uint i = 0; i < m_vecLayerTerrains[m_iTerrainListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerTerrains[m_iTerrainListSelectedIndex][i] == '\0') { break; }
			else 
			{
				strTerrainName += m_vecLayerTerrains[m_iTerrainListSelectedIndex][i];
				wstrTerrainName += m_vecLayerTerrains[m_iTerrainListSelectedIndex][i];
			}
		}

		// 프리팹 삭제용으로 모델파일명을 기록해놓자.
		shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(m_iSelectedActorLayerIndex)->FindActor(wstrTerrainName);

		wstring wstrModelFileName = dynamic_pointer_cast<CMeshActor>(spActor)->GetModelFileName();

		CPrefabManager::GetInstance()->RemoveModelData(wstrTerrainName);
		CSpawnManager::GetInstance()->RemoveSpawnData(strTerrainName);
		CObjectManager::GetInstance()->GetCurLevel()->GetLayer(m_iSelectedActorLayerIndex)->RemoveActor(wstrTerrainName);
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void CSelectedActorWindow::UpdateParticleActorList()
{
	const shared_ptr<CLevel>& spCurLevel = CObjectManager::GetInstance()->GetCurLevel();

	ImGui::SeparatorText("Particle");

	if (ImGui::BeginListBox("Particle", ImVec2(250, 100)))
	{
		m_vecLayerParticles.clear();

		/// Layer
		for (auto& Pair : spCurLevel->GetLayer(Client::LAYER_PARTICLE)->GetActors_Pool())
		{
			for (auto& Actor : Pair.second)
			{
				// Actor
				m_wstrParticleName = Actor->GetActorName();
				m_strParticleName = "";

				for (_uint i = 0; i < 64; ++i)
				{
					if (m_wstrParticleName[i] == '\0') { break; }
					m_strParticleName += m_wstrParticleName[i];
				}

				string strTemp = m_strParticleName;

				m_vecLayerParticles.push_back(strTemp);

				const bool isRectSelected = (m_iParticleListSelectedIndex == m_vecLayerParticles.size() - 1);
				if (ImGui::Selectable(m_vecLayerParticles[m_vecLayerParticles.size() - 1].c_str(), ImGuiSelectableFlags_None, isRectSelected))
				{
					m_iParticleListSelectedIndex = (_int)(m_vecLayerParticles.size() - 1);

					m_iSelectedParticleLayerIndex = Actor->GetLayer().lock()->GetLayerIndex();
				}
			}
		}

		ImGui::EndListBox();
	}
}
void CSelectedActorWindow::UpdateParticleActorPlayButton()
{
	if (ImGui::Button("Play Particle", ImVec2(150, 40)))
	{
		if (m_vecLayerParticles.empty())
			return;

		wstring wstrParticleName = L"";
		for (_uint i = 0; i < m_vecLayerParticles[m_iParticleListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerParticles[m_iParticleListSelectedIndex][i] == '\0') { break; }
			else { wstrParticleName += m_vecLayerParticles[m_iParticleListSelectedIndex][i]; }
		}

		shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_PARTICLE)->FindActor_Pool(wstrParticleName, wstrParticleName);
		if (nullptr == spActor)
			return;

		dynamic_pointer_cast<CParticleActor>(spActor)->RecycleBegin();
	}
}
void CSelectedActorWindow::UpdateParticleActorPauseButton()
{
	ImGui::SameLine();
	if (ImGui::Button("Pause_P", ImVec2(50, 40)))
	{
		if (m_vecLayerParticles.empty())
			return;

		auto iter = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_PARTICLE)->GetActors_Pool().begin();
		if (iter == CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_PARTICLE)->GetActors_Pool().end())
			return;

		if (iter->second.empty())
			return;

		shared_ptr<CActor> spParticleActor = iter->second.front();

		if (nullptr == spParticleActor)
			return;

		if (dynamic_pointer_cast<CParticleActor>(spParticleActor)->GetPause())
			dynamic_pointer_cast<CParticleActor>(spParticleActor)->SetPause(false);
		else
			dynamic_pointer_cast<CParticleActor>(spParticleActor)->SetPause(true);
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CSelectedActorWindow::UpdateParticleActorRemoveButton()
{
	if (ImGui::Button("Remove Particle", ImVec2(150, 30)))
	{
		if (m_vecLayerParticles.empty())
			return;

		wstring wstrParticleName = L"";
		for (_uint i = 0; i < m_vecLayerParticles[m_iParticleListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerParticles[m_iParticleListSelectedIndex][i] == '\0') { break; }
			else { wstrParticleName += m_vecLayerParticles[m_iParticleListSelectedIndex][i]; }
		}

		CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_PARTICLE)->RemoveActor_Pool(wstrParticleName);
		// CPrefabManager::GetInstance()->RemoveModelData()
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}

void CSelectedActorWindow::UpdateEffectMeshActorList()
{
	const shared_ptr<CLevel>& spCurLevel = CObjectManager::GetInstance()->GetCurLevel();

	ImGui::SeparatorText("EffectMesh");

	if (ImGui::BeginListBox("EffectMesh", ImVec2(250, 100)))
	{
		m_vecLayerEffects.clear();

		/// Layer
		for (auto& Pair : spCurLevel->GetLayer(Client::LAYER_EFFECTMESH)->GetActors_Pool())
		{
			for (auto& Actor : Pair.second)
			{
				// Actor
				m_wstrEffectName = Actor->GetActorName();
				m_strEffectName = "";

				for (_uint i = 0; i < 64; ++i)
				{
					if (m_wstrEffectName[i] == '\0') { break; }
					m_strEffectName += m_wstrEffectName[i];
				}

				string strTemp = m_strEffectName;

				m_vecLayerEffects.push_back(strTemp);

				const bool isRectSelected = (m_iEffectListSelectedIndex == m_vecLayerEffects.size() - 1);
				if (ImGui::Selectable(m_vecLayerEffects[m_vecLayerEffects.size() - 1].c_str(), ImGuiSelectableFlags_None, isRectSelected))
				{
					m_iEffectListSelectedIndex = (_int)(m_vecLayerEffects.size() - 1);

					m_iSelectedEffectLayerIndex = Actor->GetLayer().lock()->GetLayerIndex();
				}
			}
		}

		ImGui::EndListBox();
	}
}
void CSelectedActorWindow::UpdateEffectMeshActorPlayButton()
{
	if (ImGui::Button("Play EffectMesh", ImVec2(150, 40)))
	{
		if (m_vecLayerEffects.empty())
			return;

		wstring wstrEffectName = L"";
		for (_uint i = 0; i < m_vecLayerEffects[m_iEffectListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerEffects[m_iEffectListSelectedIndex][i] == '\0') { break; }
			else { wstrEffectName += m_vecLayerEffects[m_iEffectListSelectedIndex][i]; }
		}

		shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_EFFECTMESH)->FindActor_Pool(wstrEffectName, wstrEffectName);
		if (nullptr == spActor)
			return;

		dynamic_pointer_cast<CEffectMeshActor>(spActor)->RecycleBegin();
	}
}
void CSelectedActorWindow::UpdateEffectMeshActorPauseButton()
{
	ImGui::SameLine();
	if (ImGui::Button("Pause_E", ImVec2(50, 40)))
	{
		if (m_vecLayerEffects.empty())
			return;

		wstring wstrEffectName = L"";
		for (_uint i = 0; i < m_vecLayerEffects[m_iEffectListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerEffects[m_iEffectListSelectedIndex][i] == '\0') { break; }
			else { wstrEffectName += m_vecLayerEffects[m_iEffectListSelectedIndex][i]; }
		}

		shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_EFFECTMESH)->FindActor_Pool(wstrEffectName, wstrEffectName);
		if (nullptr == spActor)
			return;

		if (dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetPause())
			dynamic_pointer_cast<CEffectMeshActor>(spActor)->SetPause(false);
		else
			dynamic_pointer_cast<CEffectMeshActor>(spActor)->SetPause(true);
	}

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
}
void CSelectedActorWindow::UpdateEffectMeshActorRemoveButton()
{
	if (ImGui::Button("Remove EffectMesh", ImVec2(150, 40)))
	{
		if (m_vecLayerEffects.empty())
			return;

		wstring wstrEffectName = L"";
		for (_uint i = 0; i < m_vecLayerEffects[m_iEffectListSelectedIndex].size(); ++i)
		{
			if (m_vecLayerEffects[m_iEffectListSelectedIndex][i] == '\0') { break; }
			else { wstrEffectName += m_vecLayerEffects[m_iEffectListSelectedIndex][i]; }
		}

		CObjectManager::GetInstance()->GetCurLevel()->GetLayer(Client::LAYER_EFFECTMESH)->RemoveActor_Pool(wstrEffectName);
		// CPrefabManager::GetInstance()->RemoveModelData()
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
}