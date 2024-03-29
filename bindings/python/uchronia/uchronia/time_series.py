from typing import Any, Union, List, TYPE_CHECKING
if TYPE_CHECKING:
    from uchronia.classes import (
        EnsembleTimeSeries,
        TimeSeriesLibrary,
        EnsembleForecastTimeSeries,
        TimeSeries,
        EnsemblePtrTimeSeries,
        TimeSeriesProvider,
    )
    from uchronia.const import TsRetrievalSignature, VecStr, NdTimeSeries, ItemSliceNdTimeSeries

import pandas as pd
from refcount.interop import DeletableCffiNativeHandle
from uchronia.data_set import as_xarray
from uchronia.internals import (
    is_ensemble_time_series,
    is_singular_time_series,
    is_time_series_of_ensemble_time_series,
)
import uchronia.wrap.uchronia_wrap_generated as uwg
import uchronia.wrap.uchronia_wrap_custom as uwc



def mk_date(year, month, day, hour=0, min=0, sec=0, tz=None):
    """
    Creates a pandas Timestamp date/time object

    Creates a pandas Timestamp date/time object, with a default UTC time zone and zeroes as default time arguments (i.e. midnight)

    Args:
        year (int): integer
        month (int): integer
        day (int): integer
        hour (int): integer
        min (int): integer
        sec (int): numeric
        tz (str): character, A time zone specification to be used for the conversion. Defaults to UTC.

    Returns:
        a pandas Timestamp date/time object

    """
    return pd.Timestamp(
        year=year, month=month, day=day, hour=hour, minute=min, second=sec, tz=tz
    )


# #' Gets information about the dimension(s) of data
# #'
# #' Gets information about the dimension(s) of data
# #'
# #' @param x uchronia data such as TIME_SERIES_PTR,
# #'  ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.
# #' @export
# def geometryOf(x):
#     from cinterop.cffi.marshal import is
#     import cinterop
#     # if(is.list(x)):
#     #   return(marshaledTimeSeriesToXts(x))
#     if isinstance(x, xr.DataArray): # TODO: or?
#         return geometryFromXts(x)
#     elif(cinterop.(x)):
#         return(geometryFromInteropRegularTimeSeries(x))
#     elif(cinterop::isExternalObjRef(x)):
#         if(is_singular_time_series(x)):
#         geometryFromUnivTs(x)
#         } elif(is_ensemble_time_series(x)):
#         geometryFromMultivTs(x)
#         } elif(is_time_series_of_ensemble_time_series(x)):
#         geometryFromTsEnsTs(x)
#         } else {
#         stop(paste0('as_xarray: does not know how to convert to xts an object of external type "', x@type, '"'))
#         }
#     } else {
#         k = class(x)
#         stop( paste0( 'retrieving geometry of objects of this(these) class(es) not (yet) supported: ', paste(k, collapse = ',')))
#     }
#     }

# ensembleDimension(name="", size=1):
#   return(list(name=name, size=size))
# }

# timeDimension(name="", start, time_step, size):
#   return(list(start=start, time_step=time_step, size=size ))
# }

# geometryFromGeom(geom):
#   stopifnot(is(geom,'RegularTimeSeriesGeometry'))
#   return(list(start=geom@Start, time_step=lubridate::seconds(geom@TimeStepSeconds), size=geom@Length ))
# }

# geometryFromInteropRegularTimeSeries(x):
#   stopifnot(cinterop::isInteropRegularTimeSeries(x))
#   temporal=geometryFromGeom(x@TsGeom)
#   if(x@EnsembleSize > 1):
#     list(
#         ensemble=ensembleDimension(size=x@EnsembleSize),
#         temporal=temporal
#       )
#   } else {
#     temporal
#   }
# }

# geometryFromUnivTs(x):
#   d = timeIndex(x) # not super efficient
#   geometryFromTimeIndex(d)
# }

# geometryFromTimeIndex(d):
#   if(length(d)<2) stop("Limitation: cannot retrieve time step from time series of one element")
#   timeDimension(start=d[1], time_step=lubridate::as.duration(d[2]-d[1]), size=length(d))
# }

# geometryFromXts(x):
#   stopifnot(xts::is.xts(x))
#   timeStep = NA
#   if(nrow(x) > 1):
#     ind = xts:::index.xts(x[1:2])
#     timeStep = lubridate::as.duration(ind[2]-ind[1])
#   }
#   ensSize = ncol(x)
#   temporal<-timeDimension(
#     start=xts:::start.xts(x),
#     time_step=timeStep,
#     size=nrow(x))
#   if(ensSize>1){
#     list(
#       ensemble=ensembleDimension(size=ensSize),
#       temporal=temporal
#     )
#   } else {
#     return(temporal)
#   }
# }

# geometryFromMultivTs(x):
#   if(is_ensemble_time_series(x)):
#     # KLUDGE?
#     y = get_item(x,1, convert_to_xr=FALSE)
#     temporal = geometryFromTimeIndex(timeIndex(y))
#     ensSize = EnsembleSizeEnsembleTimeSeries_py(x)
#   } elif (is(x,"RegularTimeSeriesGeometry")):
#     temporal = geometryFromGeom(x)
#     ensSize = 1
#   } elif (is(x,"RegularTimeSeries")):
#     temporal = geometryFromGeom(x@TsGeom)
#     ensSize = x@EnsembleSize
#   } else {
#     k = class(x)
#     stop( paste0( 'Failed to retrieve temporal characteristics of objects of this(these) class(es) supported: ', paste(k, collapse = ',')))
#   }
#   list(
#     ensemble=ensembleDimension(size=ensSize),
#     temporal=temporal
#   )
# }

# geometryFromTsEnsTs(x):
#   temporal = geometryFromTimeIndex(timeIndex(x))
#   # KLUDGE? the above would have checked there is at least one item...
#   y = get_item(x,1, convert_to_xr=FALSE)
#   geom = geometryFromMultivTs(y)
#   list(
#     temporal=temporal,
#     ensemble=geom$ensemble,
#     temporal_lead=geom$temporal
#   )
# }

# #' Create a time series of ensemble of time series
# #'
# #' Create a time series of ensemble of time series (a.k.a. ensemble forecast time series)
# #'
# #' @param tsStartEns  data coercible to a POSIXct start date of the ensemble time series.
# #' @param n  integer. length of the ensemble time series
# #' @param timeStep  a character vector (numeric integers are 'tolerated' as seconds). Examples are '24:00:00', 'hourly', 'daily'.
# #' @examples
# #' \dontrun{
# #' n_days = 10
# #' tsStartDate = lubridate::origin
# #' ens_fc_ts = createEnsembleForecastTimeSeries(tsStartDate, n_days, 'daily')
# #' fcastsOffset = lubridate::hours(1)
# #' nLead = 5
# #' nEns = 4
# #' set.seed(465)
# #' x = matrix(rnorm(n=nEns*nLead), ncol=nEns)
# #' for (i in as.integer(1:n_days)):
# #'   multiTimeSeriesIn = mkHourlySeries(tsStartDate+ lubridate::hours(i) + fcastsOffset, (x+i-1), is.na)
# #'   uchronia::setItem(ens_fc_ts, i, multiTimeSeriesIn)
# #' }
# #' print(x)
# #' print(uchronia::get_item(ens_fc_ts, 1))
# #' }
# #' @export
# createEnsembleForecastTimeSeries(tsStartEns, n, timeStep='daily'):
#   tsStartEns = as.POSIXct(tsStartEns)
#   if(is.numeric(timeStep)):
#     timeStep = as.integer(timeStep)
#     timeStep = as.character(timeStep)
#   } elif (!is.character(timeStep)):
#     stop(paste('unhandled type for time step information: ', typeof(timeStep)))
#   }
#   CreateEnsembleForecastTimeSeries_py(tsStartEns, n, timeStep)
# }

# ensTsToXts(ensTs):  # RegularTimeSeries in cinterop
#   x = ensTs@NumericData
#   return(mkSeriesRegularTstep(ensTs@TsGeom@Start, x, isMissingFunc=is.na, ensTs@TsGeom@TimeStepSeconds))
# }

#' Gets an item in an indexable uchronia object
#'
#' Gets an item in an indexable uchronia object of one of the the C++ uchronia types identifed by TIME_SERIES_PTR,
#'  ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.
#'
#' @param ens_fc_ts  R type equivalent for C++ type TIME_SERIES_PTR,
#'  ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.
#' @param i	   integer. One-based index in the indexable object.
#' @examples
#' \dontrun{
#' ens_fc_ts = createEnsembleForecastTimeSeries(lubridate::origin, 3, 'daily')
#' fcastsOffset = lubridate::hours(1)
#' nLead = 5
#' nEns = 4
#' set.seed(465)
#' x = matrix(rnorm(n=nEns*nLead), ncol=nEns)
#' multiTimeSeriesIn = mkHourlySeries(lubridate::origin + fcastsOffset, x, is.na)
#' uchronia::setItem(ens_fc_ts, 1, multiTimeSeriesIn)
#' print(x)
#' print(uchronia::get_item(ens_fc_ts, 1))
#' }
#' @export

import xarray as xr

def get_item(ens_fc_ts: "NdTimeSeries", i, convert_to_xr=True) -> Union["ItemSliceNdTimeSeries", xr.DataArray]:
    """
    Gets an item in an indexable uchronia object

    Gets an item in an indexable uchronia object of one of the the C++ uchronia types identifed by TIME_SERIES_PTR,
    ENSEMBLE_PTR_TIME_SERIES_PTR, or ENSEMBLE_FORECAST_TIME_SERIES_PTR.

    Args:
        ens_fc_ts (NdTimeSeries): a uni- or multidimensional time series
        i (int): One-based index in the indexable object.
        convert_to_xr (bool): if True, (try to) convert to an xarray object

    Returns:
        Union["ItemSliceNdTimeSeries", xr.DataArray]: one item extracted from the series, so it has lost one dimension compared to the input.

    Examples:
        TODO

    """
    #    \dontrun{
    #    ensFcTs <- createEnsembleForecastTimeSeries(lubridate::origin, 3, 'daily')
    #    fcastsOffset <- lubridate::hours(1)
    #    nLead <- 5
    #    nEns <- 4
    #    set.seed(465)
    #    x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
    #    multiTimeSeriesIn <- mkHourlySeries(lubridate::origin + fcastsOffset, x, is.na)
    #    uchronia::setItem(ensFcTs, 1, multiTimeSeriesIn)
    #    print(x)
    #    print(uchronia::getItem(ensFcTs, 1))
    #    }
    if not isinstance(i, int):
        raise ValueError("Only numeric indices are supported for now")
    zero_index = i - 1
    if is_singular_time_series(ens_fc_ts):
        return uwg.GetValueFromUnivariateTimeSeries_py(ens_fc_ts, zero_index)
    elif is_ensemble_time_series(ens_fc_ts):
        univ_ts = uwg.TimeSeriesFromEnsembleOfTimeSeries_py(ens_fc_ts, zero_index)
        if convert_to_xr:
            univ_ts = as_xarray(univ_ts)
        return univ_ts
    elif is_time_series_of_ensemble_time_series(ens_fc_ts):
        if convert_to_xr:
            mts = uwg.GetItemEnsembleForecastTimeSeriesAsStructure_py(ens_fc_ts, zero_index)
        else:
            mts = uwg.GetItemEnsembleForecastTimeSeries_py(ens_fc_ts, zero_index)
        return mts
    else:
        raise ValueError(
            'get_item: does not know how to get from an object of external type "'
            + ens_fc_ts.type_id,
            '"',
        )


#' Sets an item of an ensemble time series
#'
#' Sets an item of an ensemble time series
#'
#' @param ens_fc_ts  R type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
#' @param i	   integer. One-based index in the ensemble forecast.
#' @param value an xts object to put the the specified index
#' @examples
#' \dontrun{
#' ens_fc_ts = createEnsembleForecastTimeSeries(lubridate::origin, 3, 'daily')
#' fcastsOffset = lubridate::hours(1)
#' nLead = 5
#' nEns = 4
#' set.seed(465)
#' x = matrix(rnorm(n=nEns*nLead), ncol=nEns)
#' multiTimeSeriesIn = mkHourlySeries(lubridate::origin + fcastsOffset, x, is.na)
#' uchronia::setItem(ens_fc_ts, 1, multiTimeSeriesIn)
#' print(x)
#' print(uchronia::get_item(ens_fc_ts, 1))
#' }
#' @export
def set_item(ens_fc_ts: "NdTimeSeries", i:int, value:Any):
    if not isinstance(i, int): raise ValueError("Only numeric indices are supported for now")
    zero_index = i-1
    if is_singular_time_series(ens_fc_ts):
        #   if(!is.numeric(value)) stop("For an univariate time series item set must be a scalar")
        uwg.SetValueToUnivariateTimeSeries_py(ens_fc_ts, zero_index, value)
    elif is_ensemble_time_series(ens_fc_ts):
        # if(!xts::is.xts(value)) stop("For an ensemble of time series the item set must be an xts")
        uwg.SetItemEnsembleTimeSeriesAsStructure_py(ens_fc_ts, zero_index, value)
    elif is_time_series_of_ensemble_time_series(ens_fc_ts):
        #   stopifnot(xts::is.xts(value))
        uwg.SetItemEnsembleForecastTimeSeriesAsStructure_py(ens_fc_ts, zero_index, value)
    else:
        raise ValueError(
            'set_item: does not know how to get from an object of external type "'
            + ens_fc_ts.type_id,
            '"',
        )

# #' Checks whether a data library has a given top level data identifier
# #'
# #' Checks whether a data library has a given top level data identifier
# #'
# #' @param data_library external pointer type ENSEMBLE_DATA_SET_PTR, or a Python class wrapper around it 
# #' @param identifier character, the identifier to test again
# #' @seealso \code{\link{get_ensemble_dataset}} for sample code
# #' @export
# hasIdentifier(data_library, identifier):
#   data_ids = get_dataset_ids(data_library)
#   return(identifier %in% data_ids)
# }


def sub_identifiers(data_library: "TimeSeriesLibrary", identifier:str) -> List[str]:
    """
    Gets the next level data identifier of a top level ID

    Gets the next level data identifier of a top level ID.
    A collection of time series such as one identified by "streamflows" may have
    sub-identifiers such as gauge numbers. A single time series in a data library
    may thus be retrieved by a hierarchical string ID  "streamflows.401221" 401221 is a gauge ID.

    Args:
        data_library (TimeSeriesLibrary): external pointer type ENSEMBLE_DATA_SET_PTR, or a Python class wrapper around it 
        identifier (str): character, the top level identifier to test again for next level ids

    Returns:
        List[str] sub-identifiers for a root identifier

    """
    return uwg.GetEnsembleDatasetDataSubIdentifiers_py(data_library, identifier)


# #' Make an hourly time series
# #'
# #' Make an hourly time series from a numeric vector and a start date.
# #'
# #' @param startDate the start date of the time series. It must be a POSIXct object.
# #' @param x the numeric vector.
# #' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
# #' @return an xts time series.
# #' @export
# #' @examples
# #' \dontrun{
# #' mkHourlySeries(lubridate::origin, -2:3/3.0)
# #' }
# mkHourlySeries(startDate, x, isMissingFunc= function(val){val < 0}):
#   mkSeriesRegularTstep(startDate, x, isMissingFunc=isMissingFunc, delta_t_sec=3600)
# }

# #' Make an daily time series
# #'
# #' Make an daily time series from a numeric vector and a start date.
# #'
# #' @param startDate the start date of the time series. It must be a POSIXct object.
# #' @param x the numeric vector.
# #' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
# #' @return an xts time series.
# #' @export
# #' @examples
# #' \dontrun{
# #' mkDailySeries(lubridate::origin, -2:3/3.0)
# #' }
# mkDailySeries(startDate, x, isMissingFunc = function(val):val < 0}):
#   mkSeriesRegularTstep(startDate, x, isMissingFunc=isMissingFunc, delta_t_sec=3600*24)
# }

# #' Make a time series with a regular time step
# #'
# #' Make a time series with a regular time step, meaning with the same duration between all steps
# #'
# #' @param startDate the start date of the time series. It must be a POSIXct object.
# #' @param x the numeric vector.
# #' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
# #' @param delta_t_sec the time step duration in seconds
# #' @return an xts time series.
# #' @examples
# #' \dontrun{
# #' # make a time series with a time step of 10 minutes
# #' mkSeriesRegularTstep(lubridate::origin, -2:3/3.0, delta_t_sec=600)
# #' }
# #' @export
# #' @importFrom xts xts
# mkSeriesRegularTstep(startDate, x, isMissingFunc= function(val){val < 0}, delta_t_sec=3600):
#   # POSIXct[1:1], format: "1999-12-31 12:00:00"
#   # [1] "1999-12-31 12:00:00 UTC"
#   stopifnot(is(startDate, "POSIXct"))
#   if(!is.null(isMissingFunc)):
#     isMissing = isMissingFunc(x)
#     x[isMissing] = NA
#   }
#   dimFunc = getDataDimensionFunction(x)
#   n = dimFunc(x)
#   xts::xts(x, createTimeSeriesIndex(startDate, n, delta_t_sec))
# }

# #' Make a time series with a monthly step
# #'
# #' Make a time series with a monthly step
# #'
# #' @param startDate the start date of the time series. It must be a POSIXct object.
# #' @param x the numeric vector.
# #' @param isMissingFunc a function, or NULL. The function should return a logical vector. Used to detect missing values in the raw data that are not already identified as NA
# #' @return an xts time series.
# #' @examples
# #' \dontrun{
# #' # make a time series with a time step of 10 minutes
# #' mkSeriesMonthlyTstep(lubridate::origin, -2:3/3.0)
# #' }
# #' @export
# #' @importFrom xts xts
# #' @importFrom cinterop makeMonthlyTimeAxis
# mkSeriesMonthlyTstep(startDate, x, isMissingFunc= function(val){val < 0}):
#   # POSIXct[1:1], format: "1999-12-31 12:00:00"
#   # [1] "1999-12-31 12:00:00 UTC"
#   stopifnot(is(startDate, "POSIXct"))
#   if(!is.null(isMissingFunc)):
#     isMissing = isMissingFunc(x)
#     x[isMissing] = NA
#   }
#   dimFunc = getDataDimensionFunction(x)
#   n = dimFunc(x)
#   tsIndex = cinterop::makeMonthlyTimeAxis(startDate, n)
#   xts::xts(x, tsIndex)
# }

# #' Make UTC time series in a serializable form
# #'
# #' Make UTC time series in a serializable form. Serializing xts objects directly leads to issues.
# #'
# #' @param series an xts time series with time indexing in UTC
# #' @return a list with integer form of UTC POSIXct date stamps ('utcInt'), series values ('x') as vector or matrix, and column names ('name') where name's value can be NULL.
# #' @examples
# #' \dontrun{
# #' # make a time series with a time step of 10 minutes
# #' x = mkSeriesRegularTstep(lubridate::origin, -2:3/3.0, delta_t_sec=600)
# #' serializableTs(x)
# #' }
# #' @export
# serializableTs(series):
#   d = zoo::index(series)
#   stopifnot(is(d, "POSIXct"))
#   stopifnot(attr(d, "tzone") == 'UTC')
#   xnames = names(series)
#   funcToNum = ifelse(ncol(series)>1, as.matrix, as.numeric)
#   list(
#     utcInt=as.integer(d),
#     x=funcToNum(series),
#     name=xnames
#   )
# }

# #' Deserialize information to a UTC time series
# #'
# #' Deserialize information to a UTC time series. This function is overcoming some behaviors in saving/loading xts series to/from binary RData format. Usage is not intended for most users. See \code{\link{serializableTs}}
# #'
# #' @param varData a list with integer form of UTC POSIXct date stamps ('utcInt'), and series values ('x') as vector or matrix, and column names ('name') where name's value can be NULL.
# #' @return an xts time series with UTC time indexing
# #' @export
# #' @importFrom xts xts
# deserializeSampleSeries(varData):
#   s = varData$utcInt
#   class(s) = c('POSIXct', 'POSIXt') # important to set both
#   attr(s, 'tzone') = 'UTC'
#   x = varData$x
#   if(!is.numeric(x)): stop(paste0('data is not a numeric mode: ', class(x)))}
#   dimFunc = getDataDimensionFunction(x)
#   if(dimFunc(x) != length(s)) stop('data and time indexing information have different length')
#   res = xts::xts(x, s)
#   if('name' %in% names(varData)):
#     names(res) = varData$name
#   }
#   res
# }

# getDataDimensionFunction(x):
#   if(is.matrix(x)):
#     dimFunc = nrow
#   } elif (is.vector(x)):
#     dimFunc = length
#   } else {
#     stop(paste0('unsupported data type for time series data values: ', class(x)))
#   }
#   return(dimFunc)
# }
