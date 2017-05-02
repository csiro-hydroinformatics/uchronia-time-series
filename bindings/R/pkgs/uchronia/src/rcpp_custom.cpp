#include "uchronia_wrapper_setup.h"
#include "uchronia_r_exports.h"
#include "uchronia_struct_interop.h"

#include "cinterop/rcpp_timeseries_interop.hpp"


using namespace Rcpp;
using namespace cinterop::utils;

void NativeExceptionHandler(const char * str)
{
	std::string msg(str);
	// Because Rcpp has its own exception handling meachanism, we just rethrow
	throw std::runtime_error(msg);
	// Calling Rcpp::stop(msg) is no good,  it seems, as it leads to buffer overrun in Release mode.
	//Rcpp::stop(msg);
}

// [[Rcpp::export]]
void RegisterExceptionCallback_Pkg()
{
	void* exceptionHandlerFun = (void*)(&NativeExceptionHandler);
	RegisterExceptionCallback(exceptionHandlerFun);

}

void toMarshalledTsinfo(const Rcpp::S4& rTsInfo, regular_time_series_geometry& mts)
{
	cinterop::timeseries::to_regular_time_series_geometry(rTsInfo, mts);
}

regular_time_series_geometry toMarshalledTsinfo(const Rcpp::S4& rTsInfo)
{
	return cinterop::timeseries::to_regular_time_series_geometry(rTsInfo);
}

regular_time_series_geometry* toMarshalledTsinfoPtr(const Rcpp::S4& rTsInfo)
{
	return cinterop::timeseries::to_regular_time_series_geometry_ptr(rTsInfo);
}

NumericMatrix toNumericMatrix(const multi_regular_time_series_data& mts)
{
	return cinterop::rcpp::to_r_numeric_matrix(mts);
}

/**
* \fn	multi_regular_time_series_data toMultiTimeSeriesData(const Rcpp::S4& timeSeriesEnsemble)
*
* \brief	Converts a timeSeriesEnsemble to a multi time series data.
*
* \param	timeSeriesEnsemble	The time series ensemble, as an S4 object of type SwiftMultiTimeSeriesData
*
* \return	timeSeriesEnsemble as a multi_regular_time_series_data.
*/

multi_regular_time_series_data toMultiTimeSeriesData(const Rcpp::S4& timeSeriesEnsemble)
{
	return cinterop::timeseries::to_multi_regular_time_series_data(timeSeriesEnsemble);
}

Rcpp::S4 toRMultiTimeSeriesData(const multi_regular_time_series_data& mts)
{
	return cinterop::timeseries::from_multi_regular_time_series_data<Rcpp::S4>(mts);
}

Rcpp::S4 fromMarshalledTsinfo(const regular_time_series_geometry& mts)
{
	return cinterop::timeseries::from_regular_time_series_geometry<Rcpp::S4>(mts);
}

void PkgDisposeMultiTimeSeriesData(multi_regular_time_series_data& d)
{
	cinterop::disposal::dispose_of<multi_regular_time_series_data>(d);
}





