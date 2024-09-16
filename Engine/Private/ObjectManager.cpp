#include "Engine_pch.h"
#include "ObjectManager.h"

#include "ResourceManager.h"
#include "CollisionManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "LightManager.h"
#include "NavigationManager.h"
#include "CameraManager.h"

#include "Level.h"
#include "Layer.h"
#include "Actor.h"

IMPLEMENT_SINGLETON(CObjectManager)

HRESULT CObjectManager::Initialize(_uint _iNumLevels, _uint _iNumLayers)
{
	// 레벨 개수만큼 공간을 확보합니다.
	if (FAILED(ReserveLevels(_iNumLevels, _iNumLayers)))
		return E_FAIL;

	return S_OK;
}

_int CObjectManager::PreUpdate(const _float& _fTimeDelta)
{
	if (nullptr != m_vecLevels[0] && !m_bLoading)
	{
		// 스태틱 레벨
		_uint iResult_Static = m_vecLevels[0]->PreUpdate(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("스태틱 레벨 PreUpdate Result가 S_OK가 아님");
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// 현재 레벨
		_uint iResult = m_vecLevels[m_iCurLevelIndex]->PreUpdate(_fTimeDelta);

		if (iResult != S_OK)
			return iResult;
	}

	return S_OK;
}

_int CObjectManager::Update(const _float& _fTimeDelta)
{
	if (nullptr != m_vecLevels[0] && !m_bLoading)
	{
		// 스태틱 레벨
		_uint iResult_Static = m_vecLevels[0]->Update(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("스태틱 레벨 Update Result가 S_OK가 아님");
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// 현재 레벨
		_uint iResult = m_vecLevels[m_iCurLevelIndex]->Update(_fTimeDelta);

		if (iResult != S_OK)
			return iResult;
	}

	return S_OK;
}

_int CObjectManager::LateUpdate(const _float& _fTimeDelta)
{
	if (nullptr != m_vecLevels[0] && !m_bLoading)
	{
		// 스태틱 레벨
		_uint iResult_Static = m_vecLevels[0]->LateUpdate(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("스태틱 레벨 LateUpdate Result가 S_OK가 아님");
	}
	
	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// 현재 레벨
		_uint iResult = m_vecLevels[m_iCurLevelIndex]->LateUpdate(_fTimeDelta);

		if (iResult != S_OK)
			return iResult;
	}

	return S_OK;
}

void CObjectManager::Render()
{
	if (nullptr != m_vecLevels[0] && !m_bLoading)
	{
		// 스태틱 레벨
		m_vecLevels[0]->Render();
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		m_vecLevels[m_iCurLevelIndex]->Render();
	}
}

void CObjectManager::EndPlay()
{
	// 스태틱 레벨
	if (nullptr != m_vecLevels[0] && !m_bLoading)
		m_vecLevels[0]->EndPlay();

	// 현재 레벨
	if (nullptr != m_vecLevels[m_iCurLevelIndex])
		m_vecLevels[m_iCurLevelIndex]->EndPlay();
}

void CObjectManager::Release()
{
	for (auto& level : m_vecLevels)
	{
		if (nullptr != level)
		{
			level->Release();
			level.reset();
		}
	}

	m_vecLevels.clear();

	// 오브젝트 매니저 파괴
	m_upInstance.reset();
}

HRESULT CObjectManager::AddActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor)
{
#pragma region 예외 처리
	if (nullptr == _spActor)
		return E_FAIL;

	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return E_FAIL;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return E_FAIL;
#pragma endregion 예외 처리

	// 이름 중복을 검사합니다.
	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// 중복된 이름의 액터가 존재하는 경우 E_FAIL을 반환합니다.
	if (nullptr != spActor)
		return E_FAIL;

	// 그렇지 않은 경우 레벨의 레이어에 액터를 생성하여 추가합니다.
	m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->CreateActor<CActor>(_wstrActorName);

	return S_OK;
}

HRESULT CObjectManager::InsertActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor)
{
#pragma region 예외 처리
	if (nullptr == _spActor)
		return E_FAIL;

	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return E_FAIL;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return E_FAIL;
#pragma endregion 예외 처리

	// 이름 중복을 검사합니다.
	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// 중복된 이름의 액터가 존재하는 경우 E_FAIL을 반환합니다.
	if (nullptr != spActor)
		return E_FAIL;

	// 그렇지 않은 경우 레벨의 레이어에 ※이미 생성되어 세팅이 다 된※ 액터를 단순히 추가만합니다.
	m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->InsertActor(_wstrActorName, _spActor);

	return S_OK;
}

shared_ptr<CActor> CObjectManager::FindActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName)
{
#pragma region 예외 처리
	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return nullptr;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return nullptr;
#pragma endregion 예외 처리

	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// 찾고자 하는 액터가 존재하지 않는 경우 E_FAIL을 반환합니다.
	if (nullptr == spActor)
		return nullptr;

	// 그렇지 않은 경우 찾은 액터를 반환합니다.
	return spActor;
}

HRESULT CObjectManager::ReserveLevels(_uint _iNumLevels, _uint _iNumLayers)
{
	// 이미 레벨 벡터가 초기화 되어있을 경우, E_FAIL을 반환합니다.
	if (!m_vecLevels.empty())
		return E_FAIL;

	// 지정된 레벨 개수만큼 공간을 확보합니다.
	m_vecLevels.reserve(_iNumLevels);

	// 지정된 레벨 개수와 레이어 개수에 따라 레벨을 생성합니다.
	for (_uint iLevelIndex = 0; iLevelIndex < _iNumLevels; ++iLevelIndex)
		CreateLevel(iLevelIndex, _iNumLayers);

	m_iNumLevels = _iNumLevels;
	m_iNumLayers = _iNumLayers;

	return S_OK;
}

shared_ptr<CLevel> CObjectManager::CreateLevel(_uint _iLevelIndex, _uint _iNumLayers)
{
	shared_ptr<CLevel> spLevel = make_shared<CLevel>();
	spLevel->SetLevelIndex(_iLevelIndex);
	spLevel->ReserveLayers(_iNumLayers);

	m_vecLevels.push_back(spLevel);
	return spLevel;
}

shared_ptr<CLevel> CObjectManager::FindLevel(_uint _iLevelIndex)
{
#pragma region 예외 처리
	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return nullptr;
#pragma endregion 예외 처리

	return m_vecLevels[_iLevelIndex];
}

HRESULT CObjectManager::OpenLevel(shared_ptr<CLevel> _spNewLevel)
{
	_uint iPrevLevelIndex = m_iCurLevelIndex;

	// 이전 레벨 리소스 정리
	if (0 != m_iCurLevelIndex) /* 스태틱 레벨은 정리하지 않습니다. */
		// CResourceManager::GetInstance()->ClearCurrentScene(); -> Loader로 이전

	//// 이전 레벨 오브젝트 정리
	//if (0 != m_iCurLevelIndex) /* 스태틱 레벨은 정리하지 않습니다. */
	//	CObjectManager::GetInstance()->ClearCurrentScene(m_iCurLevelIndex);

	// 이전 레벨 프리팹 데이터 정리(스태틱 레벨은 추후에 추가해야함)
	if (0 != m_iCurLevelIndex)
		CPrefabManager::GetInstance()->ClearCurrentScene(m_iCurLevelIndex);

	// 이전 레벨 스폰 데이터 정리(스태틱 레벨은 추후에 추가해야함)
	if (0 != m_iCurLevelIndex)
		CSpawnManager::GetInstance()->ClearCurrentScene();

	if (0 != m_iCurLevelIndex)
		CCollisionManager::GetInstance()->ClearCurrentScene();

	if (0 != m_iCurLevelIndex)
		CLightManager::GetInstance()->ClearCurrentScene();

	if (0 != m_iCurLevelIndex)
		CNavigationManager::GetInstance()->ClearCurrentScene();

	if (0 != m_iCurLevelIndex)
		CCameraManager::GetInstance()->ClearCurrentScene();

	// 현재 레벨을 새로운 레벨로 변경
	m_spCurLevel = _spNewLevel;
	m_iCurLevelIndex = _spNewLevel->GetLevelIndex();

	m_vecLevels[m_iCurLevelIndex] = _spNewLevel;

	m_vecLevels[m_iCurLevelIndex]->Initialize(m_iNumLayers);

	// 이전 레벨 오브젝트 정리
	if (0 != m_iCurLevelIndex) /* 스태틱 레벨은 정리하지 않습니다. */
		CObjectManager::GetInstance()->ClearCurrentScene(iPrevLevelIndex);

	return S_OK;
}

void CObjectManager::ClearCurrentScene(_uint _iLevelIndex)
{
	if (m_vecLevels[_iLevelIndex] && _iLevelIndex != 0 /* 스태틱 레벨은 삭제하지 않는다. */)
	{
		m_vecLevels[_iLevelIndex]->Release();
		m_vecLevels[_iLevelIndex].reset();
	}
}
