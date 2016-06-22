#pragma once

#include <memory>

namespace foundation
{
	class AsyncThreadPool;
	typedef	std::shared_ptr<AsyncThreadPool>	AsyncThreadPoolSPtr;
	typedef	std::weak_ptr<AsyncThreadPool>		AsyncThreadPoolWPtr;
}