#include "cinterop/c_cpp_interop.hpp"
#include "cinterop/timeseries_interop.hpp"
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

TimeSeries CreateTimeSeries(const multi_regular_time_series_data& g)
{
	if (g.ensemble_size == 0)
		throw std::logic_error("time series data structure has ensemble size 0");
	return CreateTimeSeries(g.numeric_data[0], g.time_series_geometry);
}

TimeSeriesEnsemble<TimeSeries> ToTimeSeriesEnsemble(const multi_regular_time_series_data& rawData)
{
	if (rawData.ensemble_size == 0)
		throw std::logic_error("time series data structure has ensemble size 0");
	auto start = to_ptime(rawData.time_series_geometry.start);
	TimeSeriesEnsemble<TimeSeries> ts(rawData.numeric_data, rawData.ensemble_size, rawData.time_series_geometry.length, start, TimeStep::FromSeconds(rawData.time_series_geometry.time_step_seconds));
	return ts;
}

TimeSeriesEnsemble<TimeSeries>* ToTimeSeriesEnsemblePtr(const multi_regular_time_series_data& rawData)
{
	auto start = to_ptime(rawData.time_series_geometry.start);
	return new TimeSeriesEnsemble<TimeSeries>(rawData.numeric_data, rawData.ensemble_size, rawData.time_series_geometry.length, start, TimeStep::FromSeconds(rawData.time_series_geometry.time_step_seconds));
}

multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeriesEnsemble<TimeSeries>& mts)
{
	multi_regular_time_series_data* result = new multi_regular_time_series_data();
	CopyToMultiTimeSeriesData(mts, *result);
	return result;
}

multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeries& ts)
{
	multi_regular_time_series_data* result = new multi_regular_time_series_data();
	CopyToMultiTimeSeriesData(ts, *result);
	return result;
}

TimeSeries* SingleTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts)
{
	TimeSeries* result = new TimeSeries();
	CopyFromMultiTimeSeriesData(ts, *result);
	return result;
}

TimeSeriesEnsemble<TimeSeries>* MultiTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts)
{
	TimeSeriesEnsemble<TimeSeries>* result = new TimeSeriesEnsemble<TimeSeries>();
	CopyFromMultiTimeSeriesData(ts, *result);
	return result;
}

TimeSeries SingleTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts)
{
	TimeSeries result;
	CopyFromMultiTimeSeriesData(ts, result);
	return result;
}

TimeSeriesEnsemble<TimeSeries> MultiTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts)
{
	TimeSeriesEnsemble<TimeSeries> result;
	CopyFromMultiTimeSeriesData(ts, result);
	return result;
}

void CopyToMultiTimeSeriesData(const TimeSeriesEnsemble<TimeSeries>& mts, multi_regular_time_series_data& result)
{
	result.ensemble_size = mts.Size();
	auto ts = mts.Get(0);
	ToTimeSeriesGeomStruct<TimeSeries>(ts, result.time_series_geometry);
	result.numeric_data = ToRawData(mts);
}

void CopyToMultiTimeSeriesData(const TimeSeries& ts, multi_regular_time_series_data& result)
{
	result.ensemble_size = 1;
	ToTimeSeriesGeomStruct<TimeSeries>(ts, result.time_series_geometry);
	result.numeric_data = new double*[result.ensemble_size];
	result.numeric_data[0] = ToRawData(ts);
}

void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeries& ts)
{
	auto geom = interopdata.time_series_geometry;
	auto values = cinterop::utils::to_cpp_numeric_vector(interopdata.numeric_data[0], geom.length);
	ts.Reset(values,
		cinterop::utils::from_date_time_to_second<ptime>(geom.start),
		TimeStep::FromSeconds(geom.time_step_seconds));
}

void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeriesEnsemble<TimeSeries>& mts)
{
	auto geom = interopdata.time_series_geometry;
	TimeSeriesEnsemble<TimeSeries> tmp(
		interopdata.numeric_data, interopdata.ensemble_size, geom.length,
		cinterop::utils::from_date_time_to_second<ptime>(geom.start),
		TimeStep::FromSeconds(geom.time_step_seconds));
	mts = tmp;
}

double** ToRawData(const TimeSeriesEnsemble<TimeSeries>& mts)
{
	size_t ensSize = mts.Size();
	double** result = new double*[ensSize];
	vector<double*>* values = mts.GetValues();
	for (size_t i = 0; i < ensSize; i++)
	{
		TimeSeries timeSeries = mts.Get(i);
		result[i] = new double[timeSeries.GetLength()];
		timeSeries.CopyWithMissingValueTo(result[i], datatypes::interop::MissingValueHandling::TimeSeriesMissingValueValue);
	}
	delete values;
	return result;
}

double* ToRawData(const TimeSeries& ts)
{
	double* result = new double[ts.GetLength()];
	ts.CopyWithMissingValueTo(result, datatypes::interop::MissingValueHandling::TimeSeriesMissingValueValue);
	return result;
}

time_series_dimensions_description* ToTimeSeriesDimensionDescriptions(vector<DataDimensionDescriptor>& mts)
{
	time_series_dimensions_description* res = new time_series_dimensions_description();
	res->dimensions = new time_series_dimension_description[mts.size()];
	res->num_dimensions = mts.size();
	for (size_t i = 0; i < mts.size(); i++)
	{
		res->dimensions[i].dimension_type = STRDUP(mts[i].DimensionType.c_str());
		res->dimensions[i].size = mts[i].Size;
	}
	return res;
}

void DisposeMultiTimeSeriesData(multi_regular_time_series_data* d)
{
	if (d != nullptr)
	{
		cinterop::disposal::dispose_of<multi_regular_time_series_data>(*d);
		delete d;
	}
}

void DisposeTimeSeriesDimensionDescriptions(time_series_dimensions_description* d)
{
	if (d != nullptr)
	{
		cinterop::disposal::dispose_of<time_series_dimensions_description>(*d);
		delete d;
	}
}





