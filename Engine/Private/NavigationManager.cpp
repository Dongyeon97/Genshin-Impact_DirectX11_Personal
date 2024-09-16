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
	// 종료되기 전에 네비게이션을 파일로 저장합니다.

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
	// [ 현재 네비게이션 ]
	m_spCurNavigation;

	// [ 네비게이션 매니저가 보관하고 있는 네비게이션 컴포넌트 ]
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

	// 방금 추가한 네비게이션을 현재 네비게이션으로 설정
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

	// 1. 처음 찍히는 Cell이면 아무런 제약없이 찍을 수 있다.
	if (!vecCells.empty())
	{
		// 2. 두 번째 Cell부터는 매개변수로 들어온 벡터와, 현재 네비가 들고있는 정점들 간의 거리를 비교
		for (auto& Cell : vecCells)
		{
			for (_uint i = 0; i < CCell::POINT_END; ++i)
			{
				vExistingPoint = Cell->GetPoint(CCell::POINTS(i));

				_float fDistance = XMVector3Length(_vInputPoint - XMLoadFloat3(&vExistingPoint)).m128_f32[0];

				// 2. 일정 거리안에 정점이 인식됬으면 그 정점과 이 벡터를 일치시킨다.
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

	// 3. 인식된 정점이 없다면 E_FAIL 반환
	return E_FAIL;
}
