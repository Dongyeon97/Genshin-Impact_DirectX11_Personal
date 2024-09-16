#include "Engine_pch.h"
#include "ThreadManager.h"

IMPLEMENT_SINGLETON(CThreadManager)

using namespace std;

HRESULT Engine::CThreadManager::Initialize()
{
	// 뮤텍스
	m_spMutex = make_shared<mutex>();

	// 스레드
	m_iNumThreads = 8;
	m_vecWorkerThreads.reserve(m_iNumThreads);

	// 워크 스레드 준비
	for (_uint i = 0; i < m_iNumThreads; ++i)
	{
		// WorkThread를 생성하여, 벡터에 보관합니다.
		m_vecWorkerThreads.emplace_back([this]() {
			this->ProgressWorkerThread();
			});
	}

	return S_OK;
}

_uint Engine::CThreadManager::Tick()
{
	// 스레드 풀 가동
	// ProgressThreadPool();

	return 0;
}

void Engine::CThreadManager::ProgressThreadPool()
{
	for (auto& thread : m_vecWorkerThreads)
	{
		thread.join();
	}
}

void Engine::CThreadManager::ProgressWorkerThread()
{
	while (true)
	{
		unique_lock<mutex> ulMutex(*m_spMutex.get());

		// 큐에 작업이 들어있거나, m_bStopAll이 true가 될 때까지 기다립니다.
		m_cvJobs.wait(ulMutex, [this]() {
			return !this->m_qJobs.empty() || m_bStopAll;
			});

		// m_bStopAll이 true거나 큐에 작업이 없다면, return합니다.
		if (m_bStopAll && this->m_qJobs.empty())
		{
			return;
		}

		// 큐에 작업이 들어있는 경우, 가장 오래전에 추가된 작업을 front로 얻어옵니다.
		function<void()> fnJob = move(m_qJobs.front());

		// 해당 작업을 큐에서 제거합니다.
		m_qJobs.pop();
		ulMutex.unlock();

		// 해당 작업을 수행합니다.
		CoInitialize(nullptr);
		fnJob();
	}
}

void Engine::CThreadManager::EnqueueJob(std::function<void()> _job)
{
	// m_bStopAll이 true인 경우 더이상 작업이 추가되면 안되기 때문에, 예외를 던집니다.
	if (m_bStopAll)
	{
		throw runtime_error("ThreadPool 사용 중지");
	}

	// lock_guard로 임계영역을 설정합니다.
	lock_guard<mutex> Mutex(*m_spMutex.get());

	// 작업을 큐에 추가합니다.
	m_qJobs.push(move(_job));

	// 자고있는 쓰레드 하나를 깨웁니다?
	m_cvJobs.notify_one();
}

void Engine::CThreadManager::FinishThreadPool()
{
	// 스레드 풀이 끝나면 m_bStopAll을 true로 바꿔줍니다.
	m_bStopAll = true;

	// 모든 스레드 들에게 작업이 끝났음을 알립니다.
	m_cvJobs.notify_all();

	// 모든 스레드를 join해줍니다.
	// join : 해당 스레드의 작업이 끝났을 경우 호출이 됩니다.
	for (auto& thread : m_vecWorkerThreads)
	{
		thread.join();
	}
}

void Engine::CThreadManager::Release()
{
	// 스레드 풀 종료
	FinishThreadPool();

	// 싱글턴 매니저를 메모리에서 해제합니다.
	m_upInstance.reset();
	m_upInstance = nullptr;
}