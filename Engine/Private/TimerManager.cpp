#include "Engine_pch.h"
#include "TimerManager.h"

#include "Timer.h"

IMPLEMENT_SINGLETON(CTimerManager)

_float CTimerManager::ComputeTimeDelta(const wstring& _wstrTimerTag)
{
	shared_ptr<CTimer> spTimer = FindTimer(_wstrTimerTag);
	if (nullptr == spTimer)
		return 0.0f;

	return spTimer->ComputeTimeDelta();
}

HRESULT CTimerManager::AddTimer(const wstring& _wstrTimerTag)
{
	shared_ptr<CTimer> spTimer = FindTimer(_wstrTimerTag);

	if (nullptr != spTimer)
		return E_FAIL;

	spTimer = make_shared<CTimer>();

	if (FAILED(spTimer->ReadyTimer()))
		return E_FAIL;

	m_mapTimers.insert({ _wstrTimerTag, spTimer });

	return S_OK;
}

void CTimerManager::Release()
{
	for (auto& pair : m_mapTimers) { pair.second = nullptr; }
	m_mapTimers.clear();

	// 타임 매니저 파괴
	m_upInstance.reset();
}

shared_ptr<CTimer> CTimerManager::FindTimer(const wstring& _wstrTimerTag)
{
	auto iter = m_mapTimers.find(_wstrTimerTag);

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}
