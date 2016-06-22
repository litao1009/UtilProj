#pragma once

#include <boost/system/error_code.hpp>
#include <boost/optional.hpp>

namespace foundation
{
	typedef	boost::optional<boost::system::error_code>	ErrorCode;
}