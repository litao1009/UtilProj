#include "EventQueue.h"

#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace foundation;


class EventQueue::Imp
{
public:

	Imp()
	{
		MaxSize_ = 1000;
		StopFlag_ = true;
	}

public:

	std::list<IEventSPtr>	Queue_;
	std::atomic_uint32_t	MaxSize_;
	std::atomic_bool		StopFlag_;
	std::mutex				Mutex_;
	std::condition_variable	HasCondition_;
	std::condition_variable	SizeCondition_;
};

EventQueue::EventQueue() :ImpUPtr_(std::make_unique<Imp>())
{

}

EventQueue::~EventQueue()
{

}

void EventQueue::SetMaxEventsSize(uint32_t maxNr)
{
	auto& imp_ = *ImpUPtr_;

	imp_.MaxSize_ = maxNr;
}

void EventQueue::PushEvent(const IEventSPtr& event)
{
	auto& imp_ = *ImpUPtr_;

	{
		std::unique_lock<std::mutex> lock(imp_.Mutex_);

		while ( imp_.Queue_.size() > imp_.MaxSize_ && !imp_.StopFlag_ )
		{
			imp_.SizeCondition_.wait(lock);
		}

		imp_.Queue_.push_front(event);
	}

	imp_.HasCondition_.notify_one();
}

IEventSPtr EventQueue::PopEvent()
{
	auto& imp_ = *ImpUPtr_;

	std::unique_lock<std::mutex> lock(imp_.Mutex_);

	while ( imp_.Queue_.empty() && !imp_.StopFlag_ )
	{
		imp_.HasCondition_.wait(lock);
	}

	if ( imp_.StopFlag_ )
	{
		return nullptr;
	}

	auto event = imp_.Queue_.back();
	imp_.Queue_.pop_back();

	return event;
}

bool EventQueue::Start()
{
	auto& imp_ = *ImpUPtr_;

	auto ret = imp_.StopFlag_.exchange(false);

	return ret;
}

void EventQueue::Stop()
{
	auto& imp_ = *ImpUPtr_;

	auto ret = imp_.StopFlag_.exchange(true);

	if ( !ret )
	{
		imp_.HasCondition_.notify_all();
	}
}

bool EventQueue::IsStop() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.StopFlag_;
}

void EventQueue::Clean()
{
	auto& imp_ = *ImpUPtr_;

	std::unique_lock<std::mutex> lock(imp_.Mutex_);

	imp_.Queue_.clear();
}