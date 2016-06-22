#pragma once

#include "IEvent.h"

#include <unordered_map>

namespace foundation
{
	typedef	std::unordered_map<const char*, EventSlot>	EventSlotMap;
}