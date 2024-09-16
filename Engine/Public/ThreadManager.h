#pragma once

BEGIN(Engine)

class ENGINE_DLL CThreadManager final
{
/********************
	Ctor/Dtor
********************/
public:
	DECLARE_SINGLETON(CThreadManager)

/********************
	Events
********************/
public:
	HRESULT Initialize();

	_uint Tick();

	void Release();
	
/********************
	Methods
********************/
public:
	// [ 스레드 풀 가동 ]
	void ProgressThreadPool();

	// [ 스레드 풀 종료 ]
	void FinishThreadPool();

	// [ 워커 스레드 ]
	void ProgressWorkerThread();

	// [ 작업 추가 ]
	void EnqueueJob(function<void()> _job);

/********************
	Getter/Setter
********************/
public:
	const shared_ptr<mutex> GetMutex()
	{ 
		if (nullptr == m_spMutex)
			return nullptr;

	return m_spMutex; 
	}

/******************
	Data Members
******************/
private:
	// [ 뮤텍스 ]
	shared_ptr<mutex> m_spMutex = nullptr;

	// [ ]
	condition_variable m_cvJobs;

	// [ 스레드 개수 ]
	_uint m_iNumThreads = 0;

	// [ 스레드 ]
	vector<thread> m_vecWorkerThreads;

	// [ 스레드가 할 일들을 보관하는 작업 queue ]
	queue<function<void()>> m_qJobs;

	// [ 모든 스레드 종료 ]
	bool m_bStopAll = false;
};

END