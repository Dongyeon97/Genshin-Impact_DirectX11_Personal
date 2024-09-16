#pragma once

/* 대부분의 기능 CObjectManager로 이동. 현재는 빈 깡통임 */

BEGIN(Engine)

class CLevel;

class ENGINE_DLL CLevelManager final
{
	DECLARE_SINGLETON(CLevelManager)

/********************
	Framework
********************/
public:
	HRESULT Initialize();
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void Render();
	void EndPlay();
	void Release();

/********************
	Methods
********************/
public:
	// 클라이언트를 실행한 뒤 처음 레벨을 시작할 때 호출합니다.
	// HRESULT OpenLevel(_uint _iLevelIndex, shared_ptr<CLevel> _spNewLevel);


/********************
	Getter/Setter
********************/
public:
	// 현재 레벨을 불러옵니다.
	// shared_ptr<CLevel> GetCurLevel() { assert(m_spCurLevel);  return m_spCurLevel; }

/********************
	Data Members
********************/
private:


	// 레벨매니저가 소유하고 있는 레벨들
	// std::map<_uint, std::shared_ptr<CLevel>> m_mapLevels;
};

END