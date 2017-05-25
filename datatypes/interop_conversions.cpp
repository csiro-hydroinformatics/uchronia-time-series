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





