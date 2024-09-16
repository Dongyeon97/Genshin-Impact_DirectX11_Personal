#pragma once

class CLoader final : public std::enable_shared_from_this<CLoader>
{
public:
	explicit CLoader() = default;
	~CLoader() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(LEVEL _eNextLevelIndex);
	void	Release();

/********************
	Methods
********************/
public:
	HRESULT Loading_Selector();
	_bool isFinished() const {
		return m_isFinished;
	}

public:
	void CreateLevel();

#ifdef _DEBUG
public:
	void Output_LoadingText();
#endif

private:
	HRESULT Loading_For_Logo();
	HRESULT Loading_For_Stage1();
	HRESULT Loading_For_Stage2();
	HRESULT Loading_For_Stage3();

/********************
	Data Members
********************/
private:
	wstring				m_strLoadingText = {};

	_bool				m_isFinished = { false };

private:
	HANDLE				m_hThread = { 0 };

	CRITICAL_SECTION	m_CriticalSection = {};

	LEVEL				m_eNextLevelIndex = Client::LEVEL_END;
};