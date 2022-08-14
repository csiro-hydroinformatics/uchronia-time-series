
from typing import Callable, Sequence, Union, TYPE_CHECKING

from typing_extensions import TypeAlias 
import xarray as xr

Scalar: TypeAlias = Union[float, bool, int]
"""A numeric or logical scalar"""
VecScalars: TypeAlias = Union[Scalar, Sequence[Scalar]]
"""One or more scalars: a Scalar, or a sequence thereof"""
VecStr: TypeAlias = Union[str, Sequence[str]]
"""One or more str: a str, or a sequence thereof"""
VecNum: TypeAlias = Union[float, Sequence[float]]
"""One or more float numbers: a float, or a sequence thereof"""


if TYPE_CHECKING:
    from uchronia.classes import (
        TimeSeriesProvider,TimeSeries, EnsembleTimeSeries, EnsembleForecastTimeSeries
    )

    TsRetrievalSignature: TypeAlias = Callable[[TimeSeriesProvider,str],xr.DataArray]
    """TsRetrievalSignature"""
    NdTimeSeries: TypeAlias = Union[TimeSeries, EnsembleTimeSeries, EnsembleForecastTimeSeries]
    """NdTimeSeries"""
    ItemSliceNdTimeSeries: TypeAlias = Union[float, TimeSeries, EnsembleTimeSeries]
    """ItemSliceNdTimeSeries"""
    VecNdTimeSeries: TypeAlias = Union[NdTimeSeries, Sequence[NdTimeSeries]]
    """VecNdTimeSeries"""
