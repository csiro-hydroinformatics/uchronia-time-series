from collections.abc import Callable, Sequence
from typing import TYPE_CHECKING, TypeAlias

import xarray as xr

Scalar: TypeAlias = float | bool | int
"""A numeric or logical scalar"""
VecScalars: TypeAlias = Scalar | Sequence[Scalar]
"""One or more scalars: a Scalar, or a sequence thereof"""
VecStr: TypeAlias = str | Sequence[str]
"""One or more str: a str, or a sequence thereof"""
VecNum: TypeAlias = float | Sequence[float]
"""One or more float numbers: a float, or a sequence thereof"""


if TYPE_CHECKING:
    from uchronia.classes import EnsembleForecastTimeSeries, EnsembleTimeSeries, TimeSeries, TimeSeriesProvider

    TsRetrievalSignature: TypeAlias = Callable[[TimeSeriesProvider, str], xr.DataArray]
    """TsRetrievalSignature"""
    NdTimeSeries: TypeAlias = TimeSeries | EnsembleTimeSeries | EnsembleForecastTimeSeries
    """NdTimeSeries"""
    ItemSliceNdTimeSeries: TypeAlias = float | TimeSeries | EnsembleTimeSeries
    """ItemSliceNdTimeSeries"""
    VecNdTimeSeries: TypeAlias = NdTimeSeries | Sequence[NdTimeSeries]
    """VecNdTimeSeries"""
