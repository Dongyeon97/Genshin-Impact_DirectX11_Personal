#pragma once

/* NavigationManager */
// 1. 현재 스테이지의 네비게이션 컴포넌트를 저장. -> Terrain으로부터 받아옴
// 2. Actor의 SetTranslation 함수 호출에 1.번에서 저장한 네비게이션을 활용하도록 함.

BEGIN(Engine)

class CNavigation;

class ENGINE_DLL CNavigationManager final
{
	DECLARE_SINGLETON(CNavigationManager)
/********************
	Framework
********************/
public:
	HRESULT Initialize();
	void Release();

/********************
	Methods
********************/
public:
	void ClearCurrentScene();

public:
	// [ 네비게이션 추가 ]
	HRESULT AddNavigation(const wstring& _wstrNaviTag, shared_ptr<CNavigation> _wpNavigation);

	// [ 네비게이션 검색 ]
	shared_ptr<CNavigation> FindNavigation(const wstring& _wstrNaviTag);

	// [ 네비게이션 삭제 ]
	void RemoveNavigation(const wstring& _wstrNaviTag);

	// [ 셀을 네비게이션에 추가 ]
	HRESULT AddNaviCell(const _float3* _vPoints);

	// [ 마지막에 추가된 셀부터 삭제 ]
	HRESULT RemoveLatestCell();

	// [ 매개변수로 받은 정점을 현재 네비게이션의 정점과 일치되도록 보정 ]
	HRESULT CorrectingPoints(_vector& _vInputPoint);

/********************
	Getter / Setter
********************/
public:
	shared_ptr<CNavigation> GetCurNavigation()
	{ 
		if (nullptr != m_spCurNavigation)
			return m_spCurNavigation;
		return nullptr;
	}

	void SetCurNavigation(shared_ptr<CNavigation> _spCurNavi) { m_spCurNavigation = _spCurNavi; }

/********************
	Data Members
********************/
private:
	// [ 현재 네비게이션 ]
	shared_ptr<CNavigation> m_spCurNavigation;

	// [ 네비게이션 매니저가 보관하고 있는 네비게이션 컴포넌트 ]
	map<wstring, shared_ptr<CNavigation>> m_mapNavigations;
};

END