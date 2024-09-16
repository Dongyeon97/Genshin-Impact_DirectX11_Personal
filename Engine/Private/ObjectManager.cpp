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
	// ���� ������ŭ ������ Ȯ���մϴ�.
	if (FAILED(ReserveLevels(_iNumLevels, _iNumLayers)))
		return E_FAIL;

	return S_OK;
}

_int CObjectManager::PreUpdate(const _float& _fTimeDelta)
{
	if (nullptr != m_vecLevels[0] && !m_bLoading)
	{
		// ����ƽ ����
		_uint iResult_Static = m_vecLevels[0]->PreUpdate(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("����ƽ ���� PreUpdate Result�� S_OK�� �ƴ�");
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// ���� ����
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
		// ����ƽ ����
		_uint iResult_Static = m_vecLevels[0]->Update(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("����ƽ ���� Update Result�� S_OK�� �ƴ�");
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// ���� ����
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
		// ����ƽ ����
		_uint iResult_Static = m_vecLevels[0]->LateUpdate(_fTimeDelta);

		if (iResult_Static != S_OK)
			MSG_BOX("����ƽ ���� LateUpdate Result�� S_OK�� �ƴ�");
	}
	
	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		// ���� ����
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
		// ����ƽ ����
		m_vecLevels[0]->Render();
	}

	if (nullptr != m_vecLevels[m_iCurLevelIndex])
	{
		m_vecLevels[m_iCurLevelIndex]->Render();
	}
}

void CObjectManager::EndPlay()
{
	// ����ƽ ����
	if (nullptr != m_vecLevels[0] && !m_bLoading)
		m_vecLevels[0]->EndPlay();

	// ���� ����
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

	// ������Ʈ �Ŵ��� �ı�
	m_upInstance.reset();
}

HRESULT CObjectManager::AddActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor)
{
#pragma region ���� ó��
	if (nullptr == _spActor)
		return E_FAIL;

	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return E_FAIL;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return E_FAIL;
#pragma endregion ���� ó��

	// �̸� �ߺ��� �˻��մϴ�.
	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// �ߺ��� �̸��� ���Ͱ� �����ϴ� ��� E_FAIL�� ��ȯ�մϴ�.
	if (nullptr != spActor)
		return E_FAIL;

	// �׷��� ���� ��� ������ ���̾ ���͸� �����Ͽ� �߰��մϴ�.
	m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->CreateActor<CActor>(_wstrActorName);

	return S_OK;
}

HRESULT CObjectManager::InsertActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor)
{
#pragma region ���� ó��
	if (nullptr == _spActor)
		return E_FAIL;

	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return E_FAIL;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return E_FAIL;
#pragma endregion ���� ó��

	// �̸� �ߺ��� �˻��մϴ�.
	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// �ߺ��� �̸��� ���Ͱ� �����ϴ� ��� E_FAIL�� ��ȯ�մϴ�.
	if (nullptr != spActor)
		return E_FAIL;

	// �׷��� ���� ��� ������ ���̾ ���̹� �����Ǿ� ������ �� �ȡ� ���͸� �ܼ��� �߰����մϴ�.
	m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->InsertActor(_wstrActorName, _spActor);

	return S_OK;
}

shared_ptr<CActor> CObjectManager::FindActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName)
{
#pragma region ���� ó��
	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return nullptr;

	if (_iLayerIndex >= m_iNumLayers ||
		_iLayerIndex < 0)
		return nullptr;
#pragma endregion ���� ó��

	shared_ptr<CActor> spActor = m_vecLevels[_iLevelIndex]->FindLayer(_iLayerIndex)->FindActor(_wstrActorName);

	// ã���� �ϴ� ���Ͱ� �������� �ʴ� ��� E_FAIL�� ��ȯ�մϴ�.
	if (nullptr == spActor)
		return nullptr;

	// �׷��� ���� ��� ã�� ���͸� ��ȯ�մϴ�.
	return spActor;
}

HRESULT CObjectManager::ReserveLevels(_uint _iNumLevels, _uint _iNumLayers)
{
	// �̹� ���� ���Ͱ� �ʱ�ȭ �Ǿ����� ���, E_FAIL�� ��ȯ�մϴ�.
	if (!m_vecLevels.empty())
		return E_FAIL;

	// ������ ���� ������ŭ ������ Ȯ���մϴ�.
	m_vecLevels.reserve(_iNumLevels);

	// ������ ���� ������ ���̾� ������ ���� ������ �����մϴ�.
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
#pragma region ���� ó��
	if (_iLevelIndex >= m_iNumLevels ||
		_iLevelIndex < 0)
		return nullptr;
#pragma endregion ���� ó��

	return m_vecLevels[_iLevelIndex];
}

HRESULT CObjectManager::OpenLevel(shared_ptr<CLevel> _spNewLevel)
{
	_uint iPrevLevelIndex = m_iCurLevelIndex;

	// ���� ���� ���ҽ� ����
	if (0 != m_iCurLevelIndex) /* ����ƽ ������ �������� �ʽ��ϴ�. */
		// CResourceManager::GetInstance()->ClearCurrentScene(); -> Loader�� ����

	//// ���� ���� ������Ʈ ����
	//if (0 != m_iCurLevelIndex) /* ����ƽ ������ �������� �ʽ��ϴ�. */
	//	CObjectManager::GetInstance()->ClearCurrentScene(m_iCurLevelIndex);

	// ���� ���� ������ ������ ����(����ƽ ������ ���Ŀ� �߰��ؾ���)
	if (0 != m_iCurLevelIndex)
		CPrefabManager::GetInstance()->ClearCurrentScene(m_iCurLevelIndex);

	// ���� ���� ���� ������ ����(����ƽ ������ ���Ŀ� �߰��ؾ���)
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

	// ���� ������ ���ο� ������ ����
	m_spCurLevel = _spNewLevel;
	m_iCurLevelIndex = _spNewLevel->GetLevelIndex();

	m_vecLevels[m_iCurLevelIndex] = _spNewLevel;

	m_vecLevels[m_iCurLevelIndex]->Initialize(m_iNumLayers);

	// ���� ���� ������Ʈ ����
	if (0 != m_iCurLevelIndex) /* ����ƽ ������ �������� �ʽ��ϴ�. */
		CObjectManager::GetInstance()->ClearCurrentScene(iPrevLevelIndex);

	return S_OK;
}

void CObjectManager::ClearCurrentScene(_uint _iLevelIndex)
{
	if (m_vecLevels[_iLevelIndex] && _iLevelIndex != 0 /* ����ƽ ������ �������� �ʴ´�. */)
	{
		m_vecLevels[_iLevelIndex]->Release();
		m_vecLevels[_iLevelIndex].reset();
	}
}
