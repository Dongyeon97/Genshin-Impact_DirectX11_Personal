#pragma once

/* NavigationManager */
// 1. ���� ���������� �׺���̼� ������Ʈ�� ����. -> Terrain���κ��� �޾ƿ�
// 2. Actor�� SetTranslation �Լ� ȣ�⿡ 1.������ ������ �׺���̼��� Ȱ���ϵ��� ��.

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
	// [ �׺���̼� �߰� ]
	HRESULT AddNavigation(const wstring& _wstrNaviTag, shared_ptr<CNavigation> _wpNavigation);

	// [ �׺���̼� �˻� ]
	shared_ptr<CNavigation> FindNavigation(const wstring& _wstrNaviTag);

	// [ �׺���̼� ���� ]
	void RemoveNavigation(const wstring& _wstrNaviTag);

	// [ ���� �׺���̼ǿ� �߰� ]
	HRESULT AddNaviCell(const _float3* _vPoints);

	// [ �������� �߰��� ������ ���� ]
	HRESULT RemoveLatestCell();

	// [ �Ű������� ���� ������ ���� �׺���̼��� ������ ��ġ�ǵ��� ���� ]
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
	// [ ���� �׺���̼� ]
	shared_ptr<CNavigation> m_spCurNavigation;

	// [ �׺���̼� �Ŵ����� �����ϰ� �ִ� �׺���̼� ������Ʈ ]
	map<wstring, shared_ptr<CNavigation>> m_mapNavigations;
};

END