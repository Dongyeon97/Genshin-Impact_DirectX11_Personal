#include "Engine_pch.h"
#include "Layer.h"

#include "DeviceManager.h"

#include "Level.h"
#include "Actor.h"

void CLayer::Initialize()
{
	// ��� ���̾�� Initialize �ܰ迡�� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// ���͸� �ʱ�ȭ�մϴ�.
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			spActor->Initialize();
		}
	}

	for (auto& pair : m_umapActors)
	{
		pair.second->Initialize();
	}
}

_int CLayer::PreUpdate(const _float& _fTimeDelta)
{
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (!spActor->GetActorUse())
				continue;

			_int iResult = spActor->PreUpdate(_fTimeDelta);
			if (iResult != S_OK) 
			{
				switch (iResult)
				{
				case -1 :
				{
					// ����
					break;
				}
				case 1:
				{
					// ��Ȱ��ȭ
					spActor->SetActorUse(false);
					break;
				}
				}
			}
		}
	}

	for (auto& pair : m_umapActors)
	{
		if (!pair.second->GetActorUse())
			continue;

		_int iResult = pair.second->PreUpdate(_fTimeDelta);
		if (iResult != S_OK) { return iResult; }
	}

	return S_OK;
}

_int CLayer::Update(const _float& _fTimeDelta)
{
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (!spActor->GetActorUse())
				continue;

			_int iResult = spActor->Update(_fTimeDelta);
			if (iResult != S_OK)
			{
				switch (iResult)
				{
				case -1:
				{
					// ����
					break;
				}
				case 1:
				{
					// ��Ȱ��ȭ
					spActor->SetActorUse(false);
					break;
				}
				}
			}
		}
	}

	for (auto& pair : m_umapActors)
	{
		if (!pair.second->GetActorUse())
			continue;

		_int iResult = pair.second->Update(_fTimeDelta);
		if (iResult != S_OK) { return iResult; }
	}
	return S_OK;
}

_int CLayer::LateUpdate(const _float& _fTimeDelta)
{
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (!spActor->GetActorUse())
				continue;

			_int iResult = spActor->LateUpdate(_fTimeDelta);
			if (iResult != S_OK)
			{
				switch (iResult)
				{
				case -1:
				{
					// ����
					break;
				}
				case 1:
				{
					// ��Ȱ��ȭ
					spActor->SetActorUse(false);
					break;
				}
				}
			}
		}
	}

	for (auto& pair : m_umapActors)
	{
		if (!pair.second->GetActorUse())
			continue;

		_int iResult = pair.second->LateUpdate(_fTimeDelta);
		if (iResult != S_OK)
		{
			RemoveActor(pair.second->GetActorName());
			return iResult;
		}
	}
	return S_OK;
}

void CLayer::Render()
{
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (!spActor->GetActorUse())
				continue;

			spActor->Render();
		}
	}

	for (auto& pair : m_umapActors)
	{
		if (!pair.second->GetActorUse())
			continue;

		pair.second->Render();
	}
}

void CLayer::EndPlay()
{
	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (!spActor->GetActorUse())
				continue;

			spActor->EndPlay();
		}
	}

	for (auto& pair : m_umapActors)
	{
		if (!pair.second->GetActorUse())
			continue;

		pair.second->EndPlay();
	}
}

void CLayer::Release()
{
	for (auto& pair : m_umapActors)
	{
		if (nullptr != pair.second)
		{
			pair.second->Release();
			pair.second.reset();
		}
	}
	m_umapActors.clear();

	for (auto& pair : m_umapObjectPool)
	{
		for (auto& spActor : pair.second)
		{
			if (nullptr != spActor)
			{
				spActor->Release();
				spActor.reset();
			}
		}
	}
	m_umapObjectPool.clear();

	m_umapActors.clear();
}

shared_ptr<CActor> CLayer::RecycleFromPool(const wstring& _wstrActorVariation)
{
	auto pair = m_umapObjectPool.find(_wstrActorVariation);

	if(pair == m_umapObjectPool.end()) { return nullptr; }

	// ������Ʈ Ǯ�� ��ȸ�ϸ�, �̸��� ���� �� �߿��� ���� ���� ã������ �ָ� Ȱ��ȭ ��Ű��, ����������.
	for (auto& spActor : pair->second)
	{
		if (!spActor->GetActorUse())
		{
			spActor->SetActorUse(true);
			spActor->RecycleBegin();
			return spActor;
		}
	}

	return nullptr;
}

shared_ptr<CActor> CLayer::FindActor(const wstring& _wstrActor)
{
	if (m_umapActors.find(_wstrActor) == m_umapActors.end())
		return nullptr;

	return m_umapActors[_wstrActor];
}

shared_ptr<CActor> CLayer::FindActor_Pool(const wstring& _wstrActorVariation, const wstring& _wstrActorName)
{
	auto iter = m_umapObjectPool.find(_wstrActorVariation);

	if (iter == m_umapObjectPool.end())
		return nullptr;

	for (auto& spActor : m_umapObjectPool[_wstrActorVariation])
	{
		if (_wstrActorName == spActor->GetActorName())
		{
			return spActor;
		}
	}

	return nullptr;
}

void CLayer::RemoveActor(const wstring& _wstrActor)
{
	if (m_umapActors.find(_wstrActor) == m_umapActors.end()) { return; }

	m_umapActors[_wstrActor]->Release();
	m_umapActors[_wstrActor].reset();
	m_umapActors.erase(_wstrActor);
}

void CLayer::RemoveActor_Pool(const wstring& _wstrActor)
{
	wstring wstrActorVariation = L"";

	for (_uint i = 0; i < _wstrActor.size(); ++i)
	{
		if (isdigit(_wstrActor[i])) break;
		wstrActorVariation += _wstrActor[i];
	}

	if (m_umapObjectPool.find(wstrActorVariation) == m_umapObjectPool.end()) 
		return;

	auto iter = m_umapObjectPool[wstrActorVariation].begin();

	for (iter; iter != m_umapObjectPool[wstrActorVariation].end(); ++iter)
	{
		if (_wstrActor == (*iter)->GetActorName())
		{
			(*iter)->Release();
			(*iter).reset();
			m_umapObjectPool[wstrActorVariation].erase(iter);

			if (m_umapObjectPool[wstrActorVariation].empty())
			{
				m_umapObjectPool[wstrActorVariation].clear();
				m_umapObjectPool.erase(wstrActorVariation);
			}

			return;
		}
	}
}
