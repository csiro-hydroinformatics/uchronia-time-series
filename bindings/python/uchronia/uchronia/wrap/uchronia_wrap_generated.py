##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

import xarray as xr
import pandas as pd
import numpy as np
from typing import TYPE_CHECKING, Dict, List, Tuple, Any, Optional
from datetime import datetime
from refcount.interop import CffiData, OwningCffiNativeHandle, DeletableCffiNativeHandle, wrap_as_pointer_handle
from cinterop.cffi.marshal import as_bytes, TimeSeriesGeometryNative
from uchronia.wrap.ffi_interop import marshal, uchronia_so
# phase out importing from uchronia.classes, to prevent cyclic imports
# from uchronia.classes import wrap_cffi_native_handle
# Additional specific imports for this package

import uchronia.wrap.ffi_interop as _u_wrap


if TYPE_CHECKING:
    from uchronia.classes import (
        EnsembleTimeSeries,
        TimeSeriesLibrary,
        EnsembleForecastTimeSeries,
        TimeSeries,
        EnsemblePtrTimeSeries,
        TimeSeriesProvider,
    )

    from refcount.interop import WrapperCreationFunction

__wrap_cffi_native_handle:Optional['WrapperCreationFunction']=None

def set_wrap_cffi_native_handle(wrapper_function:'WrapperCreationFunction'):
    global __wrap_cffi_native_handle
    __wrap_cffi_native_handle = wrapper_function

def custom_wrap_cffi_native_handle(obj, type_id='', release_native = None):
    '''Create a wrapper around a cffi pointer (if this is one), 
    with the suitable native release function call specific to this external pointer. 
    '''
    if __wrap_cffi_native_handle is None:
        raise RuntimeError('The function creating custom wrappers around native objects is None: you must use set_wrap_cffi_native_handle to initialise it')
    if release_native is None:
        release_native = dispose_shared_pointer_py
    return __wrap_cffi_native_handle(obj, type_id, release_native)

def charp_array_to_py(values:CffiData, size:int, dispose:bool=True) -> List[str]:
    pystrings = marshal.c_charptrptr_as_string_list(values, size)
    if dispose:
        uchronia_so.DeleteAnsiStringArray(values, size)
    return pystrings

def char_array_to_py(values:CffiData, dispose:bool=True) -> str:
    pystring = marshal.c_string_as_py_string(values)
    if dispose:
        uchronia_so.DeleteAnsiString(values)
    return pystring

def named_values_to_py(values:CffiData, dispose:bool=True) -> Dict[str,float]:
    x = marshal.named_values_to_dict(values)
    if dispose:
        uchronia_so.DisposeNamedValuedVectorsSwift(values)
    return x

def opaque_ts_as_xarray_time_series(ptr:CffiData, dispose:bool=True) -> xr.DataArray:
    res = marshal.as_xarray_time_series(ptr)
    if dispose:
        uchronia_so.DisposeMultiTimeSeriesData(ptr)
    return res

def py_time_series_dimensions_description(ptr:CffiData, dispose:bool=True) -> List[Tuple[str,int]]:
    n = ptr.num_dimensions
    def dim_spec(i):
        d = ptr.dimensions[i]
        return(marshal.c_string_as_py_string(d.dimension_type), d.size)
    res = [dim_spec(i) for i in range(n)]
    if dispose:
        uchronia_so.DisposeDataDimensionsDescriptions(ptr)
    return res

def toSceParametersNative(x:dict) -> OwningCffiNativeHandle:
    res = marshal.new_native_struct('SceParameters*')
    p = res.ptr
    p.Alpha = int(x['Alpha'])
    p.Beta = int(x['Beta'])
    p.P = int(x['P'])
    p.Pmin = int(x['Pmin'])
    p.M = int(x['M'])
    p.Q = int(x['Q'])
    p.NumShuffle = x['NumShuffle']
    p.TrapezoidalDensityParameter = float(x['TrapezoidalDensityParameter'])
    p.ReflectionRatio = float(x['ReflectionRatio'])
    p.ContractionRatio = float(x['ContractionRatio'])
    return res

def dispose_shared_pointer_py(ptr:Any) -> None:
    # Upon a process terminating, somehow wrap_as_pointer_handle can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if wrap_as_pointer_handle is None:
        return
    ptr_xptr = wrap_as_pointer_handle(ptr)
    # Upon a process terminating, somehow 'uchronia_so' can end up being None,
    # leading to a TypeError: 'NoneType' object is not callable.
    # This is a nuisance, and hard to fully diagnose.
    # So, we will use the following workaround to guard against it. See WIRADA-659.
    if uchronia_so is not None and uchronia_so.DisposeSharedPointer is not None:
        uchronia_so.DisposeSharedPointer(ptr_xptr.ptr)

@_u_wrap.check_exceptions
def _GetLastStdExceptionMessage_native():
    result = uchronia_so.GetLastStdExceptionMessage()
    return result

def GetLastStdExceptionMessage_py() -> str:
    """GetLastStdExceptionMessage_py
    
    GetLastStdExceptionMessage_py: generated wrapper function for API function GetLastStdExceptionMessage
    
    Args:
    
    Returns:
        (str): returned result
    
    """
    result = _GetLastStdExceptionMessage_native()
    return char_array_to_py(result, dispose=True)


@_u_wrap.check_exceptions
def _RegisterExceptionCallback_native(callback):
    uchronia_so.RegisterExceptionCallback(callback)

def RegisterExceptionCallback_py(callback:Any) -> None:
    """RegisterExceptionCallback_py
    
    RegisterExceptionCallback_py: generated wrapper function for API function RegisterExceptionCallback
    
    Args:
        callback (Any): callback
    
    """
    _RegisterExceptionCallback_native(callback)


@_u_wrap.check_exceptions
def _RegisterExceptionCallbackUchronia_native(callback):
    uchronia_so.RegisterExceptionCallbackUchronia(callback)

def RegisterExceptionCallbackUchronia_py(callback:Any) -> None:
    """RegisterExceptionCallbackUchronia_py
    
    RegisterExceptionCallbackUchronia_py: generated wrapper function for API function RegisterExceptionCallbackUchronia
    
    Args:
        callback (Any): callback
    
    """
    _RegisterExceptionCallbackUchronia_native(callback)


@_u_wrap.check_exceptions
def _DisposeSharedPointer_native(ptr):
    uchronia_so.DisposeSharedPointer(ptr)

def DisposeSharedPointer_py(ptr:Any) -> None:
    """DisposeSharedPointer_py
    
    DisposeSharedPointer_py: generated wrapper function for API function DisposeSharedPointer
    
    Args:
        ptr (Any): ptr
    
    """
    ptr_xptr = wrap_as_pointer_handle(ptr)
    _DisposeSharedPointer_native(ptr_xptr.ptr)


@_u_wrap.check_exceptions
def _SetTimeSeriesMissingValueValue_native(missingValueValue):
    uchronia_so.SetTimeSeriesMissingValueValue(missingValueValue)

def SetTimeSeriesMissingValueValue_py(missingValueValue:float) -> None:
    """SetTimeSeriesMissingValueValue_py
    
    SetTimeSeriesMissingValueValue_py: generated wrapper function for API function SetTimeSeriesMissingValueValue
    
    Args:
        missingValueValue (float): missingValueValue
    
    """
    _SetTimeSeriesMissingValueValue_native(missingValueValue)


@_u_wrap.check_exceptions
def _LoadEnsembleDataset_native(libraryIdentifier, dataPath):
    result = uchronia_so.LoadEnsembleDataset(libraryIdentifier, dataPath)
    return result

def LoadEnsembleDataset_py(libraryIdentifier:str, dataPath:str) -> 'TimeSeriesLibrary':
    """LoadEnsembleDataset_py
    
    LoadEnsembleDataset_py: generated wrapper function for API function LoadEnsembleDataset
    
    Args:
        libraryIdentifier (str): libraryIdentifier
        dataPath (str): dataPath
    
    Returns:
        ('TimeSeriesLibrary'): returned result
    
    """
    libraryIdentifier_c_charp = wrap_as_pointer_handle(as_bytes(libraryIdentifier))
    dataPath_c_charp = wrap_as_pointer_handle(as_bytes(dataPath))
    result = _LoadEnsembleDataset_native(libraryIdentifier_c_charp.ptr, dataPath_c_charp.ptr)
    # no cleanup for const char*
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')


@_u_wrap.check_exceptions
def _CreateEnsembleDataset_native(type):
    result = uchronia_so.CreateEnsembleDataset(type)
    return result

def CreateEnsembleDataset_py(type:str) -> 'TimeSeriesLibrary':
    """CreateEnsembleDataset_py
    
    CreateEnsembleDataset_py: generated wrapper function for API function CreateEnsembleDataset
    
    Args:
        type (str): type
    
    Returns:
        ('TimeSeriesLibrary'): returned result
    
    """
    type_c_charp = wrap_as_pointer_handle(as_bytes(type))
    result = _CreateEnsembleDataset_native(type_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')



def _GetEnsembleDatasetDataIdentifiers_native(dataLibrary, size):
    return uchronia_so.GetEnsembleDatasetDataIdentifiers(dataLibrary, size)

def GetEnsembleDatasetDataIdentifiers_py(dataLibrary:'TimeSeriesLibrary'):
    """GetEnsembleDatasetDataIdentifiers_py
    
    GetEnsembleDatasetDataIdentifiers_py: generated wrapper function for API function GetEnsembleDatasetDataIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = _GetEnsembleDatasetDataIdentifiers_native(dataLibrary_xptr.ptr, size)


    result = charp_array_to_py(values, size[0], True)
    return result


def _GetEnsembleDatasetDataSubIdentifiers_native(dataLibrary, dataCollectionId, size):
    return uchronia_so.GetEnsembleDatasetDataSubIdentifiers(dataLibrary, dataCollectionId, size)

def GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary:'TimeSeriesLibrary', dataCollectionId:str):
    """GetEnsembleDatasetDataSubIdentifiers_py
    
    GetEnsembleDatasetDataSubIdentifiers_py: generated wrapper function for API function GetEnsembleDatasetDataSubIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataCollectionId_c_charp = wrap_as_pointer_handle(as_bytes(dataCollectionId))

    size = marshal.new_int_scalar_ptr()
    values = _GetEnsembleDatasetDataSubIdentifiers_native(dataLibrary_xptr.ptr, dataCollectionId_c_charp.ptr, size)
    # no cleanup for const char*

    result = charp_array_to_py(values, size[0], True)
    return result


def _GetEnsembleDatasetDataSummaries_native(dataLibrary, size):
    return uchronia_so.GetEnsembleDatasetDataSummaries(dataLibrary, size)

def GetEnsembleDatasetDataSummaries_py(dataLibrary:'TimeSeriesLibrary'):
    """GetEnsembleDatasetDataSummaries_py
    
    GetEnsembleDatasetDataSummaries_py: generated wrapper function for API function GetEnsembleDatasetDataSummaries
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = _GetEnsembleDatasetDataSummaries_native(dataLibrary_xptr.ptr, size)


    result = charp_array_to_py(values, size[0], True)
    return result

@_u_wrap.check_exceptions
def _GetDataDimensionsDescription_native(dataLibrary, dataId):
    result = uchronia_so.GetDataDimensionsDescription(dataLibrary, dataId)
    return result

def GetDataDimensionsDescription_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> List:
    """GetDataDimensionsDescription_py
    
    GetDataDimensionsDescription_py: generated wrapper function for API function GetDataDimensionsDescription
    
    Args:
        dataLibrary ('TimeSeriesLibrary'): dataLibrary
        dataId (str): dataId
    
    Returns:
        (List): returned result
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataId_c_charp = wrap_as_pointer_handle(as_bytes(dataId))
    result = _GetDataDimensionsDescription_native(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return py_time_series_dimensions_description(result, dispose=True)


@_u_wrap.check_exceptions
def _EnsembleSizeEnsembleTimeSeries_native(ensSeries):
    result = uchronia_so.EnsembleSizeEnsembleTimeSeries(ensSeries)
    return result

def EnsembleSizeEnsembleTimeSeries_py(ensSeries:'EnsemblePtrTimeSeries') -> int:
    """EnsembleSizeEnsembleTimeSeries_py
    
    EnsembleSizeEnsembleTimeSeries_py: generated wrapper function for API function EnsembleSizeEnsembleTimeSeries
    
    Args:
        ensSeries ('EnsemblePtrTimeSeries'): ensSeries
    
    Returns:
        (int): returned result
    
    """
    ensSeries_xptr = wrap_as_pointer_handle(ensSeries)
    result = _EnsembleSizeEnsembleTimeSeries_native(ensSeries_xptr.ptr)
    return result


@_u_wrap.check_exceptions
def _DisposeDataDimensionsDescriptions_native(data):
    uchronia_so.DisposeDataDimensionsDescriptions(data)

def DisposeDataDimensionsDescriptions_py(data:List) -> None:
    """DisposeDataDimensionsDescriptions_py
    
    DisposeDataDimensionsDescriptions_py: generated wrapper function for API function DisposeDataDimensionsDescriptions
    
    Args:
        data (List): data
    
    """
    _DisposeDataDimensionsDescriptions_native(data)


@_u_wrap.check_exceptions
def _CreateEnsembleForecastTimeSeries_native(start, length, timeStepName):
    result = uchronia_so.CreateEnsembleForecastTimeSeries(start, length, timeStepName)
    return result

def CreateEnsembleForecastTimeSeries_py(start:datetime, length:int, timeStepName:str) -> 'EnsembleForecastTimeSeries':
    """CreateEnsembleForecastTimeSeries_py
    
    CreateEnsembleForecastTimeSeries_py: generated wrapper function for API function CreateEnsembleForecastTimeSeries
    
    Args:
        start (datetime): start
        length (int): length
        timeStepName (str): timeStepName
    
    Returns:
        ('EnsembleForecastTimeSeries'): returned result
    
    """
    start_datetime = marshal.datetime_to_dtts(start)
    timeStepName_c_charp = wrap_as_pointer_handle(as_bytes(timeStepName))
    result = _CreateEnsembleForecastTimeSeries_native(start_datetime.obj, length, timeStepName_c_charp.ptr)
    # start_datetime - no cleanup needed?
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _GetDatasetSingleTimeSeries_native(dataLibrary, dataId):
    result = uchronia_so.GetDatasetSingleTimeSeries(dataLibrary, dataId)
    return result

def GetDatasetSingleTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> 'TimeSeries':
    """GetDatasetSingleTimeSeries_py
    
    GetDatasetSingleTimeSeries_py: generated wrapper function for API function GetDatasetSingleTimeSeries
    
    Args:
        dataLibrary ('TimeSeriesLibrary'): dataLibrary
        dataId (str): dataId
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataId_c_charp = wrap_as_pointer_handle(as_bytes(dataId))
    result = _GetDatasetSingleTimeSeries_native(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _GetDatasetEnsembleTimeSeries_native(dataLibrary, dataEnsembleId):
    result = uchronia_so.GetDatasetEnsembleTimeSeries(dataLibrary, dataEnsembleId)
    return result

def GetDatasetEnsembleTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataEnsembleId:str) -> 'EnsemblePtrTimeSeries':
    """GetDatasetEnsembleTimeSeries_py
    
    GetDatasetEnsembleTimeSeries_py: generated wrapper function for API function GetDatasetEnsembleTimeSeries
    
    Args:
        dataLibrary ('TimeSeriesLibrary'): dataLibrary
        dataEnsembleId (str): dataEnsembleId
    
    Returns:
        ('EnsemblePtrTimeSeries'): returned result
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataEnsembleId_c_charp = wrap_as_pointer_handle(as_bytes(dataEnsembleId))
    result = _GetDatasetEnsembleTimeSeries_native(dataLibrary_xptr.ptr, dataEnsembleId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _GetDatasetEnsembleForecastTimeSeries_native(dataLibrary, dataId):
    result = uchronia_so.GetDatasetEnsembleForecastTimeSeries(dataLibrary, dataId)
    return result

def GetDatasetEnsembleForecastTimeSeries_py(dataLibrary:'TimeSeriesLibrary', dataId:str) -> 'EnsembleForecastTimeSeries':
    """GetDatasetEnsembleForecastTimeSeries_py
    
    GetDatasetEnsembleForecastTimeSeries_py: generated wrapper function for API function GetDatasetEnsembleForecastTimeSeries
    
    Args:
        dataLibrary ('TimeSeriesLibrary'): dataLibrary
        dataId (str): dataId
    
    Returns:
        ('EnsembleForecastTimeSeries'): returned result
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataId_c_charp = wrap_as_pointer_handle(as_bytes(dataId))
    result = _GetDatasetEnsembleForecastTimeSeries_native(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _SaveSingleTimeSeriesToNetcdf_native(timeSeries, filename, overwrite):
    uchronia_so.SaveSingleTimeSeriesToNetcdf(timeSeries, filename, overwrite)

def SaveSingleTimeSeriesToNetcdf_py(timeSeries:'TimeSeries', filename:str, overwrite:bool) -> None:
    """SaveSingleTimeSeriesToNetcdf_py
    
    SaveSingleTimeSeriesToNetcdf_py: generated wrapper function for API function SaveSingleTimeSeriesToNetcdf
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
        filename (str): filename
        overwrite (bool): overwrite
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    filename_c_charp = wrap_as_pointer_handle(as_bytes(filename))
    _SaveSingleTimeSeriesToNetcdf_native(timeSeries_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@_u_wrap.check_exceptions
def _SaveEnsembleTimeSeriesToNetcdf_native(collection, filename, overwrite):
    uchronia_so.SaveEnsembleTimeSeriesToNetcdf(collection, filename, overwrite)

def SaveEnsembleTimeSeriesToNetcdf_py(collection:'EnsemblePtrTimeSeries', filename:str, overwrite:bool) -> None:
    """SaveEnsembleTimeSeriesToNetcdf_py
    
    SaveEnsembleTimeSeriesToNetcdf_py: generated wrapper function for API function SaveEnsembleTimeSeriesToNetcdf
    
    Args:
        collection ('EnsemblePtrTimeSeries'): collection
        filename (str): filename
        overwrite (bool): overwrite
    
    """
    collection_xptr = wrap_as_pointer_handle(collection)
    filename_c_charp = wrap_as_pointer_handle(as_bytes(filename))
    _SaveEnsembleTimeSeriesToNetcdf_native(collection_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@_u_wrap.check_exceptions
def _SaveEnsembleForecastTimeSeriesToNetcdf_native(tsEnsTs, filename, overwrite):
    uchronia_so.SaveEnsembleForecastTimeSeriesToNetcdf(tsEnsTs, filename, overwrite)

def SaveEnsembleForecastTimeSeriesToNetcdf_py(tsEnsTs:'EnsembleForecastTimeSeries', filename:str, overwrite:bool) -> None:
    """SaveEnsembleForecastTimeSeriesToNetcdf_py
    
    SaveEnsembleForecastTimeSeriesToNetcdf_py: generated wrapper function for API function SaveEnsembleForecastTimeSeriesToNetcdf
    
    Args:
        tsEnsTs ('EnsembleForecastTimeSeries'): tsEnsTs
        filename (str): filename
        overwrite (bool): overwrite
    
    """
    tsEnsTs_xptr = wrap_as_pointer_handle(tsEnsTs)
    filename_c_charp = wrap_as_pointer_handle(as_bytes(filename))
    _SaveEnsembleForecastTimeSeriesToNetcdf_native(tsEnsTs_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@_u_wrap.check_exceptions
def _IsMissingValueItemEnsembleForecastTimeSeries_native(series, i):
    result = uchronia_so.IsMissingValueItemEnsembleForecastTimeSeries(series, i)
    return result

def IsMissingValueItemEnsembleForecastTimeSeries_py(series:'EnsembleForecastTimeSeries', i:int) -> bool:
    """IsMissingValueItemEnsembleForecastTimeSeries_py
    
    IsMissingValueItemEnsembleForecastTimeSeries_py: generated wrapper function for API function IsMissingValueItemEnsembleForecastTimeSeries
    
    Args:
        series ('EnsembleForecastTimeSeries'): series
        i (int): i
    
    Returns:
        (bool): returned result
    
    """
    series_xptr = wrap_as_pointer_handle(series)
    result = _IsMissingValueItemEnsembleForecastTimeSeries_native(series_xptr.ptr, i)
    return result


@_u_wrap.check_exceptions
def _GetItemEnsembleForecastTimeSeries_native(efts, i):
    result = uchronia_so.GetItemEnsembleForecastTimeSeries(efts, i)
    return result

def GetItemEnsembleForecastTimeSeries_py(efts:'EnsembleForecastTimeSeries', i:int) -> 'EnsemblePtrTimeSeries':
    """GetItemEnsembleForecastTimeSeries_py
    
    GetItemEnsembleForecastTimeSeries_py: generated wrapper function for API function GetItemEnsembleForecastTimeSeries
    
    Args:
        efts ('EnsembleForecastTimeSeries'): efts
        i (int): i
    
    Returns:
        ('EnsemblePtrTimeSeries'): returned result
    
    """
    efts_xptr = wrap_as_pointer_handle(efts)
    result = _GetItemEnsembleForecastTimeSeries_native(efts_xptr.ptr, i)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _TimeSeriesFromEnsembleOfTimeSeries_native(collectionTs, index):
    result = uchronia_so.TimeSeriesFromEnsembleOfTimeSeries(collectionTs, index)
    return result

def TimeSeriesFromEnsembleOfTimeSeries_py(collectionTs:'EnsemblePtrTimeSeries', index:int) -> 'TimeSeries':
    """TimeSeriesFromEnsembleOfTimeSeries_py
    
    TimeSeriesFromEnsembleOfTimeSeries_py: generated wrapper function for API function TimeSeriesFromEnsembleOfTimeSeries
    
    Args:
        collectionTs ('EnsemblePtrTimeSeries'): collectionTs
        index (int): index
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    collectionTs_xptr = wrap_as_pointer_handle(collectionTs)
    result = _TimeSeriesFromEnsembleOfTimeSeries_native(collectionTs_xptr.ptr, index)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_native(efts, indexInIssueTime, indexInForecastTime):
    result = uchronia_so.TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(efts, indexInIssueTime, indexInForecastTime)
    return result

def TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py(efts:'EnsembleForecastTimeSeries', indexInIssueTime:int, indexInForecastTime:int) -> 'TimeSeries':
    """TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py
    
    TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py: generated wrapper function for API function TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries
    
    Args:
        efts ('EnsembleForecastTimeSeries'): efts
        indexInIssueTime (int): indexInIssueTime
        indexInForecastTime (int): indexInForecastTime
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    efts_xptr = wrap_as_pointer_handle(efts)
    result = _TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_native(efts_xptr.ptr, indexInIssueTime, indexInForecastTime)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _GetValueFromUnivariateTimeSeries_native(ts, index):
    result = uchronia_so.GetValueFromUnivariateTimeSeries(ts, index)
    return result

def GetValueFromUnivariateTimeSeries_py(ts:'TimeSeries', index:int) -> float:
    """GetValueFromUnivariateTimeSeries_py
    
    GetValueFromUnivariateTimeSeries_py: generated wrapper function for API function GetValueFromUnivariateTimeSeries
    
    Args:
        ts ('TimeSeries'): ts
        index (int): index
    
    Returns:
        (float): returned result
    
    """
    ts_xptr = wrap_as_pointer_handle(ts)
    result = _GetValueFromUnivariateTimeSeries_native(ts_xptr.ptr, index)
    return result


@_u_wrap.check_exceptions
def _TransformEachItem_native(tsEnsTs, method, methodArgument):
    uchronia_so.TransformEachItem(tsEnsTs, method, methodArgument)

def TransformEachItem_py(tsEnsTs:'EnsembleForecastTimeSeries', method:str, methodArgument:str) -> None:
    """TransformEachItem_py
    
    TransformEachItem_py: generated wrapper function for API function TransformEachItem
    
    Args:
        tsEnsTs ('EnsembleForecastTimeSeries'): tsEnsTs
        method (str): method
        methodArgument (str): methodArgument
    
    """
    tsEnsTs_xptr = wrap_as_pointer_handle(tsEnsTs)
    method_c_charp = wrap_as_pointer_handle(as_bytes(method))
    methodArgument_c_charp = wrap_as_pointer_handle(as_bytes(methodArgument))
    _TransformEachItem_native(tsEnsTs_xptr.ptr, method_c_charp.ptr, methodArgument_c_charp.ptr)
    # no cleanup for const char*
    # no cleanup for const char*


@_u_wrap.check_exceptions
def _SetValueToUnivariateTimeSeries_native(ts, index, value):
    uchronia_so.SetValueToUnivariateTimeSeries(ts, index, value)

def SetValueToUnivariateTimeSeries_py(ts:'TimeSeries', index:int, value:float) -> None:
    """SetValueToUnivariateTimeSeries_py
    
    SetValueToUnivariateTimeSeries_py: generated wrapper function for API function SetValueToUnivariateTimeSeries
    
    Args:
        ts ('TimeSeries'): ts
        index (int): index
        value (float): value
    
    """
    ts_xptr = wrap_as_pointer_handle(ts)
    _SetValueToUnivariateTimeSeries_native(ts_xptr.ptr, index, value)


@_u_wrap.check_exceptions
def _GetItemEnsembleForecastTimeSeriesAsStructure_native(series, i):
    result = uchronia_so.GetItemEnsembleForecastTimeSeriesAsStructure(series, i)
    return result

def GetItemEnsembleForecastTimeSeriesAsStructure_py(series:'EnsembleForecastTimeSeries', i:int) -> xr.DataArray:
    """GetItemEnsembleForecastTimeSeriesAsStructure_py
    
    GetItemEnsembleForecastTimeSeriesAsStructure_py: generated wrapper function for API function GetItemEnsembleForecastTimeSeriesAsStructure
    
    Args:
        series ('EnsembleForecastTimeSeries'): series
        i (int): i
    
    Returns:
        (xr.DataArray): returned result
    
    """
    series_xptr = wrap_as_pointer_handle(series)
    result = _GetItemEnsembleForecastTimeSeriesAsStructure_native(series_xptr.ptr, i)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@_u_wrap.check_exceptions
def _GetItemEnsembleTimeSeriesAsStructure_native(series, i):
    result = uchronia_so.GetItemEnsembleTimeSeriesAsStructure(series, i)
    return result

def GetItemEnsembleTimeSeriesAsStructure_py(series:'EnsemblePtrTimeSeries', i:int) -> xr.DataArray:
    """GetItemEnsembleTimeSeriesAsStructure_py
    
    GetItemEnsembleTimeSeriesAsStructure_py: generated wrapper function for API function GetItemEnsembleTimeSeriesAsStructure
    
    Args:
        series ('EnsemblePtrTimeSeries'): series
        i (int): i
    
    Returns:
        (xr.DataArray): returned result
    
    """
    series_xptr = wrap_as_pointer_handle(series)
    result = _GetItemEnsembleTimeSeriesAsStructure_native(series_xptr.ptr, i)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@_u_wrap.check_exceptions
def _SetItemEnsembleForecastTimeSeriesAsStructure_native(series, i, values):
    uchronia_so.SetItemEnsembleForecastTimeSeriesAsStructure(series, i, values)

def SetItemEnsembleForecastTimeSeriesAsStructure_py(series:'EnsembleForecastTimeSeries', i:int, values:xr.DataArray) -> None:
    """SetItemEnsembleForecastTimeSeriesAsStructure_py
    
    SetItemEnsembleForecastTimeSeriesAsStructure_py: generated wrapper function for API function SetItemEnsembleForecastTimeSeriesAsStructure
    
    Args:
        series ('EnsembleForecastTimeSeries'): series
        i (int): i
        values (xr.DataArray): values
    
    """
    series_xptr = wrap_as_pointer_handle(series)
    values_tsd_ptr = marshal.as_native_time_series(values)
    _SetItemEnsembleForecastTimeSeriesAsStructure_native(series_xptr.ptr, i, values_tsd_ptr.ptr)
    # values_tsd_ptr - no cleanup needed?


@_u_wrap.check_exceptions
def _SetItemEnsembleTimeSeriesAsStructure_native(collection, i, values):
    uchronia_so.SetItemEnsembleTimeSeriesAsStructure(collection, i, values)

def SetItemEnsembleTimeSeriesAsStructure_py(collection:'EnsemblePtrTimeSeries', i:int, values:xr.DataArray) -> None:
    """SetItemEnsembleTimeSeriesAsStructure_py
    
    SetItemEnsembleTimeSeriesAsStructure_py: generated wrapper function for API function SetItemEnsembleTimeSeriesAsStructure
    
    Args:
        collection ('EnsemblePtrTimeSeries'): collection
        i (int): i
        values (xr.DataArray): values
    
    """
    collection_xptr = wrap_as_pointer_handle(collection)
    values_tsd_ptr = marshal.as_native_time_series(values)
    _SetItemEnsembleTimeSeriesAsStructure_native(collection_xptr.ptr, i, values_tsd_ptr.ptr)
    # values_tsd_ptr - no cleanup needed?


@_u_wrap.check_exceptions
def _CreatePerfectForecastTimeSeries_native(observations, start, length, timeStepName, offsetForecasts, leadTime):
    result = uchronia_so.CreatePerfectForecastTimeSeries(observations, start, length, timeStepName, offsetForecasts, leadTime)
    return result

def CreatePerfectForecastTimeSeries_py(observations:'TimeSeries', start:datetime, length:int, timeStepName:str, offsetForecasts:int, leadTime:int) -> 'EnsembleForecastTimeSeries':
    """CreatePerfectForecastTimeSeries_py
    
    CreatePerfectForecastTimeSeries_py: generated wrapper function for API function CreatePerfectForecastTimeSeries
    
    Args:
        observations ('TimeSeries'): observations
        start (datetime): start
        length (int): length
        timeStepName (str): timeStepName
        offsetForecasts (int): offsetForecasts
        leadTime (int): leadTime
    
    Returns:
        ('EnsembleForecastTimeSeries'): returned result
    
    """
    observations_xptr = wrap_as_pointer_handle(observations)
    start_datetime = marshal.datetime_to_dtts(start)
    timeStepName_c_charp = wrap_as_pointer_handle(as_bytes(timeStepName))
    result = _CreatePerfectForecastTimeSeries_native(observations_xptr.ptr, start_datetime.obj, length, timeStepName_c_charp.ptr, offsetForecasts, leadTime)
    # start_datetime - no cleanup needed?
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _ToStructEnsembleTimeSeriesData_native(ensSeries):
    result = uchronia_so.ToStructEnsembleTimeSeriesData(ensSeries)
    return result

def ToStructEnsembleTimeSeriesData_py(ensSeries:'EnsemblePtrTimeSeries') -> xr.DataArray:
    """ToStructEnsembleTimeSeriesData_py
    
    ToStructEnsembleTimeSeriesData_py: generated wrapper function for API function ToStructEnsembleTimeSeriesData
    
    Args:
        ensSeries ('EnsemblePtrTimeSeries'): ensSeries
    
    Returns:
        (xr.DataArray): returned result
    
    """
    ensSeries_xptr = wrap_as_pointer_handle(ensSeries)
    result = _ToStructEnsembleTimeSeriesData_native(ensSeries_xptr.ptr)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@_u_wrap.check_exceptions
def _ToStructSingleTimeSeriesData_native(timeSeries):
    result = uchronia_so.ToStructSingleTimeSeriesData(timeSeries)
    return result

def ToStructSingleTimeSeriesData_py(timeSeries:'TimeSeries') -> xr.DataArray:
    """ToStructSingleTimeSeriesData_py
    
    ToStructSingleTimeSeriesData_py: generated wrapper function for API function ToStructSingleTimeSeriesData
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
    
    Returns:
        (xr.DataArray): returned result
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    result = _ToStructSingleTimeSeriesData_native(timeSeries_xptr.ptr)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@_u_wrap.check_exceptions
def _CreateEnsembleTimeSeriesDataFromStruct_native(ensSeries):
    result = uchronia_so.CreateEnsembleTimeSeriesDataFromStruct(ensSeries)
    return result

def CreateEnsembleTimeSeriesDataFromStruct_py(ensSeries:xr.DataArray) -> 'EnsemblePtrTimeSeries':
    """CreateEnsembleTimeSeriesDataFromStruct_py
    
    CreateEnsembleTimeSeriesDataFromStruct_py: generated wrapper function for API function CreateEnsembleTimeSeriesDataFromStruct
    
    Args:
        ensSeries (xr.DataArray): ensSeries
    
    Returns:
        ('EnsemblePtrTimeSeries'): returned result
    
    """
    ensSeries_tsd_ptr = marshal.as_native_time_series(ensSeries)
    result = _CreateEnsembleTimeSeriesDataFromStruct_native(ensSeries_tsd_ptr.ptr)
    # ensSeries_tsd_ptr - no cleanup needed?
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _CreateSingleTimeSeriesDataFromStruct_native(timeSeries):
    result = uchronia_so.CreateSingleTimeSeriesDataFromStruct(timeSeries)
    return result

def CreateSingleTimeSeriesDataFromStruct_py(timeSeries:xr.DataArray) -> 'TimeSeries':
    """CreateSingleTimeSeriesDataFromStruct_py
    
    CreateSingleTimeSeriesDataFromStruct_py: generated wrapper function for API function CreateSingleTimeSeriesDataFromStruct
    
    Args:
        timeSeries (xr.DataArray): timeSeries
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    timeSeries_tsd_ptr = marshal.as_native_time_series(timeSeries)
    result = _CreateSingleTimeSeriesDataFromStruct_native(timeSeries_tsd_ptr.ptr)
    # timeSeries_tsd_ptr - no cleanup needed?
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@_u_wrap.check_exceptions
def _DisposeMultiTimeSeriesData_native(data):
    uchronia_so.DisposeMultiTimeSeriesData(data)

def DisposeMultiTimeSeriesData_py(data:xr.DataArray) -> None:
    """DisposeMultiTimeSeriesData_py
    
    DisposeMultiTimeSeriesData_py: generated wrapper function for API function DisposeMultiTimeSeriesData
    
    Args:
        data (xr.DataArray): data
    
    """
    _DisposeMultiTimeSeriesData_native(data)


@_u_wrap.check_exceptions
def _GetTimeSeriesGeometry_native(timeSeries, geom):
    uchronia_so.GetTimeSeriesGeometry(timeSeries, geom)

def GetTimeSeriesGeometry_py(timeSeries:'TimeSeries', geom:TimeSeriesGeometryNative) -> None:
    """GetTimeSeriesGeometry_py
    
    GetTimeSeriesGeometry_py: generated wrapper function for API function GetTimeSeriesGeometry
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
        geom (TimeSeriesGeometryNative): geom
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    geom_xptr = wrap_as_pointer_handle(geom)
    _GetTimeSeriesGeometry_native(timeSeries_xptr.ptr, geom_xptr.ptr)


@_u_wrap.check_exceptions
def _GetEnsembleForecastTimeSeriesGeometry_native(timeSeries, geom):
    uchronia_so.GetEnsembleForecastTimeSeriesGeometry(timeSeries, geom)

def GetEnsembleForecastTimeSeriesGeometry_py(timeSeries:'EnsembleForecastTimeSeries', geom:TimeSeriesGeometryNative) -> None:
    """GetEnsembleForecastTimeSeriesGeometry_py
    
    GetEnsembleForecastTimeSeriesGeometry_py: generated wrapper function for API function GetEnsembleForecastTimeSeriesGeometry
    
    Args:
        timeSeries ('EnsembleForecastTimeSeries'): timeSeries
        geom (TimeSeriesGeometryNative): geom
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    geom_xptr = wrap_as_pointer_handle(geom)
    _GetEnsembleForecastTimeSeriesGeometry_native(timeSeries_xptr.ptr, geom_xptr.ptr)


@_u_wrap.check_exceptions
def _GetTimeSeriesValues_native(timeSeries, values, arrayLength):
    uchronia_so.GetTimeSeriesValues(timeSeries, values, arrayLength)

def GetTimeSeriesValues_py(timeSeries:'TimeSeries', values:np.ndarray, arrayLength:int) -> None:
    """GetTimeSeriesValues_py
    
    GetTimeSeriesValues_py: generated wrapper function for API function GetTimeSeriesValues
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
        values (np.ndarray): values
        arrayLength (int): arrayLength
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    values_numarray = marshal.as_c_double_array(values, shallow=True)
    _GetTimeSeriesValues_native(timeSeries_xptr.ptr, values_numarray.ptr, arrayLength)
    # values_numarray - no cleanup needed?


@_u_wrap.check_exceptions
def _GetNumTimeSeries_native():
    result = uchronia_so.GetNumTimeSeries()
    return result

def GetNumTimeSeries_py() -> int:
    """GetNumTimeSeries_py
    
    GetNumTimeSeries_py: generated wrapper function for API function GetNumTimeSeries
    
    Args:
    
    Returns:
        (int): returned result
    
    """
    result = _GetNumTimeSeries_native()
    return result


@_u_wrap.check_exceptions
def _GetProviderTsGeometry_native(dataLibrary, variableIdentifier, geom):
    uchronia_so.GetProviderTsGeometry(dataLibrary, variableIdentifier, geom)

def GetProviderTsGeometry_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str, geom:TimeSeriesGeometryNative) -> None:
    """GetProviderTsGeometry_py
    
    GetProviderTsGeometry_py: generated wrapper function for API function GetProviderTsGeometry
    
    Args:
        dataLibrary ('TimeSeriesProvider'): dataLibrary
        variableIdentifier (str): variableIdentifier
        geom (TimeSeriesGeometryNative): geom
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    variableIdentifier_c_charp = wrap_as_pointer_handle(as_bytes(variableIdentifier))
    geom_xptr = wrap_as_pointer_handle(geom)
    _GetProviderTsGeometry_native(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr, geom_xptr.ptr)
    # no cleanup for const char*


@_u_wrap.check_exceptions
def _GetProviderTimeSeriesValues_native(dataLibrary, variableIdentifier, values, arrayLength):
    uchronia_so.GetProviderTimeSeriesValues(dataLibrary, variableIdentifier, values, arrayLength)

def GetProviderTimeSeriesValues_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str, values:np.ndarray, arrayLength:int) -> None:
    """GetProviderTimeSeriesValues_py
    
    GetProviderTimeSeriesValues_py: generated wrapper function for API function GetProviderTimeSeriesValues
    
    Args:
        dataLibrary ('TimeSeriesProvider'): dataLibrary
        variableIdentifier (str): variableIdentifier
        values (np.ndarray): values
        arrayLength (int): arrayLength
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    variableIdentifier_c_charp = wrap_as_pointer_handle(as_bytes(variableIdentifier))
    values_numarray = marshal.as_c_double_array(values, shallow=True)
    _GetProviderTimeSeriesValues_native(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr, values_numarray.ptr, arrayLength)
    # no cleanup for const char*
    # values_numarray - no cleanup needed?



def _GetProviderTimeSeriesIdentifiers_native(dataLibrary, size):
    return uchronia_so.GetProviderTimeSeriesIdentifiers(dataLibrary, size)

def GetProviderTimeSeriesIdentifiers_py(dataLibrary:'TimeSeriesProvider'):
    """GetProviderTimeSeriesIdentifiers_py
    
    GetProviderTimeSeriesIdentifiers_py: generated wrapper function for API function GetProviderTimeSeriesIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = _GetProviderTimeSeriesIdentifiers_native(dataLibrary_xptr.ptr, size)


    result = charp_array_to_py(values, size[0], True)
    return result

@_u_wrap.check_exceptions
def _TimeSeriesFromProviderTs_native(dataLibrary, variableIdentifier):
    result = uchronia_so.TimeSeriesFromProviderTs(dataLibrary, variableIdentifier)
    return result

def TimeSeriesFromProviderTs_py(dataLibrary:'TimeSeriesProvider', variableIdentifier:str) -> 'TimeSeries':
    """TimeSeriesFromProviderTs_py
    
    TimeSeriesFromProviderTs_py: generated wrapper function for API function TimeSeriesFromProviderTs
    
    Args:
        dataLibrary ('TimeSeriesProvider'): dataLibrary
        variableIdentifier (str): variableIdentifier
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    variableIdentifier_c_charp = wrap_as_pointer_handle(as_bytes(variableIdentifier))
    result = _TimeSeriesFromProviderTs_native(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


