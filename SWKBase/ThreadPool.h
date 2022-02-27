#pragma once

#include <thread>
#include <vector>
#include <algorithm>    // std::for_each

namespace swktool {

	typedef std::thread thread_item;
	typedef std::vector<thread_item> ThreadStorage;
	typedef std::vector<thread_item>::iterator ThreadStorageItr;



	class ThreadPool {
		ThreadStorage ThreadList;
		unsigned int MaxThreads;

	public:
		ThreadPool(int nMaxThreads) noexcept {
			MaxThreads = nMaxThreads;
		}

		virtual ~ThreadPool() noexcept {
			;
		}

		void AddThread(thread_item Thread) noexcept;
		void JoinAll();


	protected:
		void JoinThreadFromHead();
	};


}