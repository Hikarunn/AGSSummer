#include "ThreadPool.h"
#include "Worker.h"
#include <deque>

#include "Debug.h"

ThreadPool::ThreadPool(unsigned int threadNum)
{
	workers_.resize(threadNum);
	for (auto& w : workers_)
	{
		w = std::make_unique<Worker>(*this);
	}
}

ThreadPool::~ThreadPool()
{
	Join();
}

// 非同期で実行したい関数と終了時のコールバック関数を追加する
void ThreadPool::Add(std::function<void(void)>&& func)
{
	// ロックする
	std::lock_guard<std::mutex> lock(mutex_);

	// リストに追加
	taskList_.push_back(std::move(func));

	for (auto& w : workers_)
	{
		if (w->isTaskEnd())
		{
			w->SetTask(std::move(*taskList_.begin()));
			taskList_.pop_front();
			return;
		}
	}

}

void ThreadPool::Join(void)
{
	for (auto& w : workers_)
	{
		w->Join();
	}
}


// 処理中ののみ待つ
void ThreadPool::Wait(void)
{
	for (auto& w : workers_)
	{
		w->Wait();
	}
}


void ThreadPool::WaitAllTask(void)
{
	while (true)
	{
		if (taskList_.empty())
		{
			break;
		}
		std::this_thread::yield();
	}
	Wait();
}

bool ThreadPool::GetTask(Worker& worker)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (taskList_.empty())
	{
		return false;
	}
	worker.SetTask(std::move(*taskList_.begin()));
	taskList_.pop_front();
	DebugLog("ゲット");
	return true;
}

