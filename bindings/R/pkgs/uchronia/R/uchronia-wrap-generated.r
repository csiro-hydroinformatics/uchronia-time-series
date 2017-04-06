##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

#' GetLastStdExceptionMessage_R
#'
#' GetLastStdExceptionMessage_R Wrapper function for GetLastStdExceptionMessage
#'
#' @export
GetLastStdExceptionMessage_R <- function() {
  result <- GetLastStdExceptionMessage_Rcpp()
  return(cinterop::mkExternalObjRef(result, 'char*'))
}

#' RegisterExceptionCallback_R
#'
#' RegisterExceptionCallback_R Wrapper function for RegisterExceptionCallback
#'
#' @param callback R type equivalent for C++ type const void*
#' @export
RegisterExceptionCallback_R <- function(callback) {
  callback <- cinterop::getExternalXptr(callback)
  RegisterExceptionCallback_Rcpp(callback)
}

#' DisposeSharedPointer_R
#'
#' DisposeSharedPointer_R Wrapper function for DisposeSharedPointer
#'
#' @param ptr R type equivalent for C++ type VOID_PTR_PROVIDER_PTR
#' @export
DisposeSharedPointer_R <- function(ptr) {
  ptr <- cinterop::getExternalXptr(ptr)
  DisposeSharedPointer_Rcpp(ptr)
}

#' DeleteDoubleArray_R
#'
#' DeleteDoubleArray_R Wrapper function for DeleteDoubleArray
#'
#' @param value R type equivalent for C++ type double*
#' @export
DeleteDoubleArray_R <- function(value) {
  value <- cinterop::getExternalXptr(value)
  DeleteDoubleArray_Rcpp(value)
}

#' SetTimeSeriesMissingValueValue_R
#'
#' SetTimeSeriesMissingValueValue_R Wrapper function for SetTimeSeriesMissingValueValue
#'
#' @param missingValueValue R type equivalent for C++ type double
#' @export
SetTimeSeriesMissingValueValue_R <- function(missingValueValue) {
  missingValueValue <- cinterop::getExternalXptr(missingValueValue)
  SetTimeSeriesMissingValueValue_Rcpp(missingValueValue)
}

#' LoadEnsembleDataset_R
#'
#' LoadEnsembleDataset_R Wrapper function for LoadEnsembleDataset
#'
#' @param filename R type equivalent for C++ type const char*
#' @param dataPath R type equivalent for C++ type const char*
#' @export
LoadEnsembleDataset_R <- function(filename, dataPath) {
  filename <- cinterop::getExternalXptr(filename)
  dataPath <- cinterop::getExternalXptr(dataPath)
  result <- LoadEnsembleDataset_Rcpp(filename, dataPath)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_DATA_SET_PTR'))
}

#' CreateEnsembleDataset_R
#'
#' CreateEnsembleDataset_R Wrapper function for CreateEnsembleDataset
#'
#' @param type R type equivalent for C++ type const char*
#' @export
CreateEnsembleDataset_R <- function(type) {
  type <- cinterop::getExternalXptr(type)
  result <- CreateEnsembleDataset_Rcpp(type)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_DATA_SET_PTR'))
}


#' GetEnsembleDatasetDataIdentifiers_R
#'
#' GetEnsembleDatasetDataIdentifiers_R Wrapper function for GetEnsembleDatasetDataIdentifiers
#'
#' @export

GetEnsembleDatasetDataIdentifiers_R <- function(dataLibrary)
{
    dataLibrary <- cinterop::getExternalXptr(dataLibrary)

    result <- GetEnsembleDatasetDataIdentifiers_Rcpp(dataLibrary);
    return(cinterop::mkExternalObjRef(result,'dummytype'))
}


#' GetEnsembleDatasetDataSubIdentifiers_R
#'
#' GetEnsembleDatasetDataSubIdentifiers_R Wrapper function for GetEnsembleDatasetDataSubIdentifiers
#'
#' @export

GetEnsembleDatasetDataSubIdentifiers_R <- function(dataLibrary, dataCollectionId)
{
    dataLibrary <- cinterop::getExternalXptr(dataLibrary)
dataCollectionId <- cinterop::getExternalXptr(dataCollectionId)

    result <- GetEnsembleDatasetDataSubIdentifiers_Rcpp(dataLibrary, dataCollectionId);
    return(cinterop::mkExternalObjRef(result,'dummytype'))
}


#' GetEnsembleDatasetDataSummaries_R
#'
#' GetEnsembleDatasetDataSummaries_R Wrapper function for GetEnsembleDatasetDataSummaries
#'
#' @export

GetEnsembleDatasetDataSummaries_R <- function(dataLibrary)
{
    dataLibrary <- cinterop::getExternalXptr(dataLibrary)

    result <- GetEnsembleDatasetDataSummaries_Rcpp(dataLibrary);
    return(cinterop::mkExternalObjRef(result,'dummytype'))
}

#' CreateEnsembleForecastTimeSeries_R
#'
#' CreateEnsembleForecastTimeSeries_R Wrapper function for CreateEnsembleForecastTimeSeries
#'
#' @param start R type equivalent for C++ type date_time_to_second
#' @param length R type equivalent for C++ type int
#' @param timeStepName R type equivalent for C++ type const char*
#' @export
CreateEnsembleForecastTimeSeries_R <- function(start, length, timeStepName) {
  start <- cinterop::getExternalXptr(start)
  length <- cinterop::getExternalXptr(length)
  timeStepName <- cinterop::getExternalXptr(timeStepName)
  result <- CreateEnsembleForecastTimeSeries_Rcpp(start, length, timeStepName)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

#' GetDatasetSingleTimeSeries_R
#'
#' GetDatasetSingleTimeSeries_R Wrapper function for GetDatasetSingleTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataId R type equivalent for C++ type const char*
#' @export
GetDatasetSingleTimeSeries_R <- function(dataLibrary, dataId) {
  dataLibrary <- cinterop::getExternalXptr(dataLibrary)
  dataId <- cinterop::getExternalXptr(dataId)
  result <- GetDatasetSingleTimeSeries_Rcpp(dataLibrary, dataId)
  return(cinterop::mkExternalObjRef(result, 'TIME_SERIES_PTR'))
}

#' GetDatasetEnsembleTimeSeries_R
#'
#' GetDatasetEnsembleTimeSeries_R Wrapper function for GetDatasetEnsembleTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataEnsembleId R type equivalent for C++ type const char*
#' @export
GetDatasetEnsembleTimeSeries_R <- function(dataLibrary, dataEnsembleId) {
  dataLibrary <- cinterop::getExternalXptr(dataLibrary)
  dataEnsembleId <- cinterop::getExternalXptr(dataEnsembleId)
  result <- GetDatasetEnsembleTimeSeries_Rcpp(dataLibrary, dataEnsembleId)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))
}

#' GetEnsembleTimeSeriesData_R
#'
#' GetEnsembleTimeSeriesData_R Wrapper function for GetEnsembleTimeSeriesData
#'
#' @param ensSeries R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @export
GetEnsembleTimeSeriesData_R <- function(ensSeries) {
  ensSeries <- cinterop::getExternalXptr(ensSeries)
  result <- GetEnsembleTimeSeriesData_Rcpp(ensSeries)
  return(cinterop::mkExternalObjRef(result, 'multi_regular_time_series_data*'))
}

#' GetDatasetEnsembleForecastTimeSeries_R
#'
#' GetDatasetEnsembleForecastTimeSeries_R Wrapper function for GetDatasetEnsembleForecastTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataId R type equivalent for C++ type const char*
#' @export
GetDatasetEnsembleForecastTimeSeries_R <- function(dataLibrary, dataId) {
  dataLibrary <- cinterop::getExternalXptr(dataLibrary)
  dataId <- cinterop::getExternalXptr(dataId)
  result <- GetDatasetEnsembleForecastTimeSeries_Rcpp(dataLibrary, dataId)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

#' GetTimeSeriesGeometry_R
#'
#' GetTimeSeriesGeometry_R Wrapper function for GetTimeSeriesGeometry
#'
#' @param timeSeries R type equivalent for C++ type TIME_SERIES_PTR
#' @param geom R type equivalent for C++ type TS_GEOMETRY_PTR
#' @export
GetTimeSeriesGeometry_R <- function(timeSeries, geom) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  geom <- cinterop::getExternalXptr(geom)
  GetTimeSeriesGeometry_Rcpp(timeSeries, geom)
}

#' GetEnsembleForecastTimeSeriesGeometry_R
#'
#' GetEnsembleForecastTimeSeriesGeometry_R Wrapper function for GetEnsembleForecastTimeSeriesGeometry
#'
#' @param timeSeries R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param geom R type equivalent for C++ type TS_GEOMETRY_PTR
#' @export
GetEnsembleForecastTimeSeriesGeometry_R <- function(timeSeries, geom) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  geom <- cinterop::getExternalXptr(geom)
  GetEnsembleForecastTimeSeriesGeometry_Rcpp(timeSeries, geom)
}

#' GetTimeSeriesValues_R
#'
#' GetTimeSeriesValues_R Wrapper function for GetTimeSeriesValues
#'
#' @param timeSeries R type equivalent for C++ type TIME_SERIES_PTR
#' @param values R type equivalent for C++ type double*
#' @param arrayLength R type equivalent for C++ type int
#' @export
GetTimeSeriesValues_R <- function(timeSeries, values, arrayLength) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  values <- cinterop::getExternalXptr(values)
  arrayLength <- cinterop::getExternalXptr(arrayLength)
  GetTimeSeriesValues_Rcpp(timeSeries, values, arrayLength)
}

#' GetNumTimeSeries_R
#'
#' GetNumTimeSeries_R Wrapper function for GetNumTimeSeries
#'
#' @export
GetNumTimeSeries_R <- function() {
  result <- GetNumTimeSeries_Rcpp()
  return(cinterop::mkExternalObjRef(result, 'int'))
}

#' GetProviderTsGeometry_R
#'
#' GetProviderTsGeometry_R Wrapper function for GetProviderTsGeometry
#'
#' @param dataLibrary R type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
#' @param variableIdentifier R type equivalent for C++ type const char*
#' @param geom R type equivalent for C++ type TS_GEOMETRY_PTR
#' @export
GetProviderTsGeometry_R <- function(dataLibrary, variableIdentifier, geom) {
  dataLibrary <- cinterop::getExternalXptr(dataLibrary)
  variableIdentifier <- cinterop::getExternalXptr(variableIdentifier)
  geom <- cinterop::getExternalXptr(geom)
  GetProviderTsGeometry_Rcpp(dataLibrary, variableIdentifier, geom)
}

#' GetProviderTimeSeriesValues_R
#'
#' GetProviderTimeSeriesValues_R Wrapper function for GetProviderTimeSeriesValues
#'
#' @param dataLibrary R type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
#' @param variableIdentifier R type equivalent for C++ type const char*
#' @param values R type equivalent for C++ type double*
#' @param arrayLength R type equivalent for C++ type int
#' @export
GetProviderTimeSeriesValues_R <- function(dataLibrary, variableIdentifier, values, arrayLength) {
  dataLibrary <- cinterop::getExternalXptr(dataLibrary)
  variableIdentifier <- cinterop::getExternalXptr(variableIdentifier)
  values <- cinterop::getExternalXptr(values)
  arrayLength <- cinterop::getExternalXptr(arrayLength)
  GetProviderTimeSeriesValues_Rcpp(dataLibrary, variableIdentifier, values, arrayLength)
}


#' GetProviderTimeSeriesIdentifiers_R
#'
#' GetProviderTimeSeriesIdentifiers_R Wrapper function for GetProviderTimeSeriesIdentifiers
#'
#' @export

GetProviderTimeSeriesIdentifiers_R <- function(dataLibrary)
{
    dataLibrary <- cinterop::getExternalXptr(dataLibrary)

    result <- GetProviderTimeSeriesIdentifiers_Rcpp(dataLibrary);
    return(cinterop::mkExternalObjRef(result,'dummytype'))
}

