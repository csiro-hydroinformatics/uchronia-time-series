# the inclusion of the tests module is not meant to offer best practices for
# testing in general, but rather to support the `find_packages` example in
# setup.py that excludes installing the "tests" package

import os
import pandas as pd
import numpy as np
import datetime as dt
import sys
from datetime import datetime
import xarray as xr

# pkg_dir = os.path.join(os.path.dirname(__file__),'..')
# pkg_dir = "C:/src/csiro/stash/datatypes/bindings/python/uchronia"

# sys.path.append(pkg_dir)

from uchronia.wrap.ffi_interop import *
import uchronia.wrap.uchronia_wrap_generated as uwg

from cffi import FFI

# 2022-09 some of unit tests that used to be here were outdated. 
# Removing them. Now should all be covered by UT in the c-interop repo:
# /c-interop/bindings/python/cinterop/tests/test_native_handle.py

# def set_wrap_cffi_native_handle(wrapper_function:'WrapperCreationFunction'):
# def custom_wrap_cffi_native_handle(obj, type_id='', release_native = None):
# def charp_array_to_py(values:CffiData, size:int, dispose:bool=True) -> List[str]:
# def char_array_to_py(values:CffiData, dispose:bool=True) -> str:
# def named_values_to_py(values:CffiData, dispose:bool=True) -> Dict[str,float]:
# def opaque_ts_as_xarray_time_series(ptr:CffiData, dispose:bool=True) -> xr.DataArray:
# def py_time_series_dimensions_description(ptr:CffiData, dispose:bool=True) -> List[Tuple[str,int]]:
# def toSceParametersNative(x:dict) -> OwningCffiNativeHandle:


import uchronia.data_set as uds
import uchronia.sample_data as usd
import pytest

from cinterop.timeseries import create_ensemble_forecasts_series, create_daily_time_index, create_ensemble_series

_N_ENS=2
_N_LTIME=3
_N_TIME=4
_DT_ORIGIN = datetime(2000,1,1)

def mk_simple_efts():
    x = np.arange(_N_ENS*_N_LTIME*_N_TIME).reshape((_N_ENS, _N_LTIME, _N_TIME))
    ens_index = [str(i) for i in range(_N_ENS)]
    lead_time_index = list(range(1, _N_LTIME+1))
    time_index = create_daily_time_index(_DT_ORIGIN, _N_TIME)
    return create_ensemble_forecasts_series(x, ens_index, lead_time_index, time_index)

from cinterop.timeseries import ENSEMBLE_DIMNAME, TIME_DIMNAME

def mk_simple_ets(ens_dim_first:bool=True):
    x = np.arange(_N_ENS*_N_TIME).reshape((_N_ENS, _N_TIME))
    ens_index = [str(i) for i in range(_N_ENS)]
    time_index = create_daily_time_index(_DT_ORIGIN, _N_TIME)
    if ens_dim_first:
        return xr.DataArray(
            x, coords=[ens_index, time_index], dims=[ENSEMBLE_DIMNAME, TIME_DIMNAME]
        )
    else:
        return xr.DataArray(
            x.T, coords=[time_index, ens_index], dims=[TIME_DIMNAME, ENSEMBLE_DIMNAME]
        )


# def RegisterExceptionCallback_py(callback:Any) -> None:
def test_exception_handling():
    s = usd.sample_data_dir()
    data_library_file = os.path.join(s, 'time_series_library.yaml')
    d = uwg.LoadEnsembleDataset_py(data_library_file, "")
    pytest.raises(UchroniaError, d.get_dataset, "some_invalid_dataset_identifier")

# def GetLastStdExceptionMessage_py() -> str:
# def test_GetLastStdExceptionMessage_py():
#     assert uwg.GetLastStdExceptionMessage_py() == ""
# def DisposeSharedPointer_py(ptr:Any) -> None:
# def SetTimeSeriesMissingValueValue_py(missingValueValue:float) -> None:
def test_SetTimeSeriesMissingValueValue_py():
    from cinterop.timeseries import mk_daily_xarray_series, as_pydatetime
    ts = mk_daily_xarray_series(np.array([1.0, 2, 3, 4, 5]), "2000-01-01")
    native_ts = uwg.CreateSingleTimeSeriesDataFromStruct_py(ts)
    uwg.SetTimeSeriesMissingValueValue_py(3.0)
    # TODO: below too convoluted and not working. Simplify
    # efts = uwg.CreateEnsembleForecastTimeSeries_py(as_pydatetime("2000-01-01"), 2, "daily")
    # ensts = uwg.GetItemEnsembleForecastTimeSeries_py(efts, 0)
    # uwg.SetItemEnsembleTimeSeriesAsStructure_py(ensts, 0, ts)
    # py_ts = uwg.GetItemEnsembleTimeSeriesAsStructure_py(ensts, 0)
    # assert np.isna(py_ts.values[2])


# def LoadEnsembleDataset_py(libraryIdentifier:str, dataPath:str) -> 'TimeSeriesLibrary':
def test_LoadEnsembleDataset_py():
    s = usd.sample_data_dir()
    data_library_file = os.path.join(s, 'time_series_library.yaml')
    d = uwg.LoadEnsembleDataset_py(data_library_file, "")
    assert d is not None

from pathlib import Path

# def CreateEnsembleDataset_py(type:str) -> 'TimeSeriesLibrary':
def test_CreateEnsembleDataset_py():
    # 2022-09 very little supported for now, only "test_recording_library"
    # import tempfile
    # with tempfile.TemporaryDirectory() as tmpdirname:
    #     tmp = Path(tmpdirname)
    #     valid_subfolder = tmp / "path"
    #     valid_subfolder.mkdir(parents=True, exist_ok=True)
    d = uwg.CreateEnsembleDataset_py("test_recording_library")
    assert len(d.get_dataset_ids()) == 0

# def GetEnsembleDatasetDataIdentifiers_py(dataLibrary:'TimeSeriesLibrary'):
# def GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary:'TimeSeriesLibrary', dataCollectionId:str):
# def GetEnsembleDatasetDataSummaries_py(dataLibrary:'TimeSeriesLibrary'):
# def GetDataDimensionsDescription_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> List:
# def EnsembleSizeEnsembleTimeSeries_py(ensSeries:'EnsemblePtrTimeSeries') -> int:
# def DisposeDataDimensionsDescriptions_py(data:List) -> None:
# def CreateEnsembleForecastTimeSeries_py(start:datetime, length:int, timeStepName:str) -> 'EnsembleForecastTimeSeries':
# def GetDatasetSingleTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> 'TimeSeries':
# def GetDatasetEnsembleTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataEnsembleId:str) -> 'EnsemblePtrTimeSeries':
# def GetDatasetEnsembleForecastTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> 'EnsembleForecastTimeSeries':
# def SaveSingleTimeSeriesToNetcdf_py(timeSeries:'TimeSeries', filename:str, overwrite:bool) -> None:
# def SaveEnsembleTimeSeriesToNetcdf_py(collection:'EnsemblePtrTimeSeries', filename:str, overwrite:bool) -> None:
# def SaveEnsembleForecastTimeSeriesToNetcdf_py(tsEnsTs:'EnsembleForecastTimeSeries', filename:str, overwrite:bool) -> None:
# def IsMissingValueItemEnsembleForecastTimeSeries_py(series:'EnsembleForecastTimeSeries', i:int) -> bool:
# def GetItemEnsembleForecastTimeSeries_py(efts:'EnsembleForecastTimeSeries', i:int) -> 'EnsemblePtrTimeSeries':
# def TimeSeriesFromEnsembleOfTimeSeries_py(collectionTs:'EnsemblePtrTimeSeries', index:int) -> 'TimeSeries':
# def TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py(efts:'EnsembleForecastTimeSeries', indexInIssueTime:int, indexInForecastTime:int) -> 'TimeSeries':
# def GetValueFromUnivariateTimeSeries_py(ts:'TimeSeries', index:int) -> float:
# def TransformEachItem_py(tsEnsTs:'EnsembleForecastTimeSeries', method:str, methodArgument:str) -> None:
# def SetValueToUnivariateTimeSeries_py(ts:'TimeSeries', index:int, value:float) -> None:
# def GetItemEnsembleForecastTimeSeriesAsStructure_py(series:'EnsembleForecastTimeSeries', i:int) -> xr.DataArray:
# def GetItemEnsembleTimeSeriesAsStructure_py(series:'EnsemblePtrTimeSeries', i:int) -> xr.DataArray:
# def SetItemEnsembleForecastTimeSeriesAsStructure_py(series:'EnsembleForecastTimeSeries', i:int, values:xr.DataArray) -> None:
# def SetItemEnsembleTimeSeriesAsStructure_py(collection:'EnsemblePtrTimeSeries', i:int, values:xr.DataArray) -> None:
# def CreatePerfectForecastTimeSeries_py(observations:'TimeSeries', start:datetime, length:int, timeStepName:str, offsetForecasts:int, leadTime:int) -> 'EnsembleForecastTimeSeries':
# def ToStructEnsembleTimeSeriesData_py(ensSeries:'EnsemblePtrTimeSeries') -> xr.DataArray:
# def ToStructSingleTimeSeriesData_py(timeSeries:'TimeSeries') -> xr.DataArray:
# def CreateEnsembleTimeSeriesDataFromStruct_py(ensSeries:xr.DataArray) -> 'EnsemblePtrTimeSeries':
def test_CreateEnsembleTimeSeriesDataFromStruct_py():
    x = mk_simple_ets()
    eptr = uwg.CreateEnsembleTimeSeriesDataFromStruct_py(x)
    for e in range(_N_ENS):
        tseries = eptr[e]
        v = tseries.to_xarray().values
        assert np.all(x.values[e,:] == v.squeeze())

def test_as_uchronia_data():
    x = mk_simple_ets()
    eptr = uds.as_uchronia_data(x)
    for e in range(_N_ENS):
        tseries = eptr[e]
        v = tseries.to_xarray().values
        assert np.all(x.values[e,:] == v.squeeze())

def test_as_uchronia_data_timefirst():
    x = mk_simple_ets(ens_dim_first=False)
    eptr = uds.as_uchronia_data(x)
    for e in range(_N_ENS):
        tseries = eptr[e]
        v = tseries.to_xarray().values
        # data x is time-first, so to get the first ensemble:
        assert np.all(x.values[:,e] == v.squeeze())

# def CreateSingleTimeSeriesDataFromStruct_py(timeSeries:xr.DataArray) -> 'TimeSeries':
# def DisposeMultiTimeSeriesData_py(data:xr.DataArray) -> None:
# def GetTimeSeriesGeometry_py(timeSeries:'TimeSeries', geom:TimeSeriesGeometryNative) -> None:
# def GetEnsembleForecastTimeSeriesGeometry_py(timeSeries:'EnsembleForecastTimeSeries', geom:TimeSeriesGeometryNative) -> None:
# def GetTimeSeriesValues_py(timeSeries:'TimeSeries', values:np.ndarray, arrayLength:int) -> None:
# def GetNumTimeSeries_py() -> int:
# def GetProviderTsGeometry_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str, geom:TimeSeriesGeometryNative) -> None:
# def GetProviderTimeSeriesValues_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str, values:np.ndarray, arrayLength:int) -> None:
# def GetProviderTimeSeriesIdentifiers_py(dataLibrary:'TimeSeriesProvider'):
# def TimeSeriesFromProviderTs_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str) -> 'TimeSeries':


if __name__ == "__main__":
    test_CreateEnsembleDataset_py()

