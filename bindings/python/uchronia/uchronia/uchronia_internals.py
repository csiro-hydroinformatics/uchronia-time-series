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
def isTimeSeriesOfEnsembleTimeSeries (s):
    return is_cffi_native_handle(s, type_id='ENSEMBLE_FORECAST_TIME_SERIES_PTR')


#' Is the object a 'uchronia' ensemble of time series 
#' 
#' Is the object a 'uchronia' ensemble of time series 
#' 
#' @param s an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer
#' @export
def isEnsembleTimeSeries (s):
    return is_cffi_native_handle(s, type_id='ENSEMBLE_PTR_TIME_SERIES_PTR')


#' Is the object a 'uchronia' univariate time series 
#' 
#' Is the object a 'uchronia' univariate time series 
#' 
#' @param s an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer
#' @export
def isSingularTimeSeries (s):
    return is_cffi_native_handle(s, type_id='TIME_SERIES_PTR')

from refcount.interop import type_error_cffi

def checkSingularTimeSeries (s):
    if not isSingularTimeSeries(s):
        raise ValueError(type_error_cffi(s, 'TIME_SERIES_PTR'))

def checkEnsembleTimeSeries (s):
    if not isEnsembleTimeSeries(s):
        raise ValueError(type_error_cffi(s, 'ENSEMBLE_PTR_TIME_SERIES_PTR'))

def checkEnsembleForecastTimeSeries (s):
    if not isTimeSeriesOfEnsembleTimeSeries(s):
        raise ValueError(type_error_cffi(s, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR'))

def internalGetTimeSeriesFromProvider (provider, dataIds):
    internalGetMultipleTimeSeries(provider, dataIds, uwc.GetTimeSeriesFromProvider_Pkg)



def _concatenate_series(t_series_list:List, new_coord_names:List, new_dim_name) -> xr.DataArray:
    res = xr.concat(t_series_list, dim=pd.Index(new_coord_names, name=new_dim_name))
    return res

def internalGetMultipleTimeSeries(simulation, varIds, apiGetTsFunc:Callable):
    if isinstance(varIds, str):
        varIds = [varIds]
    if len(varIds) == 0: 
        return None
    def f(varId):
        return internalGetSingleModelTts(simulation, varId, apiGetTsFunc)
    t_series_list = [f(varId) for varId in varIds]
    # This function used to use do.call(merge,etc.) however tests done with xts_0.11-0 on R 3.4.4 showed that using a loop
    # is actually much faster. Completely unexpected but pretty clear. See a revisited https://jira.csiro.au/browse/WIRADA-147
    n = len(t_series_list)
    return _concatenate_series(t_series_list, varIds, "variable_identifiers")


#' Internal only - Perform an action on a uchronia object that is expected to return a time series
#'
#' Perform an action on a uchronia object that is expected to return a time series. 
#'   This function is internal to the package, to prevent code duplication between 
#'   retrieval of time series from a time series provider, or played/recorded 
#'   time series into/out of a model simulation
#'
#' @param tsProvider an external pointer to a uchronia Time Series Provider or model simulation
#' @param varId character, a data identifier for the time series
#' @param apiGetTsFunc a function, that takes as arguments tsProvider and varId, and returns a list as suitable for \code{\link{marshaledTimeSeriesToXts}}  
#' @return an xts time series.
def internalGetSingleModelTts (tsProvider, varId, apiGetTsFunc:Callable):
    if not isinstance(varId, str):
        raise DataError('internalGetSingleModelTts must work on a single variable identifier')
    tsInfo = apiGetTsFunc(tsProvider, varId)
    # return(marshaledTimeSeriesToXts(tsInfo))
    return tsInfo

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
#   if (isSingularTimeSeries(x)):
#     s <- GetTimeSeriesGeometry_Pkg(x)
#     cat(basicTimeSeriesInfo(header='time series:', spanInfo=s, bnbt=bnbt, newline=newline))
#   } else if (isEnsembleTimeSeries(x)):
#     cat('collection of time series - TODO summary')
#   } else if (isTimeSeriesOfEnsembleTimeSeries(x)):
#     s <- GetEnsembleForecastTimeSeriesGeometry_Pkg(x) 
#     cat(basicTimeSeriesInfo(header='ensemble forecast time series:', spanInfo=s, bnbt=bnbt, newline=newline))
#   } else {
#     cinterop::strExternalObjRef(x)
#   }
# }
