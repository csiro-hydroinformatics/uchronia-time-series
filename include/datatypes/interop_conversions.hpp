#pragma once

#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/common.h"
#include "datatypes/time_series_io.hpp"
#include "datatypes/interop_conversions.h"

using namespace cinterop::utils;
using namespace datatypes::timeseries;

template<typename Tts>
static void ToTimeSeriesGeomStruct(const Tts& ts, regular_time_series_geometry& g)
{
	g.length = ts.GetLength();
	ptime startpt = ts.GetStartDate();
	g.time_step_seconds = ts.GetTimeStep().GetTimeStepDuration(startpt).total_seconds();
	to_date_time_to_second(startpt, g.start);
}

template<typename Tts>
static regular_time_series_geometry ToTimeSeriesGeomStruct(const Tts& ts)
{
	regular_time_series_geometry g;
	ToTimeSeriesGeomStruct(ts, g);
	return g;
}
