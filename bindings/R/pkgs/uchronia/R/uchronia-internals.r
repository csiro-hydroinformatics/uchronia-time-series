
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


# KLUDGE: date time fcts are duplicated with joki:: but to avoid dependency on too much for now:
makeTextDate <- function(instant, tz='UTC', txtformat=dateTimeIso8601Format()) {
##  #' @examples See \code{\link{makeTextTimeInterval}} for sample code.
  instant <- as.POSIXct(instant)
  format(instant,format=txtformat, tz = tz, usetz = TRUE)
}

#' @export
dateIso8601Format <- function() {
  "%Y-%m-%d"
}

#' @export
dateTimeIso8601Format <- function(toSeconds=TRUE, toMinutes=TRUE, toHours=TRUE) {
  if(all(c(toSeconds, toMinutes, toHours))) { 
    return("%Y-%m-%d %H:%M:%S")
  } else {
    if (toSeconds) return("%Y-%m-%d %H:%M:%S")
    if (toMinutes) return("%Y-%m-%d %H:%M")
    if (toHours)   return("%Y-%m-%d %H")
  }
  return(dateIso8601Format())
}

#' @import lubridate
basicTimeSeriesInfo <- function(header='SWIFT time series:', spanInfo, bnbt = '\n\t', newline = '\n') {
  return(paste0(header, 
  bnbt, makeTextDate(spanInfo@Start, tz=lubridate::tz(spanInfo@Start)) , 
  bnbt, 'time step ', lubridate::seconds(spanInfo@TimeStepSeconds), 
  bnbt, 'size ', spanInfo@Length, 
  newline 
  ))
}

#' @export
geometryOf <- function(x) {
  warning("geometryOf: not implemented")
}

#' @export
strDatatypeRef <- function(x, ...) {
  bnbt <- '\n\t'
  newline <- '\n'
  if (isSingularTimeSeries(x)) {
    s <- GetTimeSeriesGeometry_Pkg_R(x)
    cat(basicTimeSeriesInfo(header='time series:', spanInfo=s, bnbt=bnbt, newline=newline))
  } else if (isEnsembleTimeSeries(x)) {
    cat('collection of time series - TODO summary')
  } else if (isEnsembleForecastTimeSeries(x)) {
    s <- GetEnsembleForecastTimeSeriesGeometry_Pkg_R(x) 
    cat(basicTimeSeriesInfo(header='ensemble forecast time series:', spanInfo=s, bnbt=bnbt, newline=newline))
  } else {
    cinterop::strExternalObjRef(x)
  }
}
