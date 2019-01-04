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

pkg_dir = os.path.join(os.path.dirname(__file__),'..')
# pkg_dir = "C:/src/csiro/stash/datatypes/bindings/python/uchronia"

sys.path.append(pkg_dir)

from uchronia.wrap.ffi_interop import *
import uchronia.wrap.uchronia_wrap_generated
setattr(sys.modules['uchronia.wrap.uchronia_wrap_generated'], 'uchronia_so', uchronia_so)
from uchronia.wrap.uchronia_wrap_generated import *

from cffi import FFI

def test_struct_datetime():
    mdt = MarshaledDateTime(2001,1,2,3,4,5)
    assert mdt.year == 2001
    assert mdt.month == 1
    assert mdt.day == 2
    assert mdt.hour == 3
    assert mdt.minute == 4
    assert mdt.second == 5
    del mdt

def test_struct_tsgeom():
    mdt = MarshaledDateTime(2001,1,2,3,4,5)
    geom = MarshaledTsGeometry(mdt, b'D', 365)
    s = geom.start
    del s
    del mdt

def test_struct_ensemble_series():
    mdt = MarshaledDateTime(2001,1,2,3,4,5)
    geom = MarshaledTsGeometry(mdt, b'D', 365)

    values = np.zeros((365, 1))
    np_values = np.ascontiguousarray(values.T, dtype=np.float64)
    values_c = uchronia_ffi.new("double* [{dim}]".format(dim=values.shape[1]))
    for idx in range(values.shape[1]):
        values_c[idx] = uchronia_ffi.cast("double *", np_values[idx].ctypes.data)

    mtsd = MultiTimeSeriesData(geom, values.shape[1], values_c)
    print(mtsd.ensemble_size)
    str(mtsd.numeric_data)

    del geom
    del mtsd

def create_struct_ensemble_series(n=365, n_ens=2, t_step=b'D'):
    mdt = MarshaledDateTime(2001,1,2,3,4,5)
    geom = MarshaledTsGeometry(mdt, t_step, n)

    values = np.zeros((n, 1))
    np_values = np.ascontiguousarray(values.T, dtype=np.float64)
    values_c = uchronia_ffi.new("double* [{dim}]".format(dim=values.shape[1]))
    for idx in range(values.shape[1]):
        values_c[idx] = uchronia_ffi.cast("double *", np_values[idx].ctypes.data)

    mtsd = MultiTimeSeriesData(geom, values.shape[1], values_c)
    return mtsd



def create_ensemble_series(n=365, n_ens=2, t_step=b'D'):
    data = np.random.rand(n, n_ens)
    locs = [str(i) for i in range(n_ens)]
    times = pd.date_range('2001-01-02', periods=n, freq='D')
    foo = xr.DataArray(data, coords=[times, locs], dims=['time', 'ens'])
    return foo

def to_multi_regular_time_series_data(x):
    if not isinstance(x, xr.DataArray):
        raise Exception("not supported")
    # x = xr.DataArray(data, coords=[times, locs], dims=['time', 'ens'])
    n = x.shape[0]
    n_ens = x.shape[1]
    start = x.coords.get('time').to_index()[0]
    t_step = b'D'
    mdt = MarshaledDateTime(start.year, start.month, start.day, start.hour, start.minute,start.second)
    geom = MarshaledTsGeometry(mdt, t_step, n)
    values = x.values
    np_values = np.ascontiguousarray(values.T, dtype=np.float64)
    values_c = uchronia_ffi.new("double* [{dim}]".format(dim=values.shape[1]))
    for idx in range(values.shape[1]):
        values_c[idx] = uchronia_ffi.cast("double *", np_values[idx].ctypes.data)

    mtsd = MultiTimeSeriesData(geom, values.shape[1], values_c)
    return mtsd


def test_create_series():
    tss = create_struct_ensemble_series()
    timeSeries = CreateSingleTimeSeriesDataFromStruct_py(tss)
    del timeSeries
    assert True

x = create_ensemble_series()
mtsd = to_multi_regular_time_series_data(x)

# test_create_series()


# CreateEnsembleForecastTimeSeries_py(start, length, timeStepName):
# CreatePerfectForecastTimeSeries_py(observations, start, length, timeStepName, offsetForecasts, leadTime):
# CreateEnsembleTimeSeriesDataFromStruct_py(ensSeries):
# CreateEnsembleDataset_py(type):

# SetTimeSeriesMissingValueValue_py(missingValueValue):

# GetLastStdExceptionMessage_py():
# RegisterExceptionCallback_py(callback):

# DisposeSharedPointer_py(ptr):
# DeleteAnsiStringArray_py(values, arrayLength):
# DeleteAnsiString_py(value):
# DeleteDoubleArray_py(value):
# LoadEnsembleDataset_py(libraryIdentifier, dataPath):
# GetEnsembleDatasetDataIdentifiers_py(dataLibrary):
# GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary, dataCollectionId):
# GetEnsembleDatasetDataSummaries_py(dataLibrary):
# GetDataDimensionsDescription_py(dataLibrary, dataId):
# EnsembleSizeEnsembleTimeSeries_py(ensSeries):
# DisposeDataDimensionsDescriptions_py(data):
# GetDatasetSingleTimeSeries_py(dataLibrary, dataId):
# GetDatasetEnsembleTimeSeries_py(dataLibrary, dataEnsembleId):
# GetDatasetEnsembleForecastTimeSeries_py(dataLibrary, dataId):
# SaveSingleTimeSeriesToNetcdf_py(timeSeries, filename, overwrite):
# SaveEnsembleTimeSeriesToNetcdf_py(collection, filename, overwrite):
# SaveEnsembleForecastTimeSeriesToNetcdf_py(tsEnsTs, filename, overwrite):
# IsMissingValueItemEnsembleForecastTimeSeries_py(series, i):
# GetItemEnsembleForecastTimeSeries_py(efts, i):
# TimeSeriesFromEnsembleOfTimeSeries_py(collectionTs, index):
# TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py(efts, indexInIssueTime, indexInForecastTime):
# GetValueFromUnivariateTimeSeries_py(ts, index):
# TransformEachItem_py(tsEnsTs, method, methodArgument):
# SetValueToUnivariateTimeSeries_py(ts, index, value):
# GetItemEnsembleForecastTimeSeriesAsStructure_py(series, i):
# GetItemEnsembleTimeSeriesAsStructure_py(series, i):
# SetItemEnsembleForecastTimeSeriesAsStructure_py(series, i, values):
# SetItemEnsembleTimeSeriesAsStructure_py(collection, i, values):
# ToStructEnsembleTimeSeriesData_py(ensSeries):
# ToStructSingleTimeSeriesData_py(timeSeries):
# DisposeMultiTimeSeriesData_py(data):
# GetTimeSeriesGeometry_py(timeSeries, geom):
# GetEnsembleForecastTimeSeriesGeometry_py(timeSeries, geom):
# GetTimeSeriesValues_py(timeSeries, values, arrayLength):
# GetNumTimeSeries_py():
# GetProviderTsGeometry_py(dataLibrary, variableIdentifier, geom):
# GetProviderTimeSeriesValues_py(dataLibrary, variableIdentifier, values, arrayLength):
# GetProviderTimeSeriesIdentifiers_py(dataLibrary):
# TimeSeriesFromProviderTs_py(dataLibrary, variableIdentifier):