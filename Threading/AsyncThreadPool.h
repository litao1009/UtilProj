#pragma once

#include "AsyncThreadPoolFwd.h"
#include "PostTask.h"

#include <stdint.h>

namespace foundation
{
	//************************************
	/*
		�̳߳ض���
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
		// Parameter: uint32_t threadsNumber	�߳�����
		/*
			�첽�����̳߳�
		*/
		//************************************
		bool	StartAsync(uint32_t threadsNumber);

		//************************************
		// Method:  foundation::AsyncThreadPool::JoinAllThreads
		// Access:    public 
		// Returns:   void
		/*
			�ȴ������߳̽�������Stop()�����
		*/
		//************************************
		void	JoinAllThreads();

		//************************************
		// Method:  foundation::AsyncThreadPool::Stop
		// Access:    public 
		// Returns:   void
		/*
			֪ͨ�����߳�ֹͣ������ȷ�������߳��ѽ����������JoinAllThreads()
		*/
		//************************************
		void	Stop();

		//************************************
		// Method:  foundation::AsyncThreadPool::IsStop
		// Access:    public 
		// Returns:   bool
		/*
			�Ƿ��ڹ���״̬
		*/
		//************************************
		bool	IsStop() const;

		//************************************
		// Method:  foundation::AsyncThreadPool::Post
		// Access:    public 
		// Returns:   void
		// Parameter: const PostTask & task
		/*
			��task������빤������ִ��
		*/
		//************************************
		void	Post(const PostTask& task);

		//************************************
		// Method:  foundation::AsyncThreadPool::GetThreadsNumber
		// Access:    public 
		// Returns:   uint32_t
		/*
			��ȡ��ǰ�̳߳��еĹ����߳�����
		*/
		//************************************
		uint32_t	GetThreadsNumber() const;
	};
}