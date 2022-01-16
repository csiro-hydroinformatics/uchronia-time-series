from typing import Any, Callable, Dict, List, OrderedDict, Sequence
from cffi import FFI
from refcount.interop import DeletableCffiNativeHandle, CffiData, CffiWrapperFactory
from cinterop.cffi.marshal import ConvertibleToTimestamp

import uchronia.wrap.uchronia_wrap_generated as uwg

class TimeSeriesProvider(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(TimeSeriesProvider, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )

class EnsembleForecastTimeSeries(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(EnsembleForecastTimeSeries, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )


class EnsembleTimeSeries(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(EnsembleTimeSeries, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )


class TimeSeriesLibrary(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(TimeSeriesLibrary, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )


class TimeSeries(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(TimeSeries, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )


class EnsemblePtrTimeSeries(DeletableCffiNativeHandle):
    def __init__(
        self,
        handle: CffiData,
        release_native: Callable[[CffiData], None],
        type_id: str = None,
        prior_ref_count: int = 0,
    ):
        super(EnsemblePtrTimeSeries, self).__init__(
            handle, release_native, type_id, prior_ref_count
        )

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