
isEnsembleForecastTimeSeries <- function(series) {
  return(cinterop::isExternalObjRef(series, type='ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

isEnsembleTimeSeries <- function(s) {
  return(cinterop::isExternalObjRef(s, type='ENSEMBLE_PTR_TIME_SERIES_PTR'))
}

isSingularTimeSeries <- function(s) {
  return(cinterop::isExternalObjRef(s, type='TIME_SERIES_PTR'))
}

checkSingularTimeSeries <- function(s) {
  if(!isSingularTimeSeries(s)) stop(cinterop::argErrorExternalObjType(s, 'TIME_SERIES_PTR'))
}

checkEnsembleTimeSeries <- function(s) {
  if(!isEnsembleTimeSeries(s)) stop(cinterop::argErrorExternalObjType(s, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))
}

checkEnsembleForecastTimeSeries <- function(s) {
  if(!isEnsembleForecastTimeSeries(s)) stop(cinterop::argErrorExternalObjType(s, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))
}

internalGetTimeSeriesFromProvider <- function(provider, dataIds) {
  internalGetMultipleTimeSeries(provider, dataIds, GetTimeSeriesFromProvider_Pkg_R)
}

internalGetMultipleTimeSeries <- function(simulation, varIds, apiGetTsFunc) {
  if(length(varIds)==0) return(NULL)
  tSeriesList <- lapply(varIds, function(varId) {internalGetSingleModelTts(simulation, varId, apiGetTsFunc) })
  result <- do.call(merge, tSeriesList)
  names(result) <- varIds
  result
}


#' Perform an action on a SWIFT object that is expected to return a time series
#'
#' Perform an action on a SWIFT object that is expected to return a time series. 
#'   This function is internal to the package, to prevent code duplication between 
#'   retrieval of time series from a time series provider, or played/recorded 
#'   time series into/out of a model simulation
#'
#' @param tsProvider an external pointer to a SWIFT Time Series Provider or model simulation
#' @param varId character, a data identifier for the time series
#' @param apiGetTsFunc a function, that takes as arguments tsProvider and varId, and returns a list as suitable for \code{\link{marshaledTimeSeriesToXts}}  
#' @return an xts time series.
internalGetSingleModelTts <- function(tsProvider, varId, apiGetTsFunc) {
  if(length(varId) != 1) stop('internalGetSingleModelTts must work on varId of length one')
  tsInfo <- apiGetTsFunc(tsProvider, varId)
  return(marshaledTimeSeriesToXts(tsInfo))
}

#' @import joki
#' @import lubridate
basicTimeSeriesInfo <- function(header='SWIFT time series:', spanInfo, bnbt = '\n\t', newline = '\n') {
  return(paste0(header, 
  bnbt, joki::makeTextDate(spanInfo@Start, tz=lubridate::tz(spanInfo@Start)) , 
  bnbt, 'time step ', lubridate::seconds(spanInfo@TimeStepSeconds), 
  bnbt, 'size ', spanInfo@Length, 
  newline 
  ))
}

#' @export
strDatatypeRef <- function(x, ...) {
  bnbt <- '\n\t'
  newline <- '\n'
  if (isSingularTimeSeries(x)) {
    s <- GetTimeSeriesGeometry_Pkg(x)
    cat(basicTimeSeriesInfo(header='time series:', spanInfo=s, bnbt=bnbt, newline=newline))
  } else if (isEnsembleForecastTimeSeries(x)) {
    s <- GetEnsembleForecastTimeSeriesGeometry_Pkg_R(x) 
    cat(basicTimeSeriesInfo(header='ensemble forecast time series:', spanInfo=s, bnbt=bnbt, newline=newline))
  # } else if (isDatatypeRef(x)) {
  #   cat(paste0('SWIFT object of type "', x@type ,'"\n'))
  } else {
    cinterop::strExternalObjRef(x)
  }
}
