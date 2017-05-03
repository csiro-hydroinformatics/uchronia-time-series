#' Gets an object, a library to access a set of time series
#'
#' Gets an object, a library to access a set of time series
#'
#' @param dataSetId currently only a path to a file in YAML is supported.
#' @return an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer type ENSEMBLE_DATA_SET_PTR
#' @examples
#' \dontrun{
#' yamlFn <- file.path(system.file(package='swift', 'data'), 'time_series_library.yaml')
#' if(file.exists(yamlFn)) {stop(paste0('sample YAML file ', yamlFn, ' not found')) }
#' file.show(yamlFn)
#' dataSet <- getEnsembleDataSet(yamlFn)
#' # dataIds <- getDataSetIds(dataSet)
#' precipIds <- paste( 'subarea', getSubareaIds(simulation), 'P', sep='.')
#' evapIds <- paste( 'subarea', getSubareaIds(simulation), 'E', sep='.')
#' playInputs(simulation, dataSet, precipIds, rep('rain_obs', length(precipIds)))
#' playInputs(simulation, dataSet, evapIds, rep('pet_obs', length(evapIds)), 'daily_to_hourly')
#' }
#' @export
getEnsembleDataSet <- function(dataSetId='someId', dataPath='') {
    # tools::file_ext(dataSetId)
  if(file.exists(dataSetId)) {
    # if this is an RData file, load into this environment?
    # if this is a YAML data set, or something like that
    LoadEnsembleDataset_R(dataSetId, dataPath)
  } else {
    stop('getEnsembleDataSet is in a prototype stage and supports only YAML data set descriptors')
  }
}

#' Gets the data identifiers in a data library (data set)
#'
#' Gets the data identifiers in a data library (data set)
#'
#' @export
getDataSetIds <- function(dataLibrary) {
  GetEnsembleDatasetDataIdentifiers_R(dataLibrary)
}

#' @export
`as.xts.ExternalObjRef` <-
function(x, ...) {
  return(asXts(x))
}

#' getDatasetSingleTimeSeries
#' 
#' getDatasetSingleTimeSeries Wrapper function for GetDatasetSingleTimeSeries
#' 
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param dataId character, one data identifier for the time series.
#' @export
getDatasetSingleTimeSeries <- function(dataLibrary, dataId)
{
    result <- GetDatasetSingleTimeSeries_R(dataLibrary, dataId);
    return(result)
}

#' Gets a time series from a time provider, given a data ID
#'
#' Gets a time series from a time provider, given a data ID
#'
#' @param provider an external pointer to a SWIFT Time Series Provider.
#' @param dataId character, one or more data identifier for the time series. If missing, all known identifiers will be used; be careful about the resulting size.
#' @return an xts time series.
#' @export
getTimeSeriesFromProvider <- function(provider, dataId) {
  if(missing(dataId)) {
    dataId <- getDataIdentifiers(provider)
  }
  return(internalGetTimeSeriesFromProvider(provider, dataId))
}

#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider
#'
#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider
#'
#' @param provider an external pointer to a SWIFT Time Series Provider.
#' @param dataId character, a data identifier for the time series
#' @return a character vector
#' @export
getDataIdentifiers <- function(provider) {
  return(GetProviderTimeSeriesIdentifiers_R(provider))
}

# #' Extract a time series from an hourly SAK data set
# #'
# #' Extract a time series from an hourly SAK data set
# #'
# #' @param dataSet an external pointer to the the legacy SwiftDataSet object from the SAK, e.g. hourly rainfall
# #' @param index one-based index of the time series in the data set
# #' @return an hourly xts time series 
# getHourlyTs <- function(dataSet, index) {
#   mkTimeSeries(dataSet, index, mkSeriesFunctor(mkHourlySeries))
# }

# #' Extract a time series from an daily SAK data set
# #'
# #' Extract a time series from an daily SAK data set
# #'
# #' @param dataSet an external pointer to the the legacy SwiftDataSet object from the SAK, e.g. daily rainfall
# #' @param index one-based index of the time series in the data set
# #' @return an daily xts time series 
# getDailyTs <- function (dataSet, index) {
#   mkTimeSeries(dataSet, index, mkSeriesFunctor(mkDailySeries))
# }

# mkTimeSeries <- function(dataSet, index, fun) {
#   tSeriesInfo <- getTimeSeriesInfo(as.integer(index-1), dataSet)
#   x <- clrGet(tSeriesInfo, 'Data')
#   startTime <- clrGet(tSeriesInfo, "Start")
#   fun(startTime, x)
# }


#' @export
asXts <- function(tsInfo) {
  if(is.list(tsInfo)) {
    return(marshaledTimeSeriesToXts(tsInfo))
  } else if(isSwiftRef(tsInfo)) {
    if(isSingularTimeSeries(tsInfo)) {
      return(marshaledTimeSeriesToXts(TimeSeriesToTsInfo_Pkg_R(tsInfo)))
    } else if(isEnsembleTimeSeries(tsInfo)) {
      return(ensTsToXts(GetEnsembleTimeSeries_Pkg_R(tsInfo)))
    } else {
      stop(paste0('asXts: does not know how to convert to xts an object of SWIFT type "', tsInfo@type, '"'))
    }
  } else {
    k <- class(tsInfo)
    stop( paste0( 'cannot convert objects of this(these) class(es) to an xts time series: ', paste(k, collapse = ',')))
  }
}

marshaledTimeSeriesToXts <- function(tsInfo) {
  stopifnot(is.list(tsInfo))
  if (!setequal( c('Start','tzone','Data','TimeStep'), names(tsInfo) )) {
    stop('The list provided must have names: Start,tzone,Data,TimeStep')
  }
  s <- tsInfo[['Start']]
  if(!is(s, 'POSIXct')) {stop('the Start component of the list must be a POSIXct')}
  timeZone <- tsInfo[['tzone']]
  lubridate::tz(s) <- timeZone 
  values <- tsInfo[['Data']]
  tStep <- tsInfo[['TimeStep']]
  if(is.character(tStep)) {
    tsName <- tolower(tsInfo[['TimeStep']])
    if (tsName=='daily') {
      mkDailySeries(s, values, NULL)
    } else if (tsName=='hourly') {
      mkHourlySeries(s, values, NULL)
    } else {
      stop(paste('Unsupported time step specification:', tsName))
    }
  } else if (is.integer(tStep)){
    tStepDuration <- tStep
    if (tStepDuration==86400) {
      mkDailySeries(s, values, NULL)
    } else if (tStepDuration==3600) {
      mkHourlySeries(s, values, NULL)
    } else {
      stop(paste('Unsupported time step specification:', tStepDuration, ' seconds'))
    }
  } else {
    stop(paste('Unsupported time step specification:', tStep))
  }
}



