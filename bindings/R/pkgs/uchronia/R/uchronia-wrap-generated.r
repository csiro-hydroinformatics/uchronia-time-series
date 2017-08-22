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
#' @param libraryIdentifier R type equivalent for C++ type const char*
#' @param dataPath R type equivalent for C++ type const char*
#' @export
LoadEnsembleDataset_R <- function(libraryIdentifier, dataPath) {
  libraryIdentifier <- cinterop::getExternalXptr(libraryIdentifier)
  dataPath <- cinterop::getExternalXptr(dataPath)
  result <- LoadEnsembleDataset_Rcpp(libraryIdentifier, dataPath)
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

#' EnsembleSizeEnsembleTimeSeries_R
#'
#' EnsembleSizeEnsembleTimeSeries_R Wrapper function for EnsembleSizeEnsembleTimeSeries
#'
#' @param ensSeries R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @export
EnsembleSizeEnsembleTimeSeries_R <- function(ensSeries) {
  ensSeries <- cinterop::getExternalXptr(ensSeries)
  result <- EnsembleSizeEnsembleTimeSeries_Rcpp(ensSeries)
  return(cinterop::mkExternalObjRef(result, 'int'))
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

#' SaveSingleTimeSeriesToNetcdf_R
#'
#' SaveSingleTimeSeriesToNetcdf_R Wrapper function for SaveSingleTimeSeriesToNetcdf
#'
#' @param timeSeries R type equivalent for C++ type TIME_SERIES_PTR
#' @param filename R type equivalent for C++ type char*
#' @param overwrite R type equivalent for C++ type bool
#' @export
SaveSingleTimeSeriesToNetcdf_R <- function(timeSeries, filename, overwrite) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  filename <- cinterop::getExternalXptr(filename)
  overwrite <- cinterop::getExternalXptr(overwrite)
  SaveSingleTimeSeriesToNetcdf_Rcpp(timeSeries, filename, overwrite)
}

#' SaveEnsembleTimeSeriesToNetcdf_R
#'
#' SaveEnsembleTimeSeriesToNetcdf_R Wrapper function for SaveEnsembleTimeSeriesToNetcdf
#'
#' @param collection R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @param filename R type equivalent for C++ type char*
#' @param overwrite R type equivalent for C++ type bool
#' @export
SaveEnsembleTimeSeriesToNetcdf_R <- function(collection, filename, overwrite) {
  collection <- cinterop::getExternalXptr(collection)
  filename <- cinterop::getExternalXptr(filename)
  overwrite <- cinterop::getExternalXptr(overwrite)
  SaveEnsembleTimeSeriesToNetcdf_Rcpp(collection, filename, overwrite)
}

#' SaveEnsembleForecastTimeSeriesToNetcdf_R
#'
#' SaveEnsembleForecastTimeSeriesToNetcdf_R Wrapper function for SaveEnsembleForecastTimeSeriesToNetcdf
#'
#' @param tsEnsTs R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param filename R type equivalent for C++ type char*
#' @param overwrite R type equivalent for C++ type bool
#' @export
SaveEnsembleForecastTimeSeriesToNetcdf_R <- function(tsEnsTs, filename, overwrite) {
  tsEnsTs <- cinterop::getExternalXptr(tsEnsTs)
  filename <- cinterop::getExternalXptr(filename)
  overwrite <- cinterop::getExternalXptr(overwrite)
  SaveEnsembleForecastTimeSeriesToNetcdf_Rcpp(tsEnsTs, filename, overwrite)
}

#' IsMissingValueItemEnsembleForecastTimeSeries_R
#'
#' IsMissingValueItemEnsembleForecastTimeSeries_R Wrapper function for IsMissingValueItemEnsembleForecastTimeSeries
#'
#' @param series R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param i R type equivalent for C++ type int
#' @export
IsMissingValueItemEnsembleForecastTimeSeries_R <- function(series, i) {
  series <- cinterop::getExternalXptr(series)
  i <- cinterop::getExternalXptr(i)
  result <- IsMissingValueItemEnsembleForecastTimeSeries_Rcpp(series, i)
  return(cinterop::mkExternalObjRef(result, 'bool'))
}

#' TimeSeriesFromEnsembleOfTimeSeries_R
#'
#' TimeSeriesFromEnsembleOfTimeSeries_R Wrapper function for TimeSeriesFromEnsembleOfTimeSeries
#'
#' @param collectionTs R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @param index R type equivalent for C++ type int
#' @export
TimeSeriesFromEnsembleOfTimeSeries_R <- function(collectionTs, index) {
  collectionTs <- cinterop::getExternalXptr(collectionTs)
  index <- cinterop::getExternalXptr(index)
  result <- TimeSeriesFromEnsembleOfTimeSeries_Rcpp(collectionTs, index)
  return(cinterop::mkExternalObjRef(result, 'TIME_SERIES_PTR'))
}

#' TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_R
#'
#' TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_R Wrapper function for TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries
#'
#' @param efts R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param indexInIssueTime R type equivalent for C++ type int
#' @param indexInForecastTime R type equivalent for C++ type int
#' @export
TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_R <- function(efts, indexInIssueTime, indexInForecastTime) {
  efts <- cinterop::getExternalXptr(efts)
  indexInIssueTime <- cinterop::getExternalXptr(indexInIssueTime)
  indexInForecastTime <- cinterop::getExternalXptr(indexInForecastTime)
  result <- TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp(efts, indexInIssueTime, indexInForecastTime)
  return(cinterop::mkExternalObjRef(result, 'TIME_SERIES_PTR'))
}

#' GetValueFromUnivariateTimeSeries_R
#'
#' GetValueFromUnivariateTimeSeries_R Wrapper function for GetValueFromUnivariateTimeSeries
#'
#' @param ts R type equivalent for C++ type TIME_SERIES_PTR
#' @param index R type equivalent for C++ type int
#' @export
GetValueFromUnivariateTimeSeries_R <- function(ts, index) {
  ts <- cinterop::getExternalXptr(ts)
  index <- cinterop::getExternalXptr(index)
  result <- GetValueFromUnivariateTimeSeries_Rcpp(ts, index)
  return(cinterop::mkExternalObjRef(result, 'double'))
}

#' TransformEachItem_R
#'
#' TransformEachItem_R Wrapper function for TransformEachItem
#'
#' @param tsEnsTs R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param method R type equivalent for C++ type char*
#' @param methodArgument R type equivalent for C++ type char*
#' @export
TransformEachItem_R <- function(tsEnsTs, method, methodArgument) {
  tsEnsTs <- cinterop::getExternalXptr(tsEnsTs)
  method <- cinterop::getExternalXptr(method)
  methodArgument <- cinterop::getExternalXptr(methodArgument)
  TransformEachItem_Rcpp(tsEnsTs, method, methodArgument)
}

#' SetValueToUnivariateTimeSeries_R
#'
#' SetValueToUnivariateTimeSeries_R Wrapper function for SetValueToUnivariateTimeSeries
#'
#' @param ts R type equivalent for C++ type TIME_SERIES_PTR
#' @param index R type equivalent for C++ type int
#' @param value R type equivalent for C++ type double
#' @export
SetValueToUnivariateTimeSeries_R <- function(ts, index, value) {
  ts <- cinterop::getExternalXptr(ts)
  index <- cinterop::getExternalXptr(index)
  value <- cinterop::getExternalXptr(value)
  SetValueToUnivariateTimeSeries_Rcpp(ts, index, value)
}

#' GetItemEnsembleTimeSeriesAsStructure_R
#'
#' GetItemEnsembleTimeSeriesAsStructure_R Wrapper function for GetItemEnsembleTimeSeriesAsStructure
#'
#' @param series R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @param i R type equivalent for C++ type int
#' @export
GetItemEnsembleTimeSeriesAsStructure_R <- function(series, i) {
  series <- cinterop::getExternalXptr(series)
  i <- cinterop::getExternalXptr(i)
  result <- GetItemEnsembleTimeSeriesAsStructure_Rcpp(series, i)
  return(cinterop::mkExternalObjRef(result, 'multi_regular_time_series_data*'))
}

#' SetItemEnsembleTimeSeriesAsStructure_R
#'
#' SetItemEnsembleTimeSeriesAsStructure_R Wrapper function for SetItemEnsembleTimeSeriesAsStructure
#'
#' @param collection R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @param i R type equivalent for C++ type int
#' @param values R type equivalent for C++ type const multi_regular_time_series_data&
#' @export
SetItemEnsembleTimeSeriesAsStructure_R <- function(collection, i, values) {
  collection <- cinterop::getExternalXptr(collection)
  i <- cinterop::getExternalXptr(i)
  values <- cinterop::getExternalXptr(values)
  SetItemEnsembleTimeSeriesAsStructure_Rcpp(collection, i, values)
}

#' CreatePerfectForecastTimeSeries_R
#'
#' CreatePerfectForecastTimeSeries_R Wrapper function for CreatePerfectForecastTimeSeries
#'
#' @param observations R type equivalent for C++ type TIME_SERIES_PTR
#' @param start R type equivalent for C++ type date_time_to_second
#' @param length R type equivalent for C++ type int
#' @param timeStepName R type equivalent for C++ type const char*
#' @param offsetForecasts R type equivalent for C++ type int
#' @param leadTime R type equivalent for C++ type int
#' @export
CreatePerfectForecastTimeSeries_R <- function(observations, start, length, timeStepName, offsetForecasts, leadTime) {
  observations <- cinterop::getExternalXptr(observations)
  start <- cinterop::getExternalXptr(start)
  length <- cinterop::getExternalXptr(length)
  timeStepName <- cinterop::getExternalXptr(timeStepName)
  offsetForecasts <- cinterop::getExternalXptr(offsetForecasts)
  leadTime <- cinterop::getExternalXptr(leadTime)
  result <- CreatePerfectForecastTimeSeries_Rcpp(observations, start, length, timeStepName, offsetForecasts, leadTime)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

#' ToStructEnsembleTimeSeriesData_R
#'
#' ToStructEnsembleTimeSeriesData_R Wrapper function for ToStructEnsembleTimeSeriesData
#'
#' @param ensSeries R type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
#' @export
ToStructEnsembleTimeSeriesData_R <- function(ensSeries) {
  ensSeries <- cinterop::getExternalXptr(ensSeries)
  result <- ToStructEnsembleTimeSeriesData_Rcpp(ensSeries)
  return(cinterop::mkExternalObjRef(result, 'multi_regular_time_series_data*'))
}

#' ToStructSingleTimeSeriesData_R
#'
#' ToStructSingleTimeSeriesData_R Wrapper function for ToStructSingleTimeSeriesData
#'
#' @param timeSeries R type equivalent for C++ type TIME_SERIES_PTR
#' @export
ToStructSingleTimeSeriesData_R <- function(timeSeries) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  result <- ToStructSingleTimeSeriesData_Rcpp(timeSeries)
  return(cinterop::mkExternalObjRef(result, 'multi_regular_time_series_data*'))
}

#' CreateEnsembleTimeSeriesDataFromStruct_R
#'
#' CreateEnsembleTimeSeriesDataFromStruct_R Wrapper function for CreateEnsembleTimeSeriesDataFromStruct
#'
#' @param ensSeries R type equivalent for C++ type const multi_regular_time_series_data&
#' @export
CreateEnsembleTimeSeriesDataFromStruct_R <- function(ensSeries) {
  ensSeries <- cinterop::getExternalXptr(ensSeries)
  result <- CreateEnsembleTimeSeriesDataFromStruct_Rcpp(ensSeries)
  return(cinterop::mkExternalObjRef(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))
}

#' CreateSingleTimeSeriesDataFromStruct_R
#'
#' CreateSingleTimeSeriesDataFromStruct_R Wrapper function for CreateSingleTimeSeriesDataFromStruct
#'
#' @param timeSeries R type equivalent for C++ type const multi_regular_time_series_data&
#' @export
CreateSingleTimeSeriesDataFromStruct_R <- function(timeSeries) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  result <- CreateSingleTimeSeriesDataFromStruct_Rcpp(timeSeries)
  return(cinterop::mkExternalObjRef(result, 'TIME_SERIES_PTR'))
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

