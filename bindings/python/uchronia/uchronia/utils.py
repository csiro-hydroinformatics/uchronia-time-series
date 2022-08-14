from typing import Sequence
import xarray as xr
import pandas as pd


def xr_concat(
    series: Sequence[xr.DataArray],
    new_coord_names: Sequence[str],
    new_dim_name: str = None,
    units: str = None,
) -> xr.DataArray:
    """concatenate DataArrays - helper function

    Args:
        series (Sequence[xr.DataArray]): arrays to concatenate. These should have the same coordinates.
        new_coord_names (Sequence[str]): Names of the concatenated cases
        new_dim_name (str, optional): Name of the new dimension for concatenated cases. Defaults to None.
        units (str, optional): units. Defaults to None.

    Returns:
        xr.DataArray: concatenated arrays with one added dimension compared to inputs

    Examples:
        >>> before = mk_test_season_minmax(min_x=1.0, max_x=2.0, kharif=False, units='MAF')
        >>> after = before * 1.1
        >>> xr_concat([before, after], new_coord_names=['before','after'], new_dim_name='case', units='MAF')
        <xarray.DataArray (case: 2, minmax: 2, ten_day_id: 18)>
        array([[[1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. , 1. ,
                1. , 1. , 1. , 1. , 1. , 1. ],
                [2. , 2. , 2. , 2. , 2. , 2. , 2. , 2. , 2. , 2. , 2. , 2. ,
                2. , 2. , 2. , 2. , 2. , 2. ]],
            [[1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1,
                1.1, 1.1, 1.1, 1.1, 1.1, 1.1],
                [2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2, 2.2,
                2.2, 2.2, 2.2, 2.2, 2.2, 2.2]]])
        Coordinates:
        * minmax      (minmax) <U3 'min' 'max'
        * ten_day_id  (ten_day_id) int64 19 20 21 22 23 24 25 ... 30 31 32 33 34 35 36
        * case        (case) object 'before' 'after'
        Attributes:
            units:    MAF
    """
    res = xr.concat(series, dim=pd.Index(new_coord_names, name=new_dim_name))
    set_xr_units(res, units)
    return res


def set_xr_units(x: xr.DataArray, units: str):
    """Sets the units attribute of an xr.DataArray. No effect if x is not a dataarray

    Args:
        x (xr.DataArray): data array
        units (str): units descriptor
    """
    import cinterop

    cinterop.timeseries.set_xr_units(x, units)
