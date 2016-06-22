#pragma once

#include "PreemptiveEventPoolFwd.h"
#include "IEvent.h"

#include <type_traits>

namespace foundation
{
	//************************************
	/*
	带事件队列的线程池对象
	*/
	//************************************
	class PreemptiveEventPool
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;
		
	public:

		//************************************
		/*
		自定义事件派发器

		事件队列内部派发流程：

		DispatchEvent(event)
		{
			if (Dispatcher_)
			{
				Dispatcher_->DispatchEvent(event);
			}
			else
			{
				DispatchEventByName(event); //Default
			}
		}
		*/
		//************************************
		class Dispatcher
		{
		public:

			virtual ~Dispatcher()
			{}

			virtual	void	DispatchEvent(const IEventSPtr& event) = 0;
		};
		typedef	std::shared_ptr<Dispatcher>	DispatcherSPtr;

	public:
	
		PreemptiveEventPool();
		
		~PreemptiveEventPool();

	public:

		//************************************
		// Method:  foundation::EventThreadPool::StartAsync
		// Access:    public 
		// Returns:   bool
		// Parameter: uint32_t threadSize	线程数量
		/*
			异步启动线程池
		*/
		//************************************
		bool	StartAsync(uint32_t threadSize);

		//************************************
		// Method:  foundation::EventThreadPool::JoinAllThreads
		// Access:    public 
		// Returns:   void
		/*
			等待所有线程结束，在Stop()后调用
		*/
		//************************************
		void	JoinAllThreads();

		//************************************
		// Method:  foundation::EventThreadPool::Stop
		// Access:    public 
		// Returns:   void
		/*
			通知所有线程停止工作，确保所有线程已结束，需调用JoinAllThreads()
		*/
		//************************************
		void	Stop();

		//************************************
		// Method:  foundation::EventThreadPool::IsStop
		// Access:    public 
		// Returns:   bool
		/*
			是否在工作状态
		*/
		//************************************
		bool	IsStop() const;

	public:

		//************************************
		// Method:  foundation::EventThreadPool::PostEvent
		// Access:    public 
		// Returns:   void
		// Parameter: const IEventSPtr & evt
		/*
			推送事件
		*/
		//************************************
		void	PostEvent(const IEventSPtr& evt);

		template<typename T>
		void	RegisterEventCallBack(const EventSlot& slot)
		{
			static_assert(std::is_base_of<IEvent, T>::value, "Error");

			RegisterEventCallBack(typeid(T).name(), slot);
		}

		void	RegisterEventCallBack(const char* eventName, const EventSlot& slot);

		void	SetDispatcher(const DispatcherSPtr& dispatcher);
	};
}