#pragma once

#include "AsyncThreadPoolFwd.h"
#include "PostTask.h"

#include <stdint.h>

namespace foundation
{
	//************************************
	/*
		线程池对象
	*/
	//************************************
	class AsyncThreadPool
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;

	public:

		AsyncThreadPool();

		~AsyncThreadPool();

	public:

		//************************************
		// Method:  foundation::AsyncThreadPool::StartAsync
		// Access:    public 
		// Returns:   bool
		// Parameter: uint32_t threadsNumber	线程数量
		/*
			异步启动线程池
		*/
		//************************************
		bool	StartAsync(uint32_t threadsNumber);

		//************************************
		// Method:  foundation::AsyncThreadPool::JoinAllThreads
		// Access:    public 
		// Returns:   void
		/*
			等待所有线程结束，在Stop()后调用
		*/
		//************************************
		void	JoinAllThreads();

		//************************************
		// Method:  foundation::AsyncThreadPool::Stop
		// Access:    public 
		// Returns:   void
		/*
			通知所有线程停止工作，确保所有线程已结束，需调用JoinAllThreads()
		*/
		//************************************
		void	Stop();

		//************************************
		// Method:  foundation::AsyncThreadPool::IsStop
		// Access:    public 
		// Returns:   bool
		/*
			是否在工作状态
		*/
		//************************************
		bool	IsStop() const;

		//************************************
		// Method:  foundation::AsyncThreadPool::Post
		// Access:    public 
		// Returns:   void
		// Parameter: const PostTask & task
		/*
			将task任务加入工作队列执行
		*/
		//************************************
		void	Post(const PostTask& task);

		//************************************
		// Method:  foundation::AsyncThreadPool::GetThreadsNumber
		// Access:    public 
		// Returns:   uint32_t
		/*
			获取当前线程池中的工作线程数量
		*/
		//************************************
		uint32_t	GetThreadsNumber() const;
	};
}