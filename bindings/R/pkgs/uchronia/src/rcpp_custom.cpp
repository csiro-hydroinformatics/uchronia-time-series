#include "uchronia_wrapper_setup.h"
#include "uchronia_r_exports.h"
#include "uchronia_struct_interop.h"

#include "cinterop/rcpp_interop.hpp"


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
	mts.start = to_date_time_to_second<Rcpp::Datetime>(rTsInfo.slot("Start"));
	mts.length = as<int>(rTsInfo.slot("Length"));
	mts.time_step_seconds = as<int>(rTsInfo.slot("TimeStepSeconds"));
}

regular_time_series_geometry toMarshalledTsinfo(const Rcpp::S4& rTsInfo)
{
	regular_time_series_geometry mts;
	toMarshalledTsinfo(rTsInfo, mts);
	return mts;
}

regular_time_series_geometry* toMarshalledTsinfoPtr(const Rcpp::S4& rTsInfo)
{
	regular_time_series_geometry* mts = new regular_time_series_geometry();
	toMarshalledTsinfo(rTsInfo, *mts);
	return mts;
}

NumericMatrix toNumericMatrix(const multi_regular_time_series_data& mts)
{
	size_t length = mts.time_series_geometry.length;
	size_t ensSize = mts.ensemble_size;
	NumericMatrix m(mts.time_series_geometry.length /*nrows*/, mts.ensemble_size /*ncols*/);
	for (size_t i = 0; i < ensSize; i++)
	{
		NumericVector values = to_custom_numeric_vector<NumericVector>(mts.numeric_data[i], length, false);
		m(_, i) = values;
	}
	return m;
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
	multi_regular_time_series_data result;

	const Rcpp::S4& rTsInfo = timeSeriesEnsemble.slot("TsGeom");
	result.ensemble_size = timeSeriesEnsemble.slot("EnsembleSize");
	const Rcpp::NumericMatrix& m = timeSeriesEnsemble.slot("NumericData");
	result.numeric_data = to_double_ptr_array<NumericMatrix>(m);
	result.time_series_geometry = toMarshalledTsinfo(rTsInfo);
	return result;
}

Rcpp::S4 toRMultiTimeSeriesData(const multi_regular_time_series_data& mts)
{
	Rcpp::S4 timeSeriesEnsemble("SwiftMultiTimeSeriesData");
	timeSeriesEnsemble.slot("TsGeom") = fromMarshalledTsinfo(mts.time_series_geometry);
	timeSeriesEnsemble.slot("EnsembleSize") = mts.ensemble_size;
	timeSeriesEnsemble.slot("NumericData") = toNumericMatrix(mts);

	/*
	Note that the class character itself has a list as an attribute. May be necessary, not sure yet
	> attributes(attributes(blah)$class)
	$package
	[1] "swift"
	*/
	return timeSeriesEnsemble;
}

Rcpp::S4 fromMarshalledTsinfo(const regular_time_series_geometry& mts)
{
	Rcpp::S4 rTsInfo("SwiftTsGeometry");
	rTsInfo.slot("Start") = to_posix_ct_date_time<NumericVector>(mts.start);
	rTsInfo.slot("Length") = mts.length;
	rTsInfo.slot("TimeStepSeconds") = mts.time_step_seconds;

	// 	Note that the class character itself has a list as an attribute. May be necessary, not sure yet
	return rTsInfo;
}

void PkgDisposeMultiTimeSeriesData(multi_regular_time_series_data& d)
{
	if (d.numeric_data != nullptr)
	{
		for (int i = 0; i < d.ensemble_size; i++)
			if (d.numeric_data[i] != nullptr)
			{
				delete d.numeric_data[i];
				d.numeric_data[i] = nullptr;
			}
		d.numeric_data = nullptr;
	}
}





