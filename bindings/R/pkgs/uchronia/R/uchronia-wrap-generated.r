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
  return(mkSwiftObjRef(result, 'char*'))
}

#' RegisterExceptionCallback_R
#'
#' RegisterExceptionCallback_R Wrapper function for RegisterExceptionCallback
#'
#' @param callback R type equivalent for C++ type const void*
#' @export
RegisterExceptionCallback_R <- function(callback) {
  callback <- getSwiftXptr(callback)
  RegisterExceptionCallback_Rcpp(callback)
}

#' DisposeSharedPointer_R
#'
#' DisposeSharedPointer_R Wrapper function for DisposeSharedPointer
#'
#' @param ptr R type equivalent for C++ type VOID_PTR_PROVIDER_PTR
#' @export
DisposeSharedPointer_R <- function(ptr) {
  ptr <- getSwiftXptr(ptr)
  DisposeSharedPointer_Rcpp(ptr)
}

#' DeleteDoubleArray_R
#'
#' DeleteDoubleArray_R Wrapper function for DeleteDoubleArray
#'
#' @param value R type equivalent for C++ type double*
#' @export
DeleteDoubleArray_R <- function(value) {
  value <- getSwiftXptr(value)
  DeleteDoubleArray_Rcpp(value)
}

#' SetTimeSeriesMissingValueValue_R
#'
#' SetTimeSeriesMissingValueValue_R Wrapper function for SetTimeSeriesMissingValueValue
#'
#' @param missingValueValue R type equivalent for C++ type double
#' @export
SetTimeSeriesMissingValueValue_R <- function(missingValueValue) {
  missingValueValue <- getSwiftXptr(missingValueValue)
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
  filename <- getSwiftXptr(filename)
  dataPath <- getSwiftXptr(dataPath)
  result <- LoadEnsembleDataset_Rcpp(filename, dataPath)
  return(mkSwiftObjRef(result, 'ENSEMBLE_DATA_SET_PTR'))
}

#' CreateEnsembleDataset_R
#'
#' CreateEnsembleDataset_R Wrapper function for CreateEnsembleDataset
#'
#' @param type R type equivalent for C++ type const char*
#' @export
CreateEnsembleDataset_R <- function(type) {
  type <- getSwiftXptr(type)
  result <- CreateEnsembleDataset_Rcpp(type)
  return(mkSwiftObjRef(result, 'ENSEMBLE_DATA_SET_PTR'))
}


#' GetEnsembleDatasetDataIdentifiers_R
#'
#' GetEnsembleDatasetDataIdentifiers_R Wrapper function for GetEnsembleDatasetDataIdentifiers
#'
#' @export

GetEnsembleDatasetDataIdentifiers_R <- function(dataLibrary)
{
    dataLibrary <- getSwiftXptr(dataLibrary)

    result <- GetEnsembleDatasetDataIdentifiers_Rcpp(dataLibrary);
    return(mkSwiftObjRef(result,'dummytype'))
}


#' GetEnsembleDatasetDataSubIdentifiers_R
#'
#' GetEnsembleDatasetDataSubIdentifiers_R Wrapper function for GetEnsembleDatasetDataSubIdentifiers
#'
#' @export

GetEnsembleDatasetDataSubIdentifiers_R <- function(dataLibrary, dataCollectionId)
{
    dataLibrary <- getSwiftXptr(dataLibrary)
dataCollectionId <- getSwiftXptr(dataCollectionId)

    result <- GetEnsembleDatasetDataSubIdentifiers_Rcpp(dataLibrary, dataCollectionId);
    return(mkSwiftObjRef(result,'dummytype'))
}


#' GetEnsembleDatasetDataSummaries_R
#'
#' GetEnsembleDatasetDataSummaries_R Wrapper function for GetEnsembleDatasetDataSummaries
#'
#' @export

GetEnsembleDatasetDataSummaries_R <- function(dataLibrary)
{
    dataLibrary <- getSwiftXptr(dataLibrary)

    result <- GetEnsembleDatasetDataSummaries_Rcpp(dataLibrary);
    return(mkSwiftObjRef(result,'dummytype'))
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
  start <- getSwiftXptr(start)
  length <- getSwiftXptr(length)
  timeStepName <- getSwiftXptr(timeStepName)
  result <- CreateEnsembleForecastTimeSeries_Rcpp(start, length, timeStepName)
  return(mkSwiftObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

#' GetDatasetSingleTimeSeries_R
#'
#' GetDatasetSingleTimeSeries_R Wrapper function for GetDatasetSingleTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataId R type equivalent for C++ type const char*
#' @export
GetDatasetSingleTimeSeries_R <- function(dataLibrary, dataId) {
  dataLibrary <- getSwiftXptr(dataLibrary)
  dataId <- getSwiftXptr(dataId)
  result <- GetDatasetSingleTimeSeries_Rcpp(dataLibrary, dataId)
  return(mkSwiftObjRef(result, 'TIME_SERIES_PTR'))
}

#' GetDatasetEnsembleTimeSeries_R
#'
#' GetDatasetEnsembleTimeSeries_R Wrapper function for GetDatasetEnsembleTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataEnsembleId R type equivalent for C++ type const char*
#' @export
GetDatasetEnsembleTimeSeries_R <- function(dataLibrary, dataEnsembleId) {
  dataLibrary <- getSwiftXptr(dataLibrary)
  dataEnsembleId <- getSwiftXptr(dataEnsembleId)
  result <- GetDatasetEnsembleTimeSeries_Rcpp(dataLibrary, dataEnsembleId)
  return(mkSwiftObjRef(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))
}

#' GetEnsembleTimeSeriesData_R
#'
#' GetEnsembleTimeSeriesData_R Wrapper function for GetEnsembleTimeSeriesData
#'
#' @param ensSeries R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @export
GetEnsembleTimeSeriesData_R <- function(ensSeries) {
  ensSeries <- getSwiftXptr(ensSeries)
  result <- GetEnsembleTimeSeriesData_Rcpp(ensSeries)
  return(mkSwiftObjRef(result, 'multi_regular_time_series_data*'))
}

#' GetDatasetEnsembleForecastTimeSeries_R
#'
#' GetDatasetEnsembleForecastTimeSeries_R Wrapper function for GetDatasetEnsembleForecastTimeSeries
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataId R type equivalent for C++ type const char*
#' @export
GetDatasetEnsembleForecastTimeSeries_R <- function(dataLibrary, dataId) {
  dataLibrary <- getSwiftXptr(dataLibrary)
  dataId <- getSwiftXptr(dataId)
  result <- GetDatasetEnsembleForecastTimeSeries_Rcpp(dataLibrary, dataId)
  return(mkSwiftObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

#' GetTimeSeriesGeometry_R
#'
#' GetTimeSeriesGeometry_R Wrapper function for GetTimeSeriesGeometry
#'
#' @param timeSeries R type equivalent for C++ type TIME_SERIES_PTR
#' @param geom R type equivalent for C++ type TS_GEOMETRY_PTR
#' @export
GetTimeSeriesGeometry_R <- function(timeSeries, geom) {
  timeSeries <- getSwiftXptr(timeSeries)
  geom <- getSwiftXptr(geom)
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
  timeSeries <- getSwiftXptr(timeSeries)
  geom <- getSwiftXptr(geom)
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
  timeSeries <- getSwiftXptr(timeSeries)
  values <- getSwiftXptr(values)
  arrayLength <- getSwiftXptr(arrayLength)
  GetTimeSeriesValues_Rcpp(timeSeries, values, arrayLength)
}

#' GetNumTimeSeries_R
#'
#' GetNumTimeSeries_R Wrapper function for GetNumTimeSeries
#'
#' @export
GetNumTimeSeries_R <- function() {
  result <- GetNumTimeSeries_Rcpp()
  return(mkSwiftObjRef(result, 'int'))
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
  dataLibrary <- getSwiftXptr(dataLibrary)
  variableIdentifier <- getSwiftXptr(variableIdentifier)
  geom <- getSwiftXptr(geom)
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
  dataLibrary <- getSwiftXptr(dataLibrary)
  variableIdentifier <- getSwiftXptr(variableIdentifier)
  values <- getSwiftXptr(values)
  arrayLength <- getSwiftXptr(arrayLength)
  GetProviderTimeSeriesValues_Rcpp(dataLibrary, variableIdentifier, values, arrayLength)
}


#' GetProviderTimeSeriesIdentifiers_R
#'
#' GetProviderTimeSeriesIdentifiers_R Wrapper function for GetProviderTimeSeriesIdentifiers
#'
#' @export

GetProviderTimeSeriesIdentifiers_R <- function(dataLibrary)
{
    dataLibrary <- getSwiftXptr(dataLibrary)

    result <- GetProviderTimeSeriesIdentifiers_Rcpp(dataLibrary);
    return(mkSwiftObjRef(result,'dummytype'))
}

