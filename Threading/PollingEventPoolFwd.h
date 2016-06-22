#pragma once

#include <memory>

namespace foundation
{
	class PollingEventPool;
	typedef	std::shared_ptr<PollingEventPool>	PollingEventPoolSPtr;
	typedef	std::weak_ptr<PollingEventPool>		PollingEventPoolWPtr;
}
