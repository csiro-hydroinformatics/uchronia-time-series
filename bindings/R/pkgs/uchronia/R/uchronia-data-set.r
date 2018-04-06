#' Gets one or more time series from a time series provider
#'
#' Gets one or more time series from a time series provider. This function is exported for use by R packages rather than for end users. 
#'
#' @param tsProvider R wrapper around an object coercible to a TIME_SERIES_PROVIDER_PTR
#' @param varIds character vector, IDs of the time series to retrieve from the provider
#' @param apiGetTsFunc an R function that takes as arguments 'tsProvider' and a character.
#' @return an xts time series
#' @examples
#' \dontrun{
#' internalGetRecordedTts <- function(simulation, varIds) {
#'   uchronia::getMultipleTimeSeriesFromProvider(simulation, varIds, GetRecorded_Pkg_R)
#' }
#' }
#' @export
getMultipleTimeSeriesFromProvider <- function(tsProvider, varIds, apiGetTsFunc) {
  internalGetMultipleTimeSeries(tsProvider, varIds, apiGetTsFunc)
}

#' Gets an object, a library to access a set of time series
#'
#' Gets an object, a library to access a set of time series
#'
#' @param dataSetId currently only a path to a file in YAML is supported.
#' @return an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer type ENSEMBLE_DATA_SET_PTR
#' @examples
#' \dontrun{
#'  d <- uchronia::sampleDataDir()
#'  yamlFn <- file.path(d, 'time_series_library.yaml')
#'  if(!file.exists(yamlFn)) {stop(paste0('sample YAML file ', yamlFn, ' not found')) }
#'  file.show(yamlFn)
#'  dataSet <- uchronia::getEnsembleDataSet(yamlFn)
#'  (dataIds <- uchronia::getDataSetIds(dataSet))
#'  subIdentifiers(dataSet, "var1_obs_collection")
#'  (univTs <- uchronia::getDataSet(dataSet, "var1_obs"))
#'  (multivTs <- uchronia::getDataSet(dataSet, "var1_obs_collection"))
#'  (ensFcast <- uchronia::getDataSet(dataSet, "pet_fcast_ens"))
#'  plot(asXts(univTs))
#'  zoo::plot.zoo(asXts(multivTs))
#'  ## precipIds <- paste( 'subarea', getSubareaIds(simulation), 'P', sep='.')
#'  ## evapIds <- paste( 'subarea', getSubareaIds(simulation), 'E', sep='.')
#'  ## swift::playInputs(simulation, dataSet, precipIds, rep('rain_obs', length(precipIds)))
#'  ## swift::playInputs(simulation, dataSet, evapIds, rep('pet_obs', length(evapIds)), 'daily_to_hourly')
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

#' Gets the top level data identifiers in a data library (data set)
#'
#' Gets the top level data identifiers in a data library (data set)
#'
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR, a.k.a a "time series library"
#'
#' @export
getDataSetIds <- function(dataLibrary) {
  GetEnsembleDatasetDataIdentifiers_R(dataLibrary)
}

#' @export
#' @importFrom xts as.xts
`as.xts.ExternalObjRef` <-
function(x, ...) {
  return(asXts(x))
}

queryDataGeometry <- function(dataLibrary, dataId)
{
}

#' Retrieve data from a data sets library
#' 
#' Gets the data from a library for a given identifier.
#' 
#' @param dataLibrary R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR, a.k.a a "time series library"
#' @param dataId character, one data identifier for the data retrieved.
#' @return R type equivalent for one of the C++ types TIME_SERIES_PTR, 
#'  ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.
#'  A data identifier may thus point to an univariate time series, 
#'  a multivariate time series, or a time series of ensemble of (uni- or multi- variate) time series.
#' @seealso the vignettes illustrate how to use time serie libraries
#' 
#' @export
getDataSet <- function(dataLibrary, dataId)
{
  result <- GetDatasetFromLibrary_Pkg_R(dataLibrary, dataId);
  return(result)
}


#' Retrieve data from a data sets library
#' 
#' Gets the data from a library for a given identifier.
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
#' Gets a time series from a time provider, given a data ID.
#'  This means that the argument is a wrapper around an external pointer to a object whose type is 
#'  inheriting from the C++ type datatypes::timeseries::TimeSeriesProvider<double>
#'
#' @param provider an external pointer to a uchronia Time Series Provider.
#' @param dataId character, one or more data identifier for the time series. If missing, all known identifiers will be used; be careful about the resulting size.
#' @return an xts time series.
#' @export
getTimeSeriesFromProvider <- function(provider, dataId) {
  if(missing(dataId)) {
    dataId <- uchronia::getDataIdentifiers(provider)
  }
  return(internalGetTimeSeriesFromProvider(provider, dataId))
}

#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider
#'
#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider.
#'  This means that the argument is a wrapper around an external pointer to a object whose type is 
#'  inheriting from the C++ type datatypes::timeseries::TimeSeriesProvider<double>
#'
#' @param provider an external pointer to a uchronia Time Series Provider.
#' @return a character vector
#' @export
getDataIdentifiers <- function(provider) {
  return(GetProviderTimeSeriesIdentifiers_R(provider))
}

# mkTimeSeries <- function(dataSet, index, fun) {
#   tSeriesInfo <- getTimeSeriesInfo(as.integer(index-1), dataSet)
#   x <- clrGet(tSeriesInfo, 'Data')
#   startTime <- clrGet(tSeriesInfo, "Start")
#   fun(startTime, x)
# }


#' Coerce an object to an xts time series
#'
#' Converts if possible an object to an xts time series. Suitable objects are an equivalent 'uchronia' C++ entity 
#'  via an external pointer. Typically deals with time series and ensemble thereof, but may be expanded later on to support more types.
#'
#' @param tsInfo A representation of a time series. Supported types are external pointers as data from uchronia C API, or an R list returned by some of the *_R functions.
#' @return an xts object
#' @seealso \code{\link{asUchroniaData}} for a round-trip sample code
#' @export
asXts <- function(tsInfo) {
  if(is.list(tsInfo)) {
    return(marshaledTimeSeriesToXts(tsInfo))
  } else if(cinterop::isInteropRegularTimeSeries(tsInfo)) {
    return(cinterop::asXtsTimeSeries(tsInfo))
  } else if(cinterop::isExternalObjRef(tsInfo)) {
    if(isSingularTimeSeries(tsInfo)) {
      return(marshaledTimeSeriesToXts(TimeSeriesToTsInfo_Pkg_R(tsInfo)))
    } else if(isEnsembleTimeSeries(tsInfo)) {
      return(ensTsToXts(GetEnsembleTimeSeries_Pkg_R(tsInfo)))
    } else {
      stop(paste0('asXts: does not know how to convert to xts an object of external type "', tsInfo@type, '"'))
    }
  } else {
    k <- class(tsInfo)
    stop( paste0( 'cannot convert objects of this(these) class(es) to an xts time series: ', paste(k, collapse = ',')))
  }
}

toUchroniaSeries <- function(tsInfo) {
  if(tsInfo@EnsembleSize==1) {
    return(CreateSingleTimeSeriesDataFromStruct_R(tsInfo))
  } else {
    return(CreateEnsembleTimeSeriesDataFromStruct_R(tsInfo))
  }
}

#' Coerce an object to a c++ object handled via an external pointer
#'
#' Converts an object such as an xts time series to an equivalent 'uchronia' C++ entity that it then refered to 
#' via an external pointer. Typically deals with time series and ensemble thereof, but may be expanded later on to support more types.
#'
#' @param rData An R object such as an xts object.
#' @return an S4 object 'ExternalObjRef' [package "cinterop"]
#' @examples
#' \dontrun{
#' len <- 3
#' ensSize <- 2
#' set.seed(1)
#' x <- matrix(rnorm(n=len*ensSize), ncol=ensSize)
#' ind <- lubridate::origin + (1:len) * lubridate::days(1)
#' (mts <- xts::xts(x, ind))
#' (umts <- asUchroniaData(mts))
#' asXts(umts)
#' }
#' @importFrom cinterop isExternalObjRef
#' @export
asUchroniaData <- function(rData) {
  if(xts::is.xts(rData)) {
    return(toUchroniaSeries(cinterop::asInteropRegularTimeSeries(rData)))
  } else if(cinterop::isExternalObjRef(rData)) {
    return(rData)
  } else {
    k <- class(rData)
    stop( paste0( 'cannot convert objects of this(these) class(es) to an uchronia data set (xptr): ', paste(k, collapse = ',')))
  }
}

createTimeSeriesIndex <- function(startDate, n, deltaTSec=as.numeric(3600)) {
  stopifnot(is(startDate, "POSIXct"))
  # we need to make sure we work with numeric values, not integers, here, otherwise 
  # we will quickly bump into integer overflow. Ouch. Compare:
  # len <- 29618
  # tail(0:(len - 1) * tssec)
  # tail(0:(len - 1) * as.numeric(tssec))
  return(startDate+(0:(n-1))*as.numeric(deltaTSec))
}


tsIndex <- function(geom) {
  stopifnot(is(geom, "RegularTimeSeriesGeometry"))
  len <- geom@Length
  tssec <- geom@TimeStepSeconds
  createTimeSeriesIndex(geom@Start, len, as.numeric(tssec))
}

#' Get the time index of a time series
#'
#' Get the time index of a time series
#'
#' @param tsInfo A representation of a time series. Supported types are external pointers as data from uchronia C API, or an R list returned by some of the *_R functions.
#' @return a vector of POSIXct
#' @export
timeIndex <- function(tsInfo) {
  if(is.list(tsInfo)) {
    stop("timeIndex on a list - not yet supported")
    # return(SomethingTappingIntoCInterop???(tsInfo))
  } else if(cinterop::isExternalObjRef(tsInfo)) {
    if(isSingularTimeSeries(tsInfo)) {
      return(tsIndex(GetTimeSeriesGeometry_Pkg_R(tsInfo)))
    } else if(isEnsembleTimeSeries(tsInfo)) {
      stop(paste0('timeIndex: getting a time index not (yet?) supported for an object of external type "', tsInfo@type, '"'))
    } else if(isTimeSeriesOfEnsembleTimeSeries(tsInfo)) {
      return(tsIndex(GetEnsembleForecastTimeSeriesGeometry_Pkg_R(tsInfo)))
    } else {
      stop(paste0('timeIndex: does not know how to get a time index out of an object of external type "', tsInfo@type, '"'))
    }
  } else {
    k <- class(tsInfo)
    stop( paste0( 'cannot retrieve time indexing for objects of this(these) class(es): ', paste(k, collapse = ',')))
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
  # I am discontinuing the use of lubridate::ts
  # as it is possibly a root cause of https://jira.csiro.au/browse/WIRADA-504 
  # as of lubridate version 1.7.2
  # lubridate::tz(s) <- timeZone 
  # instead using:
  attr(s, 'tzone') <- timeZone
  values <- tsInfo[['Data']]
  tStep <- tsInfo[['TimeStep']]
  if(is.character(tStep)) {
    tsName <- tolower(tsInfo[['TimeStep']])
    if (tsName=='daily') {
      mkDailySeries(s, values, NULL)
    } else if (tsName=='hourly') {
      mkHourlySeries(s, values, NULL)
    } else {
      tStepDuration <- as.integer(tsName)
      if(!is.na(tStepDuration))
      {
        return(mkSeriesRegularTstep(s, values, NULL, deltaTSec=tStepDuration))
      }
      stop(paste('Unsupported time step specification:', tsName))
    }
  } else if (is.integer(tStep)){
    tStepDuration <- tStep
    mkSeriesRegularTstep(s, values, NULL, deltaTSec=tStepDuration)
  } else {
    stop(paste('Unsupported time step specification:', tStep))
  }
}



