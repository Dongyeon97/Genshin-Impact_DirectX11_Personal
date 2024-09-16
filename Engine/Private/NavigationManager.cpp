#include "Engine_pch.h"
#include "NavigationManager.h"

#include "Navigation.h"
#include "Cell.h"

IMPLEMENT_SINGLETON(CNavigationManager)

HRESULT CNavigationManager::Initialize()
{
	return S_OK;
}

void CNavigationManager::Release()
{
	// ����Ǳ� ���� �׺���̼��� ���Ϸ� �����մϴ�.

	for (auto& pair : m_mapNavigations)
	{
		if (nullptr != pair.second)
		{
			pair.second->Release();
			pair.second.reset();
		}
	}
	m_mapNavigations.clear();

	m_upInstance.reset();
}

void CNavigationManager::ClearCurrentScene()
{
	// [ ���� �׺���̼� ]
	m_spCurNavigation;

	// [ �׺���̼� �Ŵ����� �����ϰ� �ִ� �׺���̼� ������Ʈ ]
	m_mapNavigations;

	if (nullptr != m_spCurNavigation)
	{
		m_spCurNavigation->Release();
		m_spCurNavigation.reset();
	}

	for (auto& Pair : m_mapNavigations)
	{
		if (nullptr != Pair.second)
		{
			Pair.second->Release();
			Pair.second.reset();
		}
	}
	m_mapNavigations.clear();
}

HRESULT CNavigationManager::AddNavigation(const wstring& _wstrNaviTag, shared_ptr<CNavigation> _spNavigation)
{
	if (m_mapNavigations.end() != m_mapNavigations.find(_wstrNaviTag))
		return E_FAIL;

	m_mapNavigations.emplace(_wstrNaviTag, _spNavigation);

	// ��� �߰��� �׺���̼��� ���� �׺���̼����� ����
	m_spCurNavigation = _spNavigation;

	return S_OK;
}

shared_ptr<CNavigation> CNavigationManager::FindNavigation(const wstring& _wstrNaviTag)
{
	auto iter = m_mapNavigations.find(_wstrNaviTag);

	if (m_mapNavigations.end() == iter)
		return nullptr;

	return iter->second;
}

void CNavigationManager::RemoveNavigation(const wstring& _wstrNaviTag)
{
	auto iter = m_mapNavigations.find(_wstrNaviTag);

	if (m_mapNavigations.end() != iter)
	{
		(*iter).second.reset();
		m_mapNavigations.erase(iter);
	}
}

HRESULT CNavigationManager::AddNaviCell(const _float3* _vPoints)
{
	if (nullptr != m_spCurNavigation)
		return E_FAIL;

	m_spCurNavigation->AddCell(_vPoints);

	return S_OK;
}

HRESULT CNavigationManager::RemoveLatestCell()
{
	if (nullptr == m_spCurNavigation)
		return E_FAIL;

	m_spCurNavigation->RemoveLatestCell();

	return S_OK;
}

HRESULT CNavigationManager::CorrectingPoints(_vector& _vInputPoint)
{
	_float3 vExistingPoint;

	vector<shared_ptr<CCell>>& vecCells = m_spCurNavigation->GetVecCells();

	// 1. ó�� ������ Cell�̸� �ƹ��� ������� ���� �� �ִ�.
	if (!vecCells.empty())
	{
		// 2. �� ��° Cell���ʹ� �Ű������� ���� ���Ϳ�, ���� �׺� ����ִ� ������ ���� �Ÿ��� ��
		for (auto& Cell : vecCells)
		{
			for (_uint i = 0; i < CCell::POINT_END; ++i)
			{
				vExistingPoint = Cell->GetPoint(CCell::POINTS(i));

				_float fDistance = XMVector3Length(_vInputPoint - XMLoadFloat3(&vExistingPoint)).m128_f32[0];

				// 2. ���� �Ÿ��ȿ� ������ �νĉ����� �� ������ �� ���͸� ��ġ��Ų��.
				if (50.0f > fDistance)
				{
					_vInputPoint = XMLoadFloat3(&vExistingPoint);
					return S_OK;
				}
			}
		}
	}
	else
		return S_OK;

	// 3. �νĵ� ������ ���ٸ� E_FAIL ��ȯ
	return E_FAIL;
}
