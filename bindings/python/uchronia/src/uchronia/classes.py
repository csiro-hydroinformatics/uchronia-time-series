"""Pythonic classes accessing underlying C++ objects functionalities."""

from collections.abc import Callable
from typing import TYPE_CHECKING, Any, Optional

from cffi import FFI
from refcount.interop import CffiData, CffiWrapperFactory, DeletableCffiNativeHandle

if TYPE_CHECKING:
    from uchronia.const import NdTimeSeries

from datetime import datetime, timedelta

import pandas as pd

import uchronia.data_set as uds
import uchronia.time_series as uts
import uchronia.wrap.uchronia_wrap_custom as uwc
import uchronia.wrap.uchronia_wrap_generated as uwg


class TimeSeriesProvider(DeletableCffiNativeHandle):
    """TimeSeriesProvider is an interface for objects that can provide time series.

    Under construction...

    """

    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: Optional[str] = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )


class TimeSeriesMixin:
    """Mixin interface for time series objects of various dimensionality."""

    def __init__(self):
        super().__init__()

    # def get_item(self, i: int, convert_to_xr=True):
    #     return uts.get_item(self, i, convert_to_xr)

    def as_xarray(self):
        return uts.as_xarray(self)



class EnsembleForecastTimeSeries(DeletableCffiNativeHandle, TimeSeriesMixin):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )

    @staticmethod
    def new(start: datetime, length: int, time_step: str | timedelta) -> "EnsembleForecastTimeSeries":
        if isinstance(time_step, timedelta):
            time_step = str(pd.Timedelta(time_step)).split(" ")[-1]
        return uwg.CreateEnsembleForecastTimeSeries_py(start, length, timeStepName=time_step)

    def __getitem__(self, key):
        return uwg.GetItemEnsembleForecastTimeSeriesAsStructure_py(self, key)

    def __setitem__(self, key, newvalue):
        uwg.SetItemEnsembleForecastTimeSeriesAsStructure_py(self, key, newvalue)

    def to_xarray(self):
        return None

    def time_index(self):
        geom = uwc.GetEnsembleForecastTimeSeriesGeometry_Pkg(self)
        return geom.time_index()


class EnsembleTimeSeries(DeletableCffiNativeHandle, TimeSeriesMixin):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )


class TimeSeries(DeletableCffiNativeHandle, TimeSeriesMixin):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )

    def __getitem__(self, key):
        return uwg.GetValueFromUnivariateTimeSeries_py(self, key)

    def __setitem__(self, key, newvalue):
        uwg.SetValueToUnivariateTimeSeries_py(self, key, newvalue)

    def to_xarray(self):
        return uwg.ToStructSingleTimeSeriesData_py(self)


class EnsemblePtrTimeSeries(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )

    def __getitem__(self, key):
        if isinstance(key, int):
            return uwg.TimeSeriesFromEnsembleOfTimeSeries_py(self, key)
        if isinstance(key, tuple):
            i, j = key
            # TODO: inefficient
            return uwg.TimeSeriesFromEnsembleOfTimeSeries_py(self, i)[j]
        raise KeyError("Invalid type of key")

    def __setitem__(self, key, newvalue):
        uwg.SetItemEnsembleTimeSeriesAsStructure_py(self, key, newvalue)

    def to_xarray(self):
        return uwg.ToStructEnsembleTimeSeriesData_py(self)

    # def geometry(self):
    #     return uwc.GetEnsembleForecastTimeSeriesGeometry_Pkg(self)


class TimeSeriesLibrary(TimeSeriesProvider):
    """Object that acts as a catalogue with various time series available for retrieval

    Under construction...

    """

    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super().__init__(
            handle,
            release_native,
            type_id,
            prior_ref_count,
        )

    def get_dataset_ids(self) -> list[str]:
        """Gets the top level data identifiers in a data library (data set)

        Returns:
            List[str]: identifiers for the datasets in this library
        """
        return uds.get_dataset_ids(self)

    def get_dataset(self, data_id: str) -> "NdTimeSeries":
        """Gets the data from a library for a given identifier.

        Args:
            data_id (str): character, one data identifier for the data retrieved.

        Returns:
            NdTimeSeries: a uni- or multidimensional time series
        """
        return uds.get_dataset(self, data_id)

    def sub_identifiers(self, identifier: str) -> list[str]:
        """Gets the next level data identifier of a top level ID

        Gets the next level data identifier of a top level ID.
        A collection of time series such as one identified by "streamflows" may have
        sub-identifiers such as gauge numbers. A single time series in a data library
        may thus be retrieved by a hierarchical string ID  "streamflows.401221" 401221 is a gauge ID.

        Args:
            identifier (str): character, the top level identifier to test again for next level ids

        Returns:
            List[str] sub-identifiers for a root identifier
        """
        return uts.sub_identifiers(self, identifier)

    def datasets_summaries(self) -> dict[str, str]:
        """Get the summaries of datasets in a library

        Returns:
            Dict[str,str]: For each top data ID, short description the corresponding the dataset.
        """
        return uds.datasets_summaries(self)


_api_type_wrapper = {
    # "DATATYPES_TIME_SERIES_DOUBLE_PTR": None,
    "DATATYPES_ENSEMBLE_TIME_SERIES_DOUBLE_PTR": EnsembleTimeSeries,
    "ENSEMBLE_DATA_SET_PTR": TimeSeriesLibrary,
    "ENSEMBLE_FORECAST_TIME_SERIES_PTR": EnsembleForecastTimeSeries,
    "TIME_SERIES_PTR": TimeSeries,
    # "ENSEMBLE_TIME_SERIES_PTR ": None,
    "ENSEMBLE_PTR_TIME_SERIES_PTR": EnsemblePtrTimeSeries,
    "TIME_SERIES_PROVIDER_PTR": TimeSeriesProvider,
    # "TS_GEOMETRY_PTR": None,
    # "DATE_TIME_INFO_PTR": None,
    # "MULTI_REGULAR_SERIES_STRUCT_PTR": None,
}

_STRICT_WRAPPING = False

_wrapper_factory = CffiWrapperFactory(_api_type_wrapper, _STRICT_WRAPPING)


def _create_wrapper(obj: Any, type_id: str, release_native: Callable):
    return _wrapper_factory.create_wrapper(obj, type_id, release_native)


def wrap_cffi_native_handle(obj: Any, type_id: str, release_native: Callable):
    if not isinstance(obj, FFI.CData):
        return obj
    return _create_wrapper(obj, type_id, release_native)


uwg.set_wrap_cffi_native_handle(wrapper_function=wrap_cffi_native_handle)
