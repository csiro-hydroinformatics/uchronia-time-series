#' Creates a POSIXct date/time object
#'
#' Creates a POSIXct date/time object, with a default UTC time zone and zeroes as default time arguments (i.e. midnight)
#'
#' @param year   integer
#' @param month  integer
#' @param day	   integer
#' @param hour   integer
#' @param min    integer
#' @param sec	   numeric
#' @param tz     character, A time zone specification to be used for the conversion. Defaults to UTC.
#' @export
mkDate <- function (year, month, day, hour = 0, min = 0, sec = 0, tz = "UTC")  { 
  ISOdate(year, month, day, hour, min, sec, tz=tz) 
}

#' Creates an ensemble time series 
#'
#' Creates an ensemble time series 
#'
#' @param tsStartEns  data coercible to a POSIXct start date of the ensemble time series.
#' @param n	   integer. length of the ensemble time series
#' @param timeStep  a character vector (numeric integers are 'tolerated' as seconds). Examples are '24:00:00', 'hourly', 'daily'.
#' @examples
#' \dontrun{
#' n_days <- 10  
#' tsStartDate <- lubridate::origin
#' ensFcTs <- createEnsembleForecastTimeSeries(tsStartDate, n_days, 'daily')
#' fcastsOffset <- lubridate::hours(1)
#' nLead <- 5
#' nEns <- 4
#' set.seed(465)
#' x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
#' for (i in as.integer(1:n_days)) {
#'   multiTimeSeriesIn <- mkHourlySeries(tsStartDate+ lubridate::hours(i) + fcastsOffset, (x+i-1), is.na)
#'   uchronia::setItem(ensFcTs, i, multiTimeSeriesIn)
#' }
#' print(x)
#' print(uchronia::getItem(ensFcTs, 1))
#' }
#' @export
createEnsembleForecastTimeSeries <- function (tsStartEns, n, timeStep='daily') {
  tsStartEns <- as.POSIXct(tsStartEns)
  if(is.numeric(timeStep)) {
    timeStep <- as.integer(timeStep)
    if(timeStep==3600) {timeStep <- 'hourly'
    } else if(timeStep==86400) {timeStep <- 'daily'
    }  else {stop(paste('unhandled time step: ', timeStep))}
  } else if (!is.character(timeStep)) {
    stop(paste('unhandled type for time step information: ', typeof(timeStep)))
  }
  CreateEnsembleForecastTimeSeries_R(tsStartEns, n, timeStep)
}

#' Sets an item of an ensemble time series 
#'
#' Sets an item of an ensemble time series 
#'
#' @param ensFcTs  R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param i	   integer. One-based index in the ensemble forecast.
#' @param value an xts object to put the the specified index
#' @examples
#' \dontrun{
#' ensFcTs <- createEnsembleForecastTimeSeries(lubridate::origin, 3, 'daily')
#' fcastsOffset <- lubridate::hours(1)
#' nLead <- 5
#' nEns <- 4
#' set.seed(465)
#' x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
#' multiTimeSeriesIn <- mkHourlySeries(lubridate::origin + fcastsOffset, x, is.na)
#' uchronia::setItem(ensFcTs, 1, multiTimeSeriesIn)
#' print(x)
#' print(uchronia::getItem(ensFcTs, 1))
#' }
#' @export
setItem <- function(ensFcTs, i, value) {
  if(!is.numeric(i)) stop("Only numeric indices are supported for now")
  zeroIndex <- as.integer(i-1)
  if(isSingularTimeSeries(ensFcTs)) {
    if(!is.numeric(value)) stop("For an univariate time series item set must be a scalar")
    SetValueToUnivariateTimeSeries_R(ensFcTs, zeroIndex, value)
  } else if(isEnsembleTimeSeries(ensFcTs)) {
    if(!xts::is.xts(value)) stop("For an ensemble of time series the item set must be an xts")
    SetItemEnsembleTimeSeriesAsStructure_R(ensFcTs, zeroIndex, cinterop::asInteropRegularTimeSeries(value))
  } else if(isEnsembleForecastTimeSeries(ensFcTs)) {
    stopifnot(xts::is.xts(value))
    mts <- cinterop::asInteropRegularTimeSeries(value)
    SetItemEnsembleForecastTimeSeries_Pkg_R(ensFcTs, as.integer(i-1), mts)
  } else {
    stop(paste0('getItem: does not know how to set indexed item into an object of external type "', ensFcTs@type, '"'))
  }

}

ensTsToXts <- function(ensTs) {  # RegularTimeSeries in cinterop
  x <- ensTs@NumericData
  return(mkSeriesRegularTstep(ensTs@TsGeom@Start, x, isMissingFunc=is.na, ensTs@TsGeom@TimeStepSeconds))
}

#' Gets an item of an ensemble time series 
#'
#' Gets an item of an ensemble time series 
#'
#' @param ensFcTs  R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param i	   integer. One-based index in the ensemble forecast.
#' @examples
#' \dontrun{
#' ensFcTs <- createEnsembleForecastTimeSeries(lubridate::origin, 3, 'daily')
#' fcastsOffset <- lubridate::hours(1)
#' nLead <- 5
#' nEns <- 4
#' set.seed(465)
#' x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
#' multiTimeSeriesIn <- mkHourlySeries(lubridate::origin + fcastsOffset, x, is.na)
#' uchronia::setItem(ensFcTs, 1, multiTimeSeriesIn)
#' print(x)
#' print(uchronia::getItem(ensFcTs, 1))
#' }
#' @export
getItem <- function(ensFcTs, i, convertToXts=TRUE) {
  if(!is.numeric(i)) stop("Only numeric indices are supported for now")
  zeroIndex <- as.integer(i-1)
  if(isSingularTimeSeries(ensFcTs)) {
    return(GetValueFromUnivariateTimeSeries_R(ensFcTs, zeroIndex))
  } else if(isEnsembleTimeSeries(ensFcTs)) {
    univTs <- TimeSeriesFromEnsembleOfTimeSeries_R(ensFcTs, zeroIndex)
    if(convertToXts) {univTs <- asXts(univTs)}
    return(univTs)
  } else if(isEnsembleForecastTimeSeries(ensFcTs)) {
    mts <- GetItemEnsembleForecastTimeSeries_Pkg_R(ensFcTs,zeroIndex)
    if(convertToXts) {mts <- asXts(mts)}
    return(mts)
  } else {
    stop(paste0('getItem: does not know how to get from an object of external type "', ensFcTs@type, '"'))
  }
}

#' @export
hasIdentifier <- function(dataLibrary, identifier) {
  dataIds <- getDataSetIds(dataLibrary)
  return(identifier %in% dataIds)
}

#' @export
subIdentifiers <- function(dataLibrary, identifier) {
  stationIds <- GetEnsembleDatasetDataSubIdentifiers_R(dataLibrary, identifier)
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


#' Apply a function to each row of a time series
#'
#' Apply a function to each row of a time series
#'
#' @param tSeries a time series of class 'xts'
#' @param func a function to apply to time series values for each time stamp. 
#' @examples
#' \dontrun{
#' n <- 15
#' myrand <- function(i) { i + 0.5*sqrt(i) * rnorm(100)}
#' set.seed(0)
#' d <- laply(seq_len(n), myrand)
#' ensTs <- xts(d, order.by=lubridate::origin + 0:(n-1) * 3600)
#' xtsApply(ensTs)
#' }
#' @import xts
#' @export
xtsApply <- function(tSeries, func=function(x){ quantile(x, probs = c(0.05, 0.5, 0.95)) } )
{
  stopifnot(is.xts(tSeries))
  xts(t(apply(tSeries, func, MARGIN=1)), order.by=zoo::index(tSeries))
}
