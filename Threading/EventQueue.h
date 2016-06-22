#pragma once

#include "IEvent.h"

#include <memory>

namespace foundation
{
	//************************************
	/*
	�̰߳�ȫ�Ķ��У�FIFO��
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
		*	@brief ���ö�������¼�����
		*
		*	
		*	@param maxNr(uint32_t)
		*	@return (void)
		 ***********************************************/
		void	SetMaxEventsSize(uint32_t maxNr);

		/**********************************************
		*	@brief ����¼�
		*
		*	@param event(const IEventSPtr &)
		*	@return (void)
		 ***********************************************/
		void	PushEvent(const IEventSPtr& event);

		/**********************************************
		*	@brief ��ȡ�¼���ͬ����
		*
		*	@return (foundation::IEventSPtr)
		 ***********************************************/
		IEventSPtr	PopEvent();

		/**********************************************
		*	@brief ��ʼ����
		*
		*	@return (bool)
		 ***********************************************/
		bool	Start();

		/**********************************************
		*	@brief ֹͣ����������PopEvent()���᷵�ؿն���
		*
		*	@return (void)
		 ***********************************************/
		void	Stop();

		/**********************************************
		*	@brief �Ƿ�ֹͣ����
		*
		*	@return (bool)
		 ***********************************************/
		bool	IsStop() const;

		void	Clean();
	};
}