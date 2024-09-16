#include "Engine_pch.h"
#include "ThreadManager.h"

IMPLEMENT_SINGLETON(CThreadManager)

using namespace std;

HRESULT Engine::CThreadManager::Initialize()
{
	// ���ؽ�
	m_spMutex = make_shared<mutex>();

	// ������
	m_iNumThreads = 8;
	m_vecWorkerThreads.reserve(m_iNumThreads);

	// ��ũ ������ �غ�
	for (_uint i = 0; i < m_iNumThreads; ++i)
	{
		// WorkThread�� �����Ͽ�, ���Ϳ� �����մϴ�.
		m_vecWorkerThreads.emplace_back([this]() {
			this->ProgressWorkerThread();
			});
	}

	return S_OK;
}

_uint Engine::CThreadManager::Tick()
{
	// ������ Ǯ ����
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

		// ť�� �۾��� ����ְų�, m_bStopAll�� true�� �� ������ ��ٸ��ϴ�.
		m_cvJobs.wait(ulMutex, [this]() {
			return !this->m_qJobs.empty() || m_bStopAll;
			});

		// m_bStopAll�� true�ų� ť�� �۾��� ���ٸ�, return�մϴ�.
		if (m_bStopAll && this->m_qJobs.empty())
		{
			return;
		}

		// ť�� �۾��� ����ִ� ���, ���� �������� �߰��� �۾��� front�� ���ɴϴ�.
		function<void()> fnJob = move(m_qJobs.front());

		// �ش� �۾��� ť���� �����մϴ�.
		m_qJobs.pop();
		ulMutex.unlock();

		// �ش� �۾��� �����մϴ�.
		CoInitialize(nullptr);
		fnJob();
	}
}

void Engine::CThreadManager::EnqueueJob(std::function<void()> _job)
{
	// m_bStopAll�� true�� ��� ���̻� �۾��� �߰��Ǹ� �ȵǱ� ������, ���ܸ� �����ϴ�.
	if (m_bStopAll)
	{
		throw runtime_error("ThreadPool ��� ����");
	}

	// lock_guard�� �Ӱ迵���� �����մϴ�.
	lock_guard<mutex> Mutex(*m_spMutex.get());

	// �۾��� ť�� �߰��մϴ�.
	m_qJobs.push(move(_job));

	// �ڰ��ִ� ������ �ϳ��� ����ϴ�?
	m_cvJobs.notify_one();
}

void Engine::CThreadManager::FinishThreadPool()
{
	// ������ Ǯ�� ������ m_bStopAll�� true�� �ٲ��ݴϴ�.
	m_bStopAll = true;

	// ��� ������ �鿡�� �۾��� �������� �˸��ϴ�.
	m_cvJobs.notify_all();

	// ��� �����带 join���ݴϴ�.
	// join : �ش� �������� �۾��� ������ ��� ȣ���� �˴ϴ�.
	for (auto& thread : m_vecWorkerThreads)
	{
		thread.join();
	}
}

void Engine::CThreadManager::Release()
{
	// ������ Ǯ ����
	FinishThreadPool();

	// �̱��� �Ŵ����� �޸𸮿��� �����մϴ�.
	m_upInstance.reset();
	m_upInstance = nullptr;
}