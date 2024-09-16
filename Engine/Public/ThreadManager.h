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
	// [ ������ Ǯ ���� ]
	void ProgressThreadPool();

	// [ ������ Ǯ ���� ]
	void FinishThreadPool();

	// [ ��Ŀ ������ ]
	void ProgressWorkerThread();

	// [ �۾� �߰� ]
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
	// [ ���ؽ� ]
	shared_ptr<mutex> m_spMutex = nullptr;

	// [ ]
	condition_variable m_cvJobs;

	// [ ������ ���� ]
	_uint m_iNumThreads = 0;

	// [ ������ ]
	vector<thread> m_vecWorkerThreads;

	// [ �����尡 �� �ϵ��� �����ϴ� �۾� queue ]
	queue<function<void()>> m_qJobs;

	// [ ��� ������ ���� ]
	bool m_bStopAll = false;
};

END