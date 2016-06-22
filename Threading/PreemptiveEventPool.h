#pragma once

#include "PreemptiveEventPoolFwd.h"
#include "IEvent.h"

#include <type_traits>

namespace foundation
{
	//************************************
	/*
	���¼����е��̳߳ض���
	*/
	//************************************
	class PreemptiveEventPool
	{
		class	Imp;
		std::unique_ptr<Imp>	ImpUPtr_;
		
	public:

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
		// Parameter: uint32_t threadSize	�߳�����
		/*
			�첽�����̳߳�
		*/
		//************************************
		bool	StartAsync(uint32_t threadSize);

		//************************************
		// Method:  foundation::EventThreadPool::JoinAllThreads
		// Access:    public 
		// Returns:   void
		/*
			�ȴ������߳̽�������Stop()�����
		*/
		//************************************
		void	JoinAllThreads();

		//************************************
		// Method:  foundation::EventThreadPool::Stop
		// Access:    public 
		// Returns:   void
		/*
			֪ͨ�����߳�ֹͣ������ȷ�������߳��ѽ����������JoinAllThreads()
		*/
		//************************************
		void	Stop();

		//************************************
		// Method:  foundation::EventThreadPool::IsStop
		// Access:    public 
		// Returns:   bool
		/*
			�Ƿ��ڹ���״̬
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
			�����¼�
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