#include "Engine_pch.h"
#include "LevelManager.h"

#include "ResourceManager.h"
#include "ObjectManager.h"

#include "Level.h"

IMPLEMENT_SINGLETON(CLevelManager)

HRESULT CLevelManager::Initialize()
{

	return S_OK;
}

_int CLevelManager::PreUpdate(const _float& _fTimeDelta)
{
	// if (m_spCurLevel)
		// return m_spCurLevel->PreUpdate(_fTimeDelta);

	return S_OK;
}

_int CLevelManager::Update(const _float& _fTimeDelta)
{
	// if(m_spCurLevel)
		// return m_spCurLevel->Update(_fTimeDelta);

	return S_OK;
}

_int CLevelManager::LateUpdate(const _float& _fTimeDelta)
{
	// if(m_spCurLevel)
		// return m_spCurLevel->LateUpdate(_fTimeDelta);

	return S_OK;
}

void CLevelManager::Render()
{
	// if(m_spCurLevel)
		// m_spCurLevel->Render();
}

void CLevelManager::EndPlay()
{
	// if (m_spCurLevel)
		// m_spCurLevel->EndPlay();
}

void CLevelManager::Release()
{
	// if (m_spCurLevel)
	// {
		// m_spCurLevel->Release();
		// m_spCurLevel.reset();
	// }

	// m_upInstance.reset();
}

// HRESULT CLevelManager::OpenLevel(_uint _iLevelIndex, shared_ptr<CLevel> _spNewLevel)
// {
	// ���� ���� �ڿ�(������Ʈ, ������Ʈ) ����
	// if (nullptr != m_spCurLevel)
		// m_spCurLevel->Release();

	// ���� ���� ���ҽ� ����
	// if(0 != m_iCurLevelIndex) /* ����ƽ ������ �������� �ʽ��ϴ�. */
		// CResourceManager::GetInstance()->ClearCurrentScene();

	// ���� ���� ������Ʈ ����
	// if (0 != m_iCurLevelIndex) /* ����ƽ ������ �������� �ʽ��ϴ�. */
		// CObjectManager::GetInstance()->ClearCurrentScene(m_iCurLevelIndex);

	// ���� ������ ���ο� ������ ����
	// m_spCurLevel = _spNewLevel;
	// m_iCurLevelIndex = _spNewLevel->GetLevelIndex();

	// CObjectManager::GetInstance()->SetCurLevel(m_iCurLevelIndex, _spNewLevel);

	// return S_OK;
// }