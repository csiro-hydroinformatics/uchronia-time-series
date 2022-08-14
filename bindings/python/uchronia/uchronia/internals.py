from refcount.interop import is_cffi_native_handle
from uchronia.const import VecStr
import uchronia.wrap.uchronia_wrap_custom as uwc

from typing import Any, Callable, List, TYPE_CHECKING, Sequence
import numpy as np
import xarray as xr
import pandas as pd
from refcount.interop import type_error_cffi

if TYPE_CHECKING:
    from uchronia.classes import (
        TimeSeries,
        TimeSeriesProvider
    )
    from uchronia.const import TsRetrievalSignature, VecStr, NdTimeSeries, VecNdTimeSeries

def is_time_series_of_ensemble_time_series(s: Any):
    """
    Is the object a 'uchronia' time series of ensembles of time series

    Is the object a 'uchronia' time series of ensembles of time series

    Args:
        s (Any): an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer

    """
    return is_cffi_native_handle(s, type_id="ENSEMBLE_FORECAST_TIME_SERIES_PTR")


def is_ensemble_time_series(s: Any):
    """
    Is the object a 'uchronia' ensemble of time series

    Is the object a 'uchronia' ensemble of time series

    Args:
        s (Any): an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer

    """
    return is_cffi_native_handle(s, type_id="ENSEMBLE_PTR_TIME_SERIES_PTR")


def is_singular_time_series(s: Any):
    """
    Is the object a 'uchronia' univariate time series

    Is the object a 'uchronia' univariate time series

    Args:
        s (Any): an S4 object 'ExternalObjRef' [package "cinterop"] with external pointer

    """
    return is_cffi_native_handle(s, type_id="TIME_SERIES_PTR")


def check_singular_time_series(s: Any):
    if not is_singular_time_series(s):
        raise ValueError(type_error_cffi(s, "TIME_SERIES_PTR"))


def check_ensemble_time_series(s: Any):
    if not is_ensemble_time_series(s):
        raise ValueError(type_error_cffi(s, "ENSEMBLE_PTR_TIME_SERIES_PTR"))


def check_ensemble_forecast_time_series(s: Any):
    if not is_time_series_of_ensemble_time_series(s):
        raise ValueError(type_error_cffi(s, "ENSEMBLE_FORECAST_TIME_SERIES_PTR"))


def internal_get_time_series_from_provider(provider, data_ids):
    internal_get_multiple_time_series(
        provider, data_ids, uwc.get_time_series_data_from_provider
    )


def _concatenate_series(
    t_series_list: Sequence["NdTimeSeries"], new_coord_names: Sequence[str], new_dim_name:str
) -> xr.DataArray:
    res = xr.concat(t_series_list, dim=pd.Index(new_coord_names, name=new_dim_name))
    return res


def internal_get_multiple_time_series(
    simulation:"TimeSeriesProvider", var_ids: VecStr, api_get_ts_func: "TsRetrievalSignature"
) -> "VecNdTimeSeries":
    if isinstance(var_ids, str):
        var_ids = [var_ids]
    if len(var_ids) == 0:
        return None

    def f(var_id):
        return internal_get_single_model_time_series(
            simulation, var_id, api_get_ts_func
        )

    t_series_list = [f(var_id) for var_id in var_ids]
    return _concatenate_series(t_series_list, var_ids, "variable_identifiers")


def internal_get_single_model_time_series(
    ts_provider: "TimeSeriesProvider", var_id: str, api_get_ts_func: "TsRetrievalSignature"
) -> "NdTimeSeries":
    """
    Internal only - Perform an action on a uchronia object that is expected to return a time series

    Perform an action on a uchronia object that is expected to return a time series.
    This function is internal to the package, to prevent code duplication between
    retrieval of time series from a time series provider, or played/recorded
    time series into/out of a model simulation

    Args:
        ts_provider (TimeSeriesProvider): an external pointer to a uchronia Time Series Provider or model simulation
        var_id (str): character, a data identifier for the time series
        api_get_ts_func (TsRetrievalSignature): a function, that takes as arguments ts_provider and varId, and returns a list as suitable for marshaledTimeSeriesToXts

    Returns:
        NdTimeSeries: a uni- or multidimensional time series

    """
    if not isinstance(var_id, str):
        raise ValueError(
            "internal_get_single_model_time_series must work on a single variable identifier"
        )
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
