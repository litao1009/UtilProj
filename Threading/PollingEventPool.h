#pragma once

#include "PollingEventPoolFwd.h"
#include "IEvent.h"
#include "PostTask.h"

#include <type_traits>

namespace foundation
{
	//************************************
	/*
	带事件队列的线程池对象
	*/
	//************************************
	class PollingEventPool
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;

	public:

		//************************************
		/*
			监听器

			事件队列内部派发流程：

			Listener_->BeforePopEvent();

				auto event = EventQueue_.PopEvent();

			Listener_->BeforePostEvent(event);

				DispatchEvent(event);

			Listener_->AfterPostEvent(event);
		*/
		//************************************
		class	Listener
		{
		public:
			virtual ~Listener() {}

			virtual	void	BeforePopEvent() {}

			virtual void	BeforePostEvent(const IEventSPtr& event) {}

			virtual void	AfterPostEvent(const IEventSPtr& event) {}
		};
		typedef	std::shared_ptr<Listener>	ListenerSPtr;

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

			virtual ~Dispatcher() {}

			virtual	void	DispatchEvent(const IEventSPtr& event) = 0;
		};
		typedef	std::shared_ptr<Dispatcher>	DispatcherSPtr;

	public:

		PollingEventPool();

		~PollingEventPool();

	public:


		/**********************************************
		*	@brief 异步启动线程池
		*
		*	@param threadSize(uint32_t) 线程数量
		*	@return (bool)
		 ***********************************************/
		bool	StartAsync(uint32_t threadSize);


		/**********************************************
		*	@brief 等待所有线程结束
		*
		*	在Stop()后调用
		*	@return (void)
		 ***********************************************/
		void	JoinAllThreads();


		/**********************************************
		*	@brief 通知所有线程停止工作
		*
		*	确保所有线程已结束，需调用JoinAllThreads()
		*	@return (void)
		 ***********************************************/
		void	Stop();


		/**********************************************
		*	@brief 是否在工作状态
		*
		*	@return (bool)
		 ***********************************************/
		bool	IsStop() const;

	public:


		/**********************************************
		*	@brief 向事件队列推送事件
		*
		*	@param evt(const IEventSPtr &)
		*	@return (void)
		 ***********************************************/
		void	PostEvent(const IEventSPtr& evt);

		/**********************************************
		*	@brief 向线程池推送任务
		*
		*	@param task(const PostTask &)
		*	@return (void)
		 ***********************************************/
		void	PostWorkTast(const PostTask& task);

		/**********************************************
		*	@brief 注册事件处理器
		*
		*	仅在Default Dispatch 下有效
		*	@param slot(const EventSlot &)
		*	@return (void)
		 ***********************************************/
		template<typename T>
		void	RegisterEventCallBack(const EventSlot& slot)
		{
			static_assert(std::is_base_of<IEvent, T>::value, "Error");

			RegisterEventCallBack(typeid(T).name(), slot);
		}

		void	RegisterEventCallBack(const char* eventName, const EventSlot& slot);

		/**********************************************
		*	@brief 设置监听器
		*
		*	@param listener(const ListenerSPtr &)
		*	@return (void)
		 ***********************************************/
		void	SetListener(const ListenerSPtr& listener);

		/**********************************************
		*	@brief 设置派发器
		*
		*	@param dispatcher(const DispatcherSPtr &)
		*	@return (void)
		 ***********************************************/
		void	SetDispatcher(const DispatcherSPtr& dispatcher);
	};
}