#pragma once

BEGIN(Engine)

class CTimer final : public enable_shared_from_this<CTimer>
{
public:
	CTimer() = default;
	virtual ~CTimer() = default;

public:
	HRESULT	ReadyTimer();
	_float	ComputeTimeDelta();

private:
	LARGE_INTEGER			m_lFrameTime;
	LARGE_INTEGER			m_lFixTime;
	LARGE_INTEGER			m_lLastTime;
	LARGE_INTEGER			m_lCpuTick;

	_float					m_fTimeDelta = 0.0f;
};

END