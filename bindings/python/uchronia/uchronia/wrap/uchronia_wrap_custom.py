from typing import Any
import uchronia.wrap.uchronia_wrap_generated as uwg

# void toMarshalledTsinfo(const Rcpp::S4& rTsInfo, regular_time_series_geometry& mts)
# {
#     cinterop::timeseries::to_regular_time_series_geometry(rTsInfo, mts)
# }

# regular_time_series_geometry toMarshalledTsinfo(const Rcpp::S4& rTsInfo)
# {
#     return cinterop::timeseries::to_regular_time_series_geometry(rTsInfo)
# }

# regular_time_series_geometry* toMarshalledTsinfoPtr(const Rcpp::S4& rTsInfo)
# {
#     return cinterop::timeseries::to_regular_time_series_geometry_ptr(rTsInfo)
# }

# NumericMatrix toNumericMatrix(const multi_regular_time_series_data& mts)
# {
#     return cinterop::rcpp::to_r_numeric_matrix(mts)
# }

# Rcpp::S4 fromMarshalledTsinfo(const regular_time_series_geometry& mts)
# {
#     return cinterop::timeseries::from_regular_time_series_geometry<Rcpp::S4>(mts)
# }

# # [[Rcpp::export]]
# Rcpp::S4 GetTimeSeriesGeometry_Pkg(XPtr<opaque_pointer_handle> timeSeries)
# {
#     regular_time_series_geometry mtsg
#     GetTimeSeriesGeometry(timeSeries, &mtsg)
#     return fromMarshalledTsinfo(mtsg)
# }

# // should there be something like the following:
# //Rcpp::S4 GetEnsembleTimeSeriesGeometry_Pkg(XPtr<opaque_pointer_handle> timeSeries)
# //{
# //    regular_time_series_geometry mtsg
# //    GetEnsembleTimeSeriesGeometry(timeSeries, &mtsg)
# //    return fromMarshalledTsinfo(mtsg)
# //}

# # [[Rcpp::export]]
# Rcpp::S4 GetEnsembleForecastTimeSeriesGeometry_Pkg(XPtr<opaque_pointer_handle> timeSeries)
# {
#     regular_time_series_geometry mtsg
#     GetEnsembleForecastTimeSeriesGeometry(timeSeries, &mtsg)
#     return fromMarshalledTsinfo(mtsg)
# }

# # [[Rcpp::export]]
# Rcpp::S4 GetItemEnsembleForecastTimeSeries_Pkg(XPtr<opaque_pointer_handle> series, IntegerVector i)
# {
#     multi_regular_time_series_data* mts = GetItemEnsembleForecastTimeSeriesAsStructure(series, as<int>(i))
#     auto res = cinterop::timeseries::from_multi_regular_time_series_data<Rcpp::S4>(*mts)
#     // mts created by the datatypes library - we need to use its API to dispose of it, not this present R package's version
#     DisposeMultiTimeSeriesData(mts)
#     return res
# }

# # [[Rcpp::export]]
# void SetItemEnsembleForecastTimeSeries_Pkg(XPtr<opaque_pointer_handle> series, IntegerVector i, Rcpp::S4& timeSeriesEnsemble)
# {
#     multi_regular_time_series_data mts = cinterop::timeseries::to_multi_regular_time_series_data(timeSeriesEnsemble)
#     SetItemEnsembleForecastTimeSeriesAsStructure(series, as<int>(i), &mts)
# }

# # [[Rcpp::export]]
# Rcpp::S4 GetEnsembleTimeSeries_Pkg(XPtr<opaque_pointer_handle> series)
# {
#     multi_regular_time_series_data* mts = ToStructEnsembleTimeSeriesData(series)
#     auto res = cinterop::timeseries::from_multi_regular_time_series_data<Rcpp::S4>(*mts)
#     DisposeMultiTimeSeriesData(mts)
#     return res
# }

# # [[Rcpp::export]]
# def TimeSeriesToTsInfo_Pkg(timeSeries):
#     regular_time_series_geometry mtsg
#     void* ts = timeSeries
#     GetTimeSeriesGeometry(ts, &mtsg)
#     double * values = new double[mtsg.length]
#     GetTimeSeriesValues(ts, values, mtsg.length)
#     NumericVector data = to_custom_numeric_vector<NumericVector>(values, mtsg.length, false)
#     delete[] values
#     return cinterop::timeseries::make_time_series_info<List>(data, mtsg)

# Rcpp::S4 const XPtr<opaque_pointer_handle>& xptr, const string& type = "")
# {
#     return cinterop::create_rcpp_xptr_wrapper<opaque_pointer_handle>(xptr, type)
# }

# [[Rcpp::export]]
def GetDatasetFromLibrary_Pkg(data_library, data_identifier):
    desc = uwg.GetDataDimensionsDescription_py(data_library, data_identifier)
    dimensions = len(desc)
    if dimensions == 1:
        return uwg.GetDatasetSingleTimeSeries_py(data_library, data_identifier)
    elif dimensions == 2:
        return uwg.GetDatasetEnsembleTimeSeries_py(data_library, data_identifier)
    elif dimensions == 3:
        return uwg.GetDatasetEnsembleForecastTimeSeries_py(data_library, data_identifier)
    else:
        raise Exception("Number of dimensions for a data set is not supported: " + str(dimensions))



import numpy as np

def _array_for_geom(mtsg) -> np.ndarray:
    return np.empty((mtsg.length,))

# def GetTimeSeriesFromProvider_Pkg(provider, variable_identifier):
def get_time_series_data_from_provider(provider: Any, variable_identifier, mtsg):
    uwg.GetProviderTsGeometry_py(provider, variable_identifier, mtsg)
    values = _array_for_geom(mtsg)
    uwg.GetProviderTimeSeriesValues_py(
        provider, variable_identifier, values, mtsg.length
    )
    return values


