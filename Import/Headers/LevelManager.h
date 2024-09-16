#pragma once

/* ��κ��� ��� CObjectManager�� �̵�. ����� �� ������ */

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
	// Ŭ���̾�Ʈ�� ������ �� ó�� ������ ������ �� ȣ���մϴ�.
	// HRESULT OpenLevel(_uint _iLevelIndex, shared_ptr<CLevel> _spNewLevel);


/********************
	Getter/Setter
********************/
public:
	// ���� ������ �ҷ��ɴϴ�.
	// shared_ptr<CLevel> GetCurLevel() { assert(m_spCurLevel);  return m_spCurLevel; }

/********************
	Data Members
********************/
private:


	// �����Ŵ����� �����ϰ� �ִ� ������
	// std::map<_uint, std::shared_ptr<CLevel>> m_mapLevels;
};

END