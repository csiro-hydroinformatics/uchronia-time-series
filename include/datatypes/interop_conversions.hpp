#pragma once

#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/common.h"
#include "datatypes/time_series_io.hpp"
#include "datatypes/interop_conversions.h"
#include "cinterop/timeseries_interop.hpp"

using namespace cinterop::utils;
using namespace datatypes::timeseries;

template<typename Tts>
static void ToTimeSeriesGeomStruct(const Tts& ts, regular_time_series_geometry& g)
{
	g.length = static_cast<int>(ts.GetLength());
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

namespace cinterop
{
	namespace timeseries
	{
		template<>
		inline multi_regular_time_series_data* to_multi_regular_time_series_data_ptr<TimeSeries>(const TimeSeries& ts)
		{
			return ToMultiTimeSeriesDataPtr(ts);
		}
	}
	namespace statistics
	{
		template<>
		inline statistic_definition* to_statistic_definition_ptr<TimeSeries>(const std::string& model_variable_id, const std::string& statistic_identifier, const std::string& objective_identifier, const std::string& objective_name,
			const date_time_to_second& start, const date_time_to_second& end, const TimeSeries& time_series_data)
		// Tried to have a default implementation and relying on finding an implementation for to_multi_regular_time_series_data_ptr<From> but had issues getting this working. 
		{
			using namespace cinterop::timeseries;
			statistic_definition* stat = new statistic_definition();
			stat->statistic_identifier = STRDUP(statistic_identifier.c_str());
			stat->start = start;
			stat->end = end;
			stat->model_variable_id = STRDUP(model_variable_id.c_str());
			stat->objective_identifier = STRDUP(objective_identifier.c_str());
			stat->objective_name = STRDUP(objective_name.c_str());
			stat->observations = to_multi_regular_time_series_data_ptr<TimeSeries>(time_series_data);
			return stat;
		}

	}
}


