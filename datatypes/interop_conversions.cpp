#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/time_series.hpp"
#include "datatypes/time_series_io.hpp"
#include "datatypes/interop_struct.h"
#include "datatypes/common.h"

using namespace cinterop::utils;

#define DATATYPES_USE_CPP_POINTERS
#include "datatypes/interop_conversions.h"
#include "datatypes/interop_conversions.hpp"

TimeSeries CreateTimeSeries(double * values, const regular_time_series_geometry& g)
{
	ptime startDate = to_ptime(g.start);
	TimeStep tstep(seconds(g.time_step_seconds));
	TimeSeries ts(values, g.length, startDate, tstep);
	return ts;
}

TimeSeries CreateTimeSeries(double * values, TS_GEOMETRY_PTR geom)
{
	auto g = TS_GEOMETRY_DYNCAST(geom);
	return CreateTimeSeries(values, *g);
}

TimeSeriesEnsemble<TimeSeries> ToTimeSeriesEnsemble(const multi_regular_time_series_data& rawData)
{
	auto start = to_ptime(rawData.time_series_geometry.start);
	TimeSeriesEnsemble<TimeSeries> ts(rawData.numeric_data, rawData.ensemble_size, rawData.time_series_geometry.length, start, TimeStep::FromSeconds(rawData.time_series_geometry.time_step_seconds));
	return ts;
}

TimeSeriesEnsemble<TimeSeries>* ToTimeSeriesEnsemblePtr(const multi_regular_time_series_data& rawData)
{
	auto start = to_ptime(rawData.time_series_geometry.start);
	return new TimeSeriesEnsemble<TimeSeries>(rawData.numeric_data, rawData.ensemble_size, rawData.time_series_geometry.length, start, TimeStep::FromSeconds(rawData.time_series_geometry.time_step_seconds));
}

multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(TimeSeriesEnsemble<TimeSeries>& mts)
{
	multi_regular_time_series_data* result = new multi_regular_time_series_data();
	CopyToMultiTimeSeriesData(mts, *result);
	return result;
}

void CopyToMultiTimeSeriesData(TimeSeriesEnsemble<TimeSeries>& mts, multi_regular_time_series_data& result)
{
	result.ensemble_size = mts.Size();
	auto ts = mts.Get(0);
	ToTimeSeriesGeomStruct<TimeSeries>(ts, result.time_series_geometry);
	result.numeric_data = ToRawData(mts);
}

double** ToRawData(TimeSeriesEnsemble<TimeSeries>& mts)
{
	size_t ensSize = mts.Size();
	double** result = new double*[ensSize];
	vector<double*>* values = mts.GetValues();
	for (size_t i = 0; i < ensSize; i++)
	{
		auto timeSeries = mts.Get(i);
		result[i] = new double[timeSeries->GetLength()];
		timeSeries->CopyWithMissingValueTo(result[i], datatypes::interop::MissingValueHandling::TimeSeriesMissingValueValue);
	}
	delete values;
	return result;
}

void DisposeMultiTimeSeriesData(double** d, size_t n)
{
	cinterop::utils::free_c_ptr_array<double>(d, n);
}

void DisposeMultiTimeSeriesData(multi_regular_time_series_data& d)
{
	DisposeMultiTimeSeriesData(d.numeric_data, d.ensemble_size);
}





