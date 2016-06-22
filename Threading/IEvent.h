#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include <functional>

namespace foundation
{
	class	IEvent;
	typedef	std::shared_ptr<IEvent>	IEventSPtr;

	class IEvent
	{
		class	Imp;
		std::unique_ptr<Imp>	IEventImpUPtr_;

	public:

		IEvent();

		virtual ~IEvent();

	public:

				uint64_t			GetEventID() const;
	};

	typedef	std::function<void(const IEventSPtr&)>	EventSlot;
}