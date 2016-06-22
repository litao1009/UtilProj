#pragma once

#include "IEvent.h"

#include <memory>

namespace foundation
{
	//************************************
	/*
	线程安全的队列（FIFO）
	*/
	//************************************
	class EventQueue
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;
		
	public:
	
		EventQueue();
		
		~EventQueue();

	public:

		/**********************************************
		*	@brief 设置队列最多事件数量
		*
		*	
		*	@param maxNr(uint32_t)
		*	@return (void)
		 ***********************************************/
		void	SetMaxEventsSize(uint32_t maxNr);

		/**********************************************
		*	@brief 添加事件
		*
		*	@param event(const IEventSPtr &)
		*	@return (void)
		 ***********************************************/
		void	PushEvent(const IEventSPtr& event);

		/**********************************************
		*	@brief 获取事件（同步）
		*
		*	@return (foundation::IEventSPtr)
		 ***********************************************/
		IEventSPtr	PopEvent();

		/**********************************************
		*	@brief 开始工作
		*
		*	@return (bool)
		 ***********************************************/
		bool	Start();

		/**********************************************
		*	@brief 停止工作，所有PopEvent()将会返回空对象
		*
		*	@return (void)
		 ***********************************************/
		void	Stop();

		/**********************************************
		*	@brief 是否停止工作
		*
		*	@return (bool)
		 ***********************************************/
		bool	IsStop() const;

		void	Clean();
	};
}