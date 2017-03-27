#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/time_series_io.hpp"
#include "internals_c_api.hpp"

using namespace cinterop::utils;
using namespace datatypes::timeseries;


TimeSeries CreateTimeSeries(double * values, const regular_time_series_geometry& g);
TimeSeries CreateTimeSeries(double * values, TS_GEOMETRY_PTR geom);
TimeSeriesEnsemble<TimeSeries> ToTimeSeriesEnsemble(const multi_regular_time_series_data& rawData);
TimeSeriesEnsemble<TimeSeries>* ToTimeSeriesEnsemblePtr(const multi_regular_time_series_data& rawData);
multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(TimeSeriesEnsemble<TimeSeries>& mts);
void CopyToMultiTimeSeriesData(TimeSeriesEnsemble<TimeSeries>& mts, multi_regular_time_series_data& result);
double** ToRawData(TimeSeriesEnsemble<TimeSeries>& mts);
void DisposeMultiTimeSeriesData(double** d, size_t n);
void DisposeMultiTimeSeriesData(multi_regular_time_series_data& d);

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
