#include "IEvent.h"

#include <atomic>

using namespace foundation;

class IEvent::Imp
{
public:

	uint64_t						UID_;
	static	std::atomic_uint64_t	s_Counter;
};
std::atomic_uint64_t	IEvent::Imp::s_Counter;



IEvent::IEvent() :IEventImpUPtr_(std::make_unique<Imp>())
{
	auto& imp_ = *IEventImpUPtr_;

	imp_.UID_ = Imp::s_Counter.fetch_add(1);
}

IEvent::~IEvent()
{

}

uint64_t IEvent::GetEventID() const
{
	auto& imp_ = *IEventImpUPtr_;

	return imp_.UID_;
}
