#pragma once
#include "Window.h"

BEGIN(Engine)
class CActor;
END

class CSelectedActorWindow : public CWindow
{
public:
	explicit CSelectedActorWindow() = default;
	virtual ~CSelectedActorWindow() = default;

public:
	HRESULT Initialize(std::weak_ptr<CLevel> _wpLevel);
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void EndPlay();
	void Render();
	void Release();

private:
	void UpdateSelectedActorInfo();
		void UpdateNaviActorList();
		void UpdateNaviActorRemoveButton();

		void UpdateTerrainActorList();
		void UpdateTerrainActorRemoveButton();

		void UpdateModelActorList();
		void UpdateModelActorRemoveButton();

		void UpdateParticleActorList();
		void UpdateParticleActorPlayButton();
		void UpdateParticleActorPauseButton();
		void UpdateParticleActorRemoveButton();

		void UpdateEffectMeshActorList();
		void UpdateEffectMeshActorPlayButton();
		void UpdateEffectMeshActorPauseButton();
		void UpdateEffectMeshActorRemoveButton();

private:
	_bool my_tool_active = true;

// =================================== Actor List =================================== //
private:
	vector<string> m_vecLayerActors;

	_int m_iActorListSelectedIndex = 0;
	_int m_iSelectedActorLayerIndex = 0;

	wstring m_wstrActorName = L"";
	string m_strActorName = "";

// =================================== Navigation List ================================ //
private:
	vector<string> m_vecLayerNavigations;

	_int m_iNavigationListSelectedIndex = 0;
	_int m_iSelectedNavigationLayerIndex = 0;

	wstring m_wstrNavigationName = L"";
	string m_strNavigationName = "";

// =================================== Terrain List =================================== //
private:
	vector<string> m_vecLayerTerrains;

	_int m_iTerrainListSelectedIndex = 0;
	_int m_iSelectedTerrainLayerIndex = 0;

	wstring m_wstrTerrainName = L"";
	string m_strTerrainName = "";

// =================================== Particle List =================================== //
private:
	vector<string> m_vecLayerParticles;

	_int m_iParticleListSelectedIndex = 0;
	_int m_iSelectedParticleLayerIndex = 0;

	wstring m_wstrParticleName = L"";
	string m_strParticleName = "";

// =================================== Effect List =================================== //
private:
	vector<string> m_vecLayerEffects;

	_int m_iEffectListSelectedIndex = 0;
	_int m_iSelectedEffectLayerIndex = 0;

	wstring m_wstrEffectName = L"";
	string m_strEffectName = "";
};
