#pragma once

#include "PollingEventPoolFwd.h"
#include "IEvent.h"
#include "PostTask.h"

#include <type_traits>

namespace foundation
{
	//************************************
	/*
	���¼����е��̳߳ض���
	*/
	//************************************
	class PollingEventPool
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;

	public:

		//************************************
		/*
			������

			�¼������ڲ��ɷ����̣�

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
			�Զ����¼��ɷ���

			�¼������ڲ��ɷ����̣�

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
		*	@brief �첽�����̳߳�
		*
		*	@param threadSize(uint32_t) �߳�����
		*	@return (bool)
		 ***********************************************/
		bool	StartAsync(uint32_t threadSize);


		/**********************************************
		*	@brief �ȴ������߳̽���
		*
		*	��Stop()�����
		*	@return (void)
		 ***********************************************/
		void	JoinAllThreads();


		/**********************************************
		*	@brief ֪ͨ�����߳�ֹͣ����
		*
		*	ȷ�������߳��ѽ����������JoinAllThreads()
		*	@return (void)
		 ***********************************************/
		void	Stop();


		/**********************************************
		*	@brief �Ƿ��ڹ���״̬
		*
		*	@return (bool)
		 ***********************************************/
		bool	IsStop() const;

	public:


		/**********************************************
		*	@brief ���¼����������¼�
		*
		*	@param evt(const IEventSPtr &)
		*	@return (void)
		 ***********************************************/
		void	PostEvent(const IEventSPtr& evt);

		/**********************************************
		*	@brief ���̳߳���������
		*
		*	@param task(const PostTask &)
		*	@return (void)
		 ***********************************************/
		void	PostWorkTast(const PostTask& task);

		/**********************************************
		*	@brief ע���¼�������
		*
		*	����Default Dispatch ����Ч
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
		*	@brief ���ü�����
		*
		*	@param listener(const ListenerSPtr &)
		*	@return (void)
		 ***********************************************/
		void	SetListener(const ListenerSPtr& listener);

		/**********************************************
		*	@brief �����ɷ���
		*
		*	@param dispatcher(const DispatcherSPtr &)
		*	@return (void)
		 ***********************************************/
		void	SetDispatcher(const DispatcherSPtr& dispatcher);
	};
}