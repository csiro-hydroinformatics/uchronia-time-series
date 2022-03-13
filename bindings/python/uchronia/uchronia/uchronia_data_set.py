from multiprocessing.sharedctypes import Value
from refcount.interop import is_cffi_native_handle
from uchronia.uchronia_internals import internal_get_time_series_from_provider
import uchronia.wrap.uchronia_wrap_generated as uwg
import uchronia.wrap.uchronia_wrap_custom as uwc
import os

#' Gets one or more time series from a time series provider
#'
#' Gets one or more time series from a time series provider. This function is exported for use by R packages rather than for end users. 
#'
#' @param ts_provider R wrapper around an object coercible to a TIME_SERIES_PROVIDER_PTR
#' @param var_ids character vector, IDs of the time series to retrieve from the provider
#' @param api_get_ts_func an R function that takes as arguments 'ts_provider' and a character.
#' @return an xts time series
#' @examples
#' \dontrun{
#' internalGetRecordedTts(simulation, var_ids):
#'   uchronia::get_multiple_time_series_from_provider(simulation, var_ids, get_recorded_pkg_R)
#' }
#' }
#' @export
def get_multiple_time_series_from_provider(ts_provider, var_ids, api_get_ts_func):
    from uchronia.uchronia_internals import internal_get_multiple_time_series
    return internal_get_multiple_time_series(ts_provider, var_ids, api_get_ts_func)

#' Gets an object, a library to access a set of time series
#'
#' Gets an object, a library to access a set of time series
#'
#' @param dataset_id currently only a path to a file in YAML is supported.
#' @return an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer type ENSEMBLE_DATA_SET_PTR
#' @examples
#' \dontrun{
#'  d = uchronia::sample_data_dir()
#'  yamlFn = file.path(d, 'time_series_library.yaml')
#'  if(!file.exists(yamlFn)):stop(paste0('sample YAML file ', yamlFn, ' not found')) }
#'  file.show(yamlFn)
#'  dataSet = uchronia::get_ensemble_dataset(yamlFn)
#'  (data_ids = uchronia::get_dataset_ids(dataSet))
#'  subIdentifiers(dataSet, "var1_obs_collection")
#'  (univTs = uchronia::get_dataset(dataSet, "var1_obs"))
#'  (multivTs = uchronia::get_dataset(dataSet, "var1_obs_collection"))
#'  (ensFcast = uchronia::get_dataset(dataSet, "pet_fcast_ens"))
#'  plot(as_xarray(univTs))
#'  zoo::plot.zoo(as_xarray(multivTs))
#'  ## precip_ids = paste( 'subarea', get_subarea_ids(simulation), 'P', sep='.')
#'  ## evapIds = paste( 'subarea', get_subarea_ids(simulation), 'E', sep='.')
#'  ## swift::playInputs(simulation, dataSet, precip_ids, rep('rain_obs', length(precip_ids)))
#'  ## swift::playInputs(simulation, dataSet, evapIds, rep('pet_obs', length(evapIds)), 'daily_to_hourly')
#' }
#' @export
def get_ensemble_dataset(dataset_id='someId', data_path=''):
    # tools::file_ext(dataset_id)
    if os.path.exists(dataset_id):
        # if this is an RData file, load into this environment?
        # if this is a YAML data set, or something like that
        return uwg.LoadEnsembleDataset_py(dataset_id, data_path)
    else:
        raise FileNotFoundError('get_ensemble_dataset is in a prototype stage and supports only YAML data set descriptors')

#' Gets the top level data identifiers in a data library (data set)
#'
#' Gets the top level data identifiers in a data library (data set)
#'
#' @param data_library R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR, a.k.a a "time series library"
#'
#' @export
def get_dataset_ids(data_library):
    return uwg.GetEnsembleDatasetDataIdentifiers_py(data_library)

def datasets_summaries(data_library):
    return uwg.GetEnsembleDatasetDataSummaries_py(data_library)

# #' @export
# #' @importFrom xts as.xts
# `as.xts.ExternalObjRef` <-
# function(x, ...):
#   return(as_xarray(x))
# }

# queryDataGeometry(data_library, data_id)
# {
# }

#' Retrieve data from a data sets library
#' 
#' Gets the data from a library for a given identifier.
#' 
#' @param data_library R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR, a.k.a a "time series library"
#' @param data_id character, one data identifier for the data retrieved.
#' @return R type equivalent for one of the C++ types TIME_SERIES_PTR, 
#'  ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.
#'  A data identifier may thus point to an univariate time series, 
#'  a multivariate time series, or a time series of ensemble of (uni- or multi- variate) time series.
#' @seealso the vignettes illustrate how to use time serie libraries
#' 
#' @export
def get_dataset(data_library, data_id):
  result = uwc.GetDatasetFromLibrary_Pkg(data_library, data_id)
  return result


#' Retrieve data from a data sets library
#' 
#' Gets the data from a library for a given identifier.
#' 
#' @param data_library R type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
#' @param data_id character, one data identifier for the time series.
#' @export
def get_dataset_single_time_series(data_library, data_id):
    result = uwg.GetDatasetSingleTimeSeries_py(data_library, data_id)
    return result

#' Gets a time series from a time provider, given a data ID
#'
#' Gets a time series from a time provider, given a data ID.
#'  This means that the argument is a wrapper around an external pointer to a object whose type is 
#'  inheriting from the C++ type datatypes::timeseries::TimeSeriesProvider<double>
#'
#' @param provider an external pointer to a uchronia Time Series Provider.
#' @param data_id character, one or more data identifier for the time series. If missing, all known identifiers will be used be careful about the resulting size.
#' @return an xts time series.
#' @export
def get_time_series_from_provider(provider, data_id = None):
    if data_id is None:
        data_id = get_data_identifiers(provider)
    return internal_get_time_series_from_provider(provider, data_id)

#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider
#'
#' Gets the known time series identifiers (e.g. Gauge names) of a time series provider.
#'  This means that the argument is a wrapper around an external pointer to a object whose type is 
#'  inheriting from the C++ type datatypes::timeseries::TimeSeriesProvider<double>
#'
#' @param provider an external pointer to a uchronia Time Series Provider.
#' @return a character vector
#' @export
def get_data_identifiers(provider):
      return uwg.GetProviderTimeSeriesIdentifiers_py(provider)

# # mkTimeSeries(dataSet, index, fun):
# #   tSeriesInfo = getTimeSeriesInfo(as.integer(index-1), dataSet)
# #   x = clrGet(tSeriesInfo, 'Data')
# #   startTime = clrGet(tSeriesInfo, "Start")
# #   fun(startTime, x)
# # }


#' Coerce an object to an xts time series
#'
#' Converts if possible an object to an xts time series. Suitable objects are an equivalent 'uchronia' C++ entity 
#'  via an external pointer. Typically deals with time series and ensemble thereof, but may be expanded later on to support more types.
#'
#' @param time_series_info A representation of a time series. Supported types are external pointers as data from uchronia C API, or an R list returned by some of the *_R functions.
#' @return an xts object
#' @seealso \code{\link{asUchroniaData}} for a round-trip sample code
#' @export

from uchronia.uchronia_internals import is_singular_time_series, is_ensemble_time_series
import xarray as xr

def as_xarray(time_series_info) -> xr.DataArray:
    # if isinstance(time_series_info, dict):
    #     return marshaledTimeSeriesToXts(time_series_info)
    # if cinterop::isInteropRegularTimeSeries(time_series_info)):
    #     return(cinterop::as_xarrayTimeSeries(time_series_info))
    if is_cffi_native_handle(time_series_info):
        if is_singular_time_series(time_series_info):
            return uwg.ToStructSingleTimeSeriesData_py(time_series_info)
        elif is_ensemble_time_series(time_series_info):
            return uwg.ToStructEnsembleTimeSeriesData_py(time_series_info)
        else:
            raise ValueError('as_xarray: does not know how to convert to xarray an object of external type "' + time_series_info.type_id + '"')
    else:
        k = type(time_series_info)
        raise ValueError('cannot convert objects of type ' + str(k) + ' to an xarray time series')

# toUchroniaSeries(time_series_info):
#   if(time_series_info@EnsembleSize==1):
#     return(CreateSingleTimeSeriesDataFromStruct_py(time_series_info))
#   else:
#     return(CreateEnsembleTimeSeriesDataFromStruct_py(time_series_info))
#   }
# }

# #' Coerce an object to a c++ object handled via an external pointer
# #'
# #' Converts an object such as an xts time series to an equivalent 'uchronia' C++ entity that it then refered to 
# #' via an external pointer. Typically deals with time series and ensemble thereof, but may be expanded later on to support more types.
# #'
# #' @param rData An R object such as an xts object.
# #' @return an S4 object 'ExternalObjRef' [package "cinterop"]
# #' @examples
# #' \dontrun{
# #' len = 3
# #' ensSize = 2
# #' set.seed(1)
# #' x = matrix(rnorm(n=len*ensSize), ncol=ensSize)
# #' ind = lubridate::origin + (1:len) * lubridate::days(1)
# #' (mts = xts::xts(x, ind))
# #' (umts = asUchroniaData(mts))
# #' as_xarray(umts)
# #' }
# #' @importFrom cinterop isExternalObjRef
# #' @export
# asUchroniaData(rData):
#   if(xts::is.xts(rData)):
#     return(toUchroniaSeries(cinterop::asInteropRegularTimeSeries(rData)))
#   elif (cinterop::isExternalObjRef(rData)):
#     return(rData)
#   else:
#     k = class(rData)
#     stop( paste0( 'cannot convert objects of this(these) class(es) to an uchronia data set (xptr): ', paste(k, collapse = ',')))
#   }
# }

# createTimeSeriesIndex(startDate, n, delta_t_sec=as.numeric(3600)):
#   stopifnot(is(startDate, "POSIXct"))
#   # we need to make sure we work with numeric values, not integers, here, otherwise 
#   # we will quickly bump into integer overflow. Ouch. Compare:
#   # len = 29618
#   # tail(0:(len - 1) * tssec)
#   # tail(0:(len - 1) * as.numeric(tssec))
#   return(startDate+(0:(n-1))*as.numeric(delta_t_sec))
# }


# tsIndex(geom):
#   stopifnot(is(geom, "RegularTimeSeriesGeometry"))
#   len = geom@Length
#   tssec = geom@TimeStepSeconds
#   createTimeSeriesIndex(geom@Start, len, as.numeric(tssec))
# }

# #' Get the time index of a time series
# #'
# #' Get the time index of a time series
# #'
# #' @param time_series_info A representation of a time series. Supported types are external pointers as data from uchronia C API, or an R list returned by some of the *_R functions.
# #' @return a vector of POSIXct
# #' @export
# timeIndex(time_series_info):
#   if(is.list(time_series_info)):
#     stop("timeIndex on a list - not yet supported")
#     # return(SomethingTappingIntoCInterop???(time_series_info))
#   elif (cinterop::isExternalObjRef(time_series_info)):
#     if(is_singular_time_series(time_series_info)):
#       return(tsIndex(GetTimeSeriesGeometry_Pkg(time_series_info)))
#     elif (is_ensemble_time_series(time_series_info)):
#       stop(paste0('timeIndex: getting a time index not (yet?) supported for an object of external type "', time_series_info.type_id, '"'))
#     elif (is_time_series_of_ensemble_time_series(time_series_info)):
#       return(tsIndex(GetEnsembleForecastTimeSeriesGeometry_Pkg(time_series_info)))
#     else:
#       stop(paste0('timeIndex: does not know how to get a time index out of an object of external type "', time_series_info.type_id, '"'))
#     }
#   else:
#     k = class(time_series_info)
#     stop( paste0( 'cannot retrieve time indexing for objects of this(these) class(es): ', paste(k, collapse = ',')))
#   }
# }

# marshaledTimeSeriesToXts(time_series_info):
#   # converts series definitions created with make_time_series_info in:
#   # rcpp-interop-commons\include\cinterop\rcpp_timeseries_interop.hpp
#   stopifnot(is.list(time_series_info))
#   if (!setequal( c('Start','tzone','Data','TimeStep','TimeStepCode'), names(time_series_info) )):
#     stop(paste('The list provided must have names: Start,tzone,Data,TimeStep,TimeStepCode but got:', paste(names(time_series_info), collapse=',')))
#   }
#   s = time_series_info[['Start']]
#   if(!is(s, 'POSIXct')):stop('the Start component of the list must be a POSIXct')}
#   timeZone = time_series_info[['tzone']]
#   # I am discontinuing the use of lubridate::ts
#   # as it is possibly a root cause of https://jira.csiro.au/browse/WIRADA-504 
#   # as of lubridate version 1.7.2
#   # lubridate::tz(s) = timeZone 
#   # instead using:
#   attr(s, 'tzone') = timeZone
#   values = time_series_info[['Data']]
#   tStep = time_series_info[['TimeStep']]
#   tStepCode = time_series_info[['TimeStepCode']]
#   if(tStepCode == 0L):
#     if(is.character(tStep)):
#       tsName = tolower(time_series_info[['TimeStep']])
#       if (tsName=='daily'):
#         mkDailySeries(s, values, NULL)
#       elif  (tsName=='hourly'):
#         mkHourlySeries(s, values, NULL)
#       else:
#         tStepDuration = as.integer(tsName)
#         if(!is.na(tStepDuration))
#         {
#           return(mkSeriesRegularTstep(s, values, NULL, delta_t_sec=tStepDuration))
#         }
#         stop(paste('Unsupported time step specification:', tsName))
#       }
#     elif  (is.integer(tStep)){
#       tStepDuration = tStep
#       mkSeriesRegularTstep(s, values, NULL, delta_t_sec=tStepDuration)
#     else:
#       stop(paste('Unsupported time step specification:', tStep))
#     }
#   elif (tStepCode == 1L):
#     return(mkSeriesMonthlyTstep(s, values, NULL))
#   else:
#     stop(paste('Unsupported time step code specification:', tStepCode))
#   }

# }



