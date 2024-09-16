#include "pch.h"
#include "Level_Editor.h"

#include "ImguiManager.h"
#include "LightManager.h"

#include "Layer.h"
#include "Inspector.h"
#include "SelectedActorWindow.h"

#include "Editor_Navi.h"
#include "FreeCam.h"
#include "Test_Object_1.h"

void CLevel_Editor::Initialize(_uint _iNumLayers)
{
	ReadyLights();

	// LoadLevelDataFromFileName(L"Level_Editor");

	m_spInspector = make_shared<CInspector>();
	m_spInspector->Initialize(shared_from_this());

	m_spSelectedActorWindow = make_shared<CSelectedActorWindow>();
	m_spSelectedActorWindow->Initialize(shared_from_this());

	m_vecLayers[LAYER::LAYER_PLAYER]->CreateActor<FreeCam>(L"Actor_Player");
	m_vecLayers[LAYER::LAYER_NAVIGATION]->CreateActor<CEditor_Navi>(L"Actor_Navigation");

	CLevel::Initialize(_iNumLayers);
}

_int CLevel_Editor::PreUpdate(const _float& _fTimeDelta)
{
	if(m_spInspector)
		m_spInspector->PreUpdate(_fTimeDelta);

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->PreUpdate(_fTimeDelta);

	return CLevel::PreUpdate(_fTimeDelta);
}

_int CLevel_Editor::Update(const _float& _fTimeDelta)
{
	CImguiManager::GetInstance()->UpdateBegin();

	if (m_spInspector)
		m_spInspector->Update(_fTimeDelta);

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->Update(_fTimeDelta);

	CImguiManager::GetInstance()->UpdateEnd();

	return CLevel::Update(_fTimeDelta);
}

_int CLevel_Editor::LateUpdate(const _float& _fTimeDelta)
{
	if (m_spInspector)
		m_spInspector->LateUpdate(_fTimeDelta);

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->LateUpdate(_fTimeDelta);

	return CLevel::LateUpdate(_fTimeDelta);
}

void CLevel_Editor::Render()
{
	if (m_spInspector)
		m_spInspector->Render();

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->Render();

	CLevel::Render();
}

void CLevel_Editor::EndPlay()
{
	if (m_spInspector)
		m_spInspector->EndPlay();

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->EndPlay();

	CLevel::EndPlay();
}

void CLevel_Editor::Release()
{
	if (m_spInspector)
		m_spInspector->Release();

	if (m_spSelectedActorWindow)
		m_spSelectedActorWindow->Release();

	CLevel::Release();
}

void CLevel_Editor::InitializeObject()
{
	CLevel::Initialize(Client::LAYER_END);
}

void CLevel_Editor::ReleaseObject()
{
	CLevel::Release();

	ReserveLayers(Client::LAYER_END);
}

HRESULT CLevel_Editor::ReadyLights()
{
	LIGHT_DESC tLightDesc = {};

	tLightDesc.eLightType = LIGHT_DESC::TYPE_DIRECTIONAL;
	tLightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	tLightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	tLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(CLightManager::GetInstance()->AddLight(tLightDesc)))
		return E_FAIL;

	return S_OK;
}
