#pragma once

#include <memory>

namespace foundation
{
	class PreemptiveEventPool;
	typedef	std::shared_ptr<PreemptiveEventPool>	PreemptiveEventPoolSPtr;
	typedef	std::weak_ptr<PreemptiveEventPool>		PreemptiveEventPoolWPtr;
}
