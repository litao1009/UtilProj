#include "PreemptiveEventPool.h"
#include "EventQueue.h"
#include "EventUtil.h"

#include <thread>
#include <list>
#include <mutex>

using namespace foundation;

class PreemptiveEventPool::Imp
{
public:

	std::list<std::thread>	ThreadGroup_;
	EventQueue			EventQueue_;
	EventSlotMap		SlotMap_;
	std::mutex			SlotMutex_;
	DispatcherSPtr		Dispatcher_;
};

PreemptiveEventPool::PreemptiveEventPool() :ImpUPtr_(std::make_unique<Imp>())
{

}

PreemptiveEventPool::~PreemptiveEventPool()
{

}

bool PreemptiveEventPool::StartAsync(uint32_t threadSize)
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

	imp_.ThreadGroup_.clear();

	for ( uint32_t index = 0; index < threadSize; ++index )
	{
		imp_.ThreadGroup_.emplace_back([&imp_]()
		{
			while ( !imp_.EventQueue_.IsStop() )
			{
				auto evt = imp_.EventQueue_.PopEvent();

				if ( !evt )
				{
					continue;
				}

				if ( imp_.Dispatcher_ )
				{
					imp_.Dispatcher_->DispatchEvent(evt);
				}
				else
				{
					EventSlot curSlot;
					{
						std::unique_lock<std::mutex> lock(imp_.SlotMutex_);

						auto itor = imp_.SlotMap_.find(typeid(*evt).name());
						if ( itor != imp_.SlotMap_.end() )
						{
							curSlot = itor->second;
						}
					}

					if ( curSlot )
					{
						curSlot(evt);
					}
				}
			}
		});
	}

	return true;
}

void PreemptiveEventPool::JoinAllThreads()
{
	auto& imp_ = *ImpUPtr_;

	for ( auto& curThread : imp_.ThreadGroup_ )
	{
		curThread.join();
	}
}

void PreemptiveEventPool::Stop()
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventQueue_.Stop();
}

bool PreemptiveEventPool::IsStop() const
{
	auto& imp_ = *ImpUPtr_;

	return imp_.EventQueue_.IsStop();
}

void PreemptiveEventPool::PostEvent(const IEventSPtr& evt)
{
	auto& imp_ = *ImpUPtr_;

	imp_.EventQueue_.PushEvent(evt);
}

void PreemptiveEventPool::RegisterEventCallBack(const char* eventName, const EventSlot& slot)
{
	auto& imp_ = *ImpUPtr_;
	{
		std::unique_lock<std::mutex> lock(imp_.SlotMutex_);
		imp_.SlotMap_[eventName] = slot;
	}
}

void PreemptiveEventPool::SetDispatcher(const DispatcherSPtr& dispatcher)
{
	auto& imp_ = *ImpUPtr_;

	imp_.Dispatcher_ = dispatcher;
}
