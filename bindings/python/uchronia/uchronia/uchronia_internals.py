from refcount.interop import is_cffi_native_handle
import uchronia.wrap.uchronia_wrap_custom as uwc

from typing import Callable, List
import numpy as np
import xarray as xr
import pandas as pd


#' Is the object a 'uchronia' time series of ensembles of time series 
#' 
#' Is the object a 'uchronia' time series of ensembles of time series 
#' 
#' @param s an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer
#' @export
def is_time_series_of_ensemble_time_series (s):
    return is_cffi_native_handle(s, type_id='ENSEMBLE_FORECAST_TIME_SERIES_PTR')


#' Is the object a 'uchronia' ensemble of time series 
#' 
#' Is the object a 'uchronia' ensemble of time series 
#' 
#' @param s an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer
#' @export
def is_ensemble_time_series (s):
    return is_cffi_native_handle(s, type_id='ENSEMBLE_PTR_TIME_SERIES_PTR')


#' Is the object a 'uchronia' univariate time series 
#' 
#' Is the object a 'uchronia' univariate time series 
#' 
#' @param s an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer
#' @export
def is_singular_time_series (s):
    return is_cffi_native_handle(s, type_id='TIME_SERIES_PTR')

from refcount.interop import type_error_cffi

def check_singular_time_series (s):
    if not is_singular_time_series(s):
        raise ValueError(type_error_cffi(s, 'TIME_SERIES_PTR'))

def check_ensemble_time_series (s):
    if not is_ensemble_time_series(s):
        raise ValueError(type_error_cffi(s, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))

def check_ensemble_forecast_time_series (s):
    if not is_time_series_of_ensemble_time_series(s):
        raise ValueError(type_error_cffi(s, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))

def internal_get_time_series_from_provider (provider, data_ids):
    internal_get_multiple_time_series(provider, data_ids, uwc.GetTimeSeriesFromProvider_Pkg)

def _concatenate_series(t_series_list:List, new_coord_names:List, new_dim_name) -> xr.DataArray:
    res = xr.concat(t_series_list, dim=pd.Index(new_coord_names, name=new_dim_name))
    return res

def internal_get_multiple_time_series(simulation, var_ids, api_get_ts_func:Callable):
    if isinstance(var_ids, str):
        var_ids = [var_ids]
    if len(var_ids) == 0: 
        return None
    def f(var_id):
        return internal_get_single_model_time_series(simulation, var_id, api_get_ts_func)
    t_series_list = [f(var_id) for var_id in var_ids]
    # This function used to use do.call(merge,etc.) however tests done with xts_0.11-0 on R 3.4.4 showed that using a loop
    # is actually much faster. Completely unexpected but pretty clear. See a revisited https://jira.csiro.au/browse/WIRADA-147
    n = len(t_series_list)
    return _concatenate_series(t_series_list, var_ids, "variable_identifiers")


#' Internal only - Perform an action on a uchronia object that is expected to return a time series
#'
#' Perform an action on a uchronia object that is expected to return a time series. 
#'   This function is internal to the package, to prevent code duplication between 
#'   retrieval of time series from a time series provider, or played/recorded 
#'   time series into/out of a model simulation
#'
#' @param ts_provider an external pointer to a uchronia Time Series Provider or model simulation
#' @param var_id character, a data identifier for the time series
#' @param api_get_ts_func a function, that takes as arguments ts_provider and var_id, and returns a list as suitable for \code{\link{marshaledTimeSeriesToXts}}  
#' @return an xts time series.
def internal_get_single_model_time_series (ts_provider, var_id, api_get_ts_func:Callable):
    if not isinstance(var_id, str):
        raise ValueError('internal_get_single_model_time_series must work on a single variable identifier')
    time_series_info = api_get_ts_func(ts_provider, var_id)
    # return(marshaledTimeSeriesToXts(time_series_info))
    return time_series_info

# # KLUDGE: date time fcts are duplicated with joki:: but to avoid dependency on too much for now:
# makeTextDate (instant, tz='UTC', txtformat=dateTimeIso8601Format()):
# ##  #' @examples See \code{\link{makeTextTimeInterval}} for sample code.
#   instant <- as_timestamp(instant)
#   format(instant,format=txtformat, tz = tz, usetz = TRUE)
# }

# #' @export
# dateIso8601Format ():
#   "%Y-%m-%d"
# }

# #' @export
# dateTimeIso8601Format (toSeconds=TRUE, toMinutes=TRUE, toHours=TRUE):
#   if(all(c(toSeconds, toMinutes, toHours))) { 
#     return("%Y-%m-%d %H:%M:%S")
#   } else {
#     if (toSeconds) return("%Y-%m-%d %H:%M:%S")
#     if (toMinutes) return("%Y-%m-%d %H:%M")
#     if (toHours)   return("%Y-%m-%d %H")
#   }
#   return(dateIso8601Format())
# }

# #' @importFrom lubridate tz seconds
# basicTimeSeriesInfo (header='uchronia time series:', spanInfo, bnbt = '\n\t', newline = '\n'):
#   return(paste0(header, 
#   bnbt, makeTextDate(spanInfo@Start, tz=lubridate::tz(spanInfo@Start)) , 
#   bnbt, 'time step ', lubridate::seconds(spanInfo@TimeStepSeconds), 
#   bnbt, 'size ', spanInfo@Length, 
#   newline 
#   ))
# }

# #' @export
# strDatatypeRef (x, ...):
#   bnbt <- '\n\t'
#   newline <- '\n'
#   if (is_singular_time_series(x)):
#     s <- GetTimeSeriesGeometry_Pkg(x)
#     cat(basicTimeSeriesInfo(header='time series:', spanInfo=s, bnbt=bnbt, newline=newline))
#   } else if (is_ensemble_time_series(x)):
#     cat('collection of time series - TODO summary')
#   } else if (is_time_series_of_ensemble_time_series(x)):
#     s <- GetEnsembleForecastTimeSeriesGeometry_Pkg(x) 
#     cat(basicTimeSeriesInfo(header='ensemble forecast time series:', spanInfo=s, bnbt=bnbt, newline=newline))
#   } else {
#     cinterop::strExternalObjRef(x)
#   }
# }
