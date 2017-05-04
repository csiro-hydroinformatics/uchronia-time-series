#' @export
createEnsembleForecastTimeSeries <- function (tsStartEns, n, timeStep) {
  if(is.numeric(timeStep)) {
    timeStep <- as.integer(timeStep)
    if(timeStep==3600) {timeStep <- 'daily'
    } else if(timeStep==86400) {timeStep <- 'hourly'
    }  else {stop(paste('unhandled time step: ', timeStep))}
  } else if (!is.character(timeStep)) {
    stop(paste('unhandled type for time step information: ', typeof(timeStep)))
  }
  CreateEnsembleForecastTimeSeries_R(tsStartEns, n, timeStep)
}

#' @export
setItem <- function(ensFcTs, i, multiTimeSeries) {
  stopifnot(is.xts(multiTimeSeries))
  mts <- cinterop::asInteropRegularTimeSeries(multiTimeSeries)
  SetItemEnsembleForecastTimeSeries_Pkg_R(ensFcTs, i, mts)
}

ensTsToXts <- function(ensTs) {  # RegularTimeSeries in cinterop
  x <- ensTs@NumericData
  return(mkSeriesRegularTstep(ensTs@TsGeom@Start, x, isMissingFunc=is.na, ensTs@TsGeom@TimeStepSeconds))
}

#' @export
getItem <- function(ensFcTs, i) {
  return(ensTsToXts(GetItemEnsembleForecastTimeSeries_Pkg_R(ensFcTs, i)))
}

#' Make an hourly time series
#'
#' Make an hourly time series from a numeric vector and a start date.
#'
#' @param startDate the start date of the time series. It must be a POSIXct object.
#' @param x the numeric vector.
#' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
#' @return an xts time series. 
#' @export
#' @examples
#' \dontrun{
#' mkHourlySeries(lubridate::origin, -2:3/3.0)
#' }
#' @import xts
mkHourlySeries <- function(startDate, x, isMissingFunc= function(val){val < 0}) {
  mkSeriesRegularTstep(startDate, x, isMissingFunc=isMissingFunc, deltaTSec=3600)
}

#' Make an daily time series
#'
#' Make an daily time series from a numeric vector and a start date.
#'
#' @param startDate the start date of the time series. It must be a POSIXct object.
#' @param x the numeric vector.
#' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
#' @return an xts time series. 
#' @export
#' @examples
#' \dontrun{
#' mkDailySeries(lubridate::origin, -2:3/3.0)
#' }
#' @import xts
mkDailySeries <- function (startDate, x, isMissingFunc = function(val) {val < 0}) {
  mkSeriesRegularTstep(startDate, x, isMissingFunc=isMissingFunc, deltaTSec=3600*24)
}

#' Make a time series with a regular time step
#'
#' Make a time series with a regular time step, meaning with the same duration between all steps
#'
#' @param startDate the start date of the time series. It must be a POSIXct object.
#' @param x the numeric vector.
#' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
#' @param deltaTSec the time step duration in seconds
#' @return an xts time series. 
#' @examples
#' \dontrun{
#' # make a time series with a time step of 10 minutes
#' mkSeriesRegularTstep(lubridate::origin, -2:3/3.0, deltaTSec=600)
#' }
#' @export
#' @import xts
mkSeriesRegularTstep <- function(startDate, x, isMissingFunc= function(val){val < 0}, deltaTSec=3600) {
  # POSIXct[1:1], format: "1999-12-31 12:00:00"
  # [1] "1999-12-31 12:00:00 UTC"
  stopifnot(is(startDate, "POSIXct"))
  if(!is.null(isMissingFunc)) {
    isMissing <- isMissingFunc(x)
    x[isMissing] <- NA
  }
  dimFunc <- getDataDimensionFunction(x)
  n <- dimFunc(x)
  xts(x, startDate+(0:(n-1))*deltaTSec)
}

#' Make UTC time series in a serializable form
#'
#' Make UTC time series in a serializable form. Serializing xts objects directly leads to issues.
#'
#' @param series an xts time series with time indexing in UTC
#' @return a list with integer form of UTC POSIXct date stamps ('utcInt'), series values ('x') as vector or matrix, and column names ('name') where name's value can be NULL.
#' @examples
#' \dontrun{
#' # make a time series with a time step of 10 minutes
#' x <- mkSeriesRegularTstep(lubridate::origin, -2:3/3.0, deltaTSec=600)
#' serializableTs(x)
#' }
#' @export
serializableTs <- function(series) {
  d <- zoo::index(series)
  stopifnot(is(d, "POSIXct"))
  stopifnot(attr(d, "tzone") == 'UTC')
  xnames <- names(series)
  funcToNum <- ifelse(ncol(series)>1, as.matrix, as.numeric)
  list(
    utcInt=as.integer(d),
    x=funcToNum(series),
    name=xnames
  )
}

#' Deserialize information to a UTC time series
#'
#' Deserialize information to a UTC time series. This function is overcoming some behaviors in saving/loading xts series to/from binary RData format. Usage is not intended for most users. See \code{\link{serializableTs}}
#'
#' @param varData a list with integer form of UTC POSIXct date stamps ('utcInt'), and series values ('x') as vector or matrix, and column names ('name') where name's value can be NULL.
#' @return an xts time series with UTC time indexing
#' @export
#' @import xts
deserializeSampleSeries <- function(varData) {
  s <- varData$utcInt
  class(s) <- c('POSIXct', 'POSIXt') # important to set both
  attr(s, 'tzone') <- 'UTC'
  x <- varData$x
  if(!is.numeric(x))  { stop(paste0('data is not a numeric mode: ', class(x)))}  
  dimFunc <- getDataDimensionFunction(x)
  if(dimFunc(x) != length(s)) stop('data and time indexing information have different length')
  res <- xts(x, s)
  if('name' %in% names(varData)) {
    names(res) <- varData$name
  }
  res
}
  
getDataDimensionFunction <- function(x) {
  if(is.matrix(x))  {
    dimFunc <- nrow
  } else if (is.vector(x)) {
    dimFunc <- length
  } else {
    stop(paste0('unsupported data type for time series data values: ', class(x)))
  }
  return(dimFunc)
}

