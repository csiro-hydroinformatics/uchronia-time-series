////////////////////////////////////
// 
// *** THIS FILE IS GENERATED ****
// DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
// 
////////////////////////////////////

#include "uchronia.h"
#include "uchronia_wrapper_setup.h"
#include "uchronia_struct_interop.h"

using namespace Rcpp;
using namespace cinterop::utils;
using moirai::opaque_pointer_handle;

//////////// End of preamble ////////////
// [[Rcpp::export]]
CharacterVector GetLastStdExceptionMessage_Rcpp()
{
    auto result = GetLastStdExceptionMessage();
    auto x = CharacterVector(result);
    DeleteAnsiString(result);
    return x;
}

// [[Rcpp::export]]
void RegisterExceptionCallback_Rcpp(XPtr<opaque_pointer_handle> callback)
{
    RegisterExceptionCallback(callback->get());
}

// [[Rcpp::export]]
void DisposeSharedPointer_Rcpp(XPtr<opaque_pointer_handle> ptr)
{
    DisposeSharedPointer(ptr->get());
}

// [[Rcpp::export]]
void DeleteDoubleArray_Rcpp(NumericVector value)
{
    DeleteDoubleArray(&(value[0]));
}

// [[Rcpp::export]]
void SetTimeSeriesMissingValueValue_Rcpp(NumericVector missingValueValue)
{
    SetTimeSeriesMissingValueValue(as<double>(missingValueValue));
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> LoadEnsembleDataset_Rcpp(CharacterVector libraryIdentifier, CharacterVector dataPath)
{
    auto result = LoadEnsembleDataset(libraryIdentifier[0], dataPath[0]);
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> CreateEnsembleDataset_Rcpp(CharacterVector type)
{
    auto result = CreateEnsembleDataset(type[0]);
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary)
{
	int size; 
        char** values = GetEnsembleDatasetDataIdentifiers(dataLibrary->get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataSubIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataCollectionId)
{
	int size; 
        char** values = GetEnsembleDatasetDataSubIdentifiers(dataLibrary->get(), dataCollectionId[0],  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataSummaries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary)
{
	int size; 
        char** values = GetEnsembleDatasetDataSummaries(dataLibrary->get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> CreateEnsembleForecastTimeSeries_Rcpp(Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName)
{
    date_time_to_second start_datetime = to_date_time_to_second<Rcpp::Datetime>(start);
    auto result = CreateEnsembleForecastTimeSeries(start_datetime, as<int>(length), timeStepName[0]);
    // start_datetime - no cleanup needed
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> GetDatasetSingleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataId)
{
    auto result = GetDatasetSingleTimeSeries(dataLibrary->get(), dataId[0]);
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> GetDatasetEnsembleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataEnsembleId)
{
    auto result = GetDatasetEnsembleTimeSeries(dataLibrary->get(), dataEnsembleId[0]);
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> GetDatasetEnsembleForecastTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataId)
{
    auto result = GetDatasetEnsembleForecastTimeSeries(dataLibrary->get(), dataId[0]);
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> TimeSeriesFromEnsembleOfTimeSeries_Rcpp(XPtr<opaque_pointer_handle> collectionTs, IntegerVector index)
{
    auto result = TimeSeriesFromEnsembleOfTimeSeries(collectionTs->get(), as<int>(index));
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp(XPtr<opaque_pointer_handle> efts, IntegerVector indexInIssueTime, IntegerVector indexInForecastTime)
{
    auto result = TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(efts->get(), as<int>(indexInIssueTime), as<int>(indexInForecastTime));
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> CreatePerfectForecastTimeSeries_Rcpp(XPtr<opaque_pointer_handle> observations, Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName, IntegerVector offsetForecasts, IntegerVector leadTime)
{
    date_time_to_second start_datetime = to_date_time_to_second<Rcpp::Datetime>(start);
    auto result = CreatePerfectForecastTimeSeries(observations->get(), start_datetime, as<int>(length), timeStepName[0], as<int>(offsetForecasts), as<int>(leadTime));
    // start_datetime - no cleanup needed
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> ToStructEnsembleTimeSeriesData_Rcpp(XPtr<opaque_pointer_handle> ensSeries)
{
    auto result = ToStructEnsembleTimeSeriesData(ensSeries->get());
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
XPtr<opaque_pointer_handle> ToStructSingleTimeSeriesData_Rcpp(XPtr<opaque_pointer_handle> timeSeries)
{
    auto result = ToStructSingleTimeSeriesData(timeSeries->get());
    auto x = XPtr<opaque_pointer_handle>(new opaque_pointer_handle(result));
    return x;
}

// [[Rcpp::export]]
void GetTimeSeriesGeometry_Rcpp(XPtr<opaque_pointer_handle> timeSeries, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = cinterop::timeseries::to_regular_time_series_geometry_ptr(geom);
    GetTimeSeriesGeometry(timeSeries->get(), geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetEnsembleForecastTimeSeriesGeometry_Rcpp(XPtr<opaque_pointer_handle> timeSeries, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = cinterop::timeseries::to_regular_time_series_geometry_ptr(geom);
    GetEnsembleForecastTimeSeriesGeometry(timeSeries->get(), geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetTimeSeriesValues_Rcpp(XPtr<opaque_pointer_handle> timeSeries, NumericVector values, IntegerVector arrayLength)
{
    GetTimeSeriesValues(timeSeries->get(), &(values[0]), as<int>(arrayLength));
}

// [[Rcpp::export]]
IntegerVector GetNumTimeSeries_Rcpp()
{
    auto result = GetNumTimeSeries();
    auto x = Rcpp::wrap(result);
    return x;
}

// [[Rcpp::export]]
void GetProviderTsGeometry_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector variableIdentifier, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = cinterop::timeseries::to_regular_time_series_geometry_ptr(geom);
    GetProviderTsGeometry(dataLibrary->get(), variableIdentifier[0], geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetProviderTimeSeriesValues_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector variableIdentifier, NumericVector values, IntegerVector arrayLength)
{
    GetProviderTimeSeriesValues(dataLibrary->get(), variableIdentifier[0], &(values[0]), as<int>(arrayLength));
}


// [[Rcpp::export]]
CharacterVector GetProviderTimeSeriesIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary)
{
	int size; 
        char** values = GetProviderTimeSeriesIdentifiers(dataLibrary->get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}

