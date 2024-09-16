#include "Engine_pch.h"
#include "Timer.h"

HRESULT CTimer::ReadyTimer(void)
{
	QueryPerformanceCounter(&m_lFrameTime);
	QueryPerformanceCounter(&m_lFixTime);
	QueryPerformanceCounter(&m_lLastTime);
	QueryPerformanceFrequency(&m_lCpuTick);

	return S_OK;
}

_float CTimer::ComputeTimeDelta(void)
{
	QueryPerformanceCounter(&m_lFrameTime);

	if (m_lFrameTime.QuadPart - m_lFixTime.QuadPart >= m_lCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_lCpuTick);
		m_lFixTime = m_lFrameTime;
	}

	m_fTimeDelta = float(m_lFrameTime.QuadPart - m_lLastTime.QuadPart) / m_lCpuTick.QuadPart;

	m_lLastTime = m_lFrameTime;

	return m_fTimeDelta;
}