#include "pch.h"
#include "ThreadPool.h"

namespace swktool {
	void ThreadPool::AddThread(thread_item Thread) noexcept {
		if (ThreadList.size() < MaxThreads) {
			ThreadList.push_back(std::move(Thread));
		}
		else {

			JoinThreadFromHead();

			// add the new thread into store
			ThreadList.push_back(std::move(Thread));
		}
	}

	void ThreadPool::JoinAll()
	{
		// join all threads
		for (auto itr = ThreadList.begin(); itr != ThreadList.end(); ++itr) {
			thread_item Item = std::move(*itr);

			// join the thread
			if (Item.joinable()) {
				Item.join();
			}
		}

		// remove all out of the list
		ThreadList.erase(ThreadList.begin(), ThreadList.end());
	}


	void ThreadPool::JoinThreadFromHead()
	{
		ThreadStorageItr FirstThreadItr = ThreadList.begin();
		thread_item Item = std::move(*FirstThreadItr);

		// join the thread
		if (Item.joinable()) {
			Item.join();
		}

		// remove the joined thread out of the list
		ThreadList.erase(FirstThreadItr);
	}
}


