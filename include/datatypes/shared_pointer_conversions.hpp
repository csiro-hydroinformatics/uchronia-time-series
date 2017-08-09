#pragma once

#include "datatypes/setup_exports.h"
#include "moirai/reference_handle.hpp"

namespace moirai
{
	template<> struct known_conversions<TimeSeriesProvider<double>>
	{
		static TimeSeriesProvider<double>* dyn_cast(void* p, const typeinfo& tinfo)
		{
			return as_type<TimeSeriesLibrary>(p, tinfo);
		}
	};
}