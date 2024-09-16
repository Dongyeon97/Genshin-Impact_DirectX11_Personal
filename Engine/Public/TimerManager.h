#pragma once

BEGIN(Engine)

class CTimer;

class ENGINE_DLL CTimerManager final
{
	DECLARE_SINGLETON(CTimerManager)

public:
	_float ComputeTimeDelta(const wstring& _wstrTimerTag);
	HRESULT AddTimer(const wstring& _wstrTimerTag);
	void Release();

private:
	shared_ptr<CTimer> FindTimer(const wstring& _wstrTimerTag);

private:
	map<const wstring, shared_ptr<CTimer>> m_mapTimers;
};

END