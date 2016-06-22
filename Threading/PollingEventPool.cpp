#include "PollingEventPool.h"
#include "EventQueue.h"
#include "AsyncThreadPool.h"
#include "EventUtil.h"

#include <thread>
#include <list>
#include <unordered_map>
#include <mutex>
#include <assert.h>

using namespace foundation;

class PollingEventPool::Imp
{
public:

	void	EventThreadFunc()
	{
		while ( !EventQueue_.IsStop() )
		{

			if ( Listener_ )
			{
				Listener_->BeforePopEvent();
			}

			auto event = EventQueue_.PopEvent();

			if ( !event )
			{
				assert(EventQueue_.IsStop());
				continue;
			}

			if ( Listener_ )
			{
				Listener_->BeforePostEvent(event);
			}

			if ( Dispatcher_ )
			{
				Dispatcher_->DispatchEvent(event);
			}
			else
			{
				WorkThreadGroup_.Post(std::bind(&Imp::DefaultDispathModelEvent, this, event));
			}

			if ( Listener_ )
			{
				Listener_->AfterPostEvent(event);
			}
		}
	}

	void	DefaultDispathModelEvent(const IEventSPtr& event)
	{
		EventSlot curSlot;
		{
			std::unique_lock<std::mutex> lock(SlotMutex_);

			auto itor = SlotMap_.find(typeid(*event).name());
			if ( itor != SlotMap_.end() )
			{
				curSlot = itor->second;
			}
		}

		if ( curSlot )
		{
			curSlot(event);
		}
	}

public:

	AsyncThreadPool						WorkThreadGroup_;
	EventQueue							EventQueue_;
	std::unique_ptr<std::thread>		EventThread_;
	EventSlotMap						SlotMap_;
	std::mutex							SlotMutex_;
	PollingEventPool::ListenerSPtr		Listener_;
	PollingEventPool::DispatcherSPtr	Dispatcher_;
};

PollingEventPool::PollingEventPool() :ImpUPtr_(std::make_unique<Imp>())
{

}

PollingEventPool::~PollingEventPool()
{

}

bool PollingEventPool::StartAsync(uint32_t threadSize)
{
	auto& imp_ = *ImpUPtr_;

	if ( threadSize < 1 )
	{
		return false;
	}

	if ( !imp_.EventQueue_.Start() )
	{
		return false;
	}

	auto ret = imp_.WorkThreadGroup_.StartAsync(threadSize);
	if (!ret)
	{
		return ret;
	}

	try
	{
		imp_.EventThread_ = std::make_unique<std::thread>(std::bind(&Imp::EventThreadFunc, ImpUPtr_.get()));
	}
	catch (std::exception& /*exp*/)
	{
		return false;
	}

	return true;
}

void PollingEventPool::JoinAllThreads()
{
	auto& imp_ = *ImpUPtr_;

	if ( !imp_.EventThread_ )
	{
		return;
	}

	imp_.EventThread_->join();
	imp_.WorkThreadGroup_.JoinAllThreads();
}

void PollingEventPool::Stop()
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventQueue_.Stop();
	imp_.WorkThreadGroup_.Stop();
}

bool PollingEventPool::IsStop() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.EventQueue_.IsStop();
}

void PollingEventPool::PostEvent(const IEventSPtr& evt)
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventQueue_.PushEvent(evt);
}

void PollingEventPool::RegisterEventCallBack(const char* eventName, const EventSlot& slot)
{
	auto& imp_ = *ImpUPtr_;

	{
		std::unique_lock<std::mutex> lock(imp_.SlotMutex_);
		imp_.SlotMap_[eventName] = slot;
	}
}

void PollingEventPool::SetListener(const ListenerSPtr& listener)
{
	auto& imp_ = *ImpUPtr_;

	imp_.Listener_ = listener;
}

void PollingEventPool::PostWorkTast(const PostTask& task)
{
	auto& imp_ = *ImpUPtr_;

	imp_.WorkThreadGroup_.Post(task);
}

void PollingEventPool::SetDispatcher(const DispatcherSPtr& dispatcher)
{
	auto& imp_ = *ImpUPtr_;

	imp_.Dispatcher_ = dispatcher;
}

