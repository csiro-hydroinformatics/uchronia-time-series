##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

import xarray as xr
import pandas as pd
import numpy as np
from typing import TYPE_CHECKING, Dict, List, Tuple, Any
from datetime import datetime
from refcount.interop import CffiData, OwningCffiNativeHandle, DeletableCffiNativeHandle, wrap_as_pointer_handle
from cinterop.cffi.marshal import as_bytes, TimeSeriesGeometryNative
from uchronia.wrap.ffi_interop import marshal, uchronia_so, check_exceptions
# phase out importing from uchronia.classes, to prevent cyclic imports
# from uchronia.classes import wrap_cffi_native_handle

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

__wrap_cffi_native_handle:'WrapperCreationFunction'=None

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
        release_native = DisposeSharedPointer_py
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

@check_exceptions
def GetLastStdExceptionMessage_py() -> str:
    """GetLastStdExceptionMessage_py
    
    GetLastStdExceptionMessage_py: generated wrapper function for API function GetLastStdExceptionMessage
    
    Args:
    
    Returns:
        (str): returned result
    
    """
    result = uchronia_so.GetLastStdExceptionMessage()
    return char_array_to_py(result, dispose=True)


@check_exceptions
def RegisterExceptionCallback_py(callback:Any) -> None:
    """RegisterExceptionCallback_py
    
    RegisterExceptionCallback_py: generated wrapper function for API function RegisterExceptionCallback
    
    Args:
        callback (Any): callback
    
    """
    uchronia_so.RegisterExceptionCallback(callback)


@check_exceptions
def DisposeSharedPointer_py(ptr:Any) -> None:
    """DisposeSharedPointer_py
    
    DisposeSharedPointer_py: generated wrapper function for API function DisposeSharedPointer
    
    Args:
        ptr (Any): ptr
    
    """
    ptr_xptr = wrap_as_pointer_handle(ptr)
    uchronia_so.DisposeSharedPointer(ptr_xptr.ptr)


@check_exceptions
def SetTimeSeriesMissingValueValue_py(missingValueValue:float) -> None:
    """SetTimeSeriesMissingValueValue_py
    
    SetTimeSeriesMissingValueValue_py: generated wrapper function for API function SetTimeSeriesMissingValueValue
    
    Args:
        missingValueValue (float): missingValueValue
    
    """
    uchronia_so.SetTimeSeriesMissingValueValue(missingValueValue)


@check_exceptions
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
    result = uchronia_so.LoadEnsembleDataset(libraryIdentifier_c_charp.ptr, dataPath_c_charp.ptr)
    # no cleanup for const char*
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')


@check_exceptions
def CreateEnsembleDataset_py(type:str) -> 'TimeSeriesLibrary':
    """CreateEnsembleDataset_py
    
    CreateEnsembleDataset_py: generated wrapper function for API function CreateEnsembleDataset
    
    Args:
        type (str): type
    
    Returns:
        ('TimeSeriesLibrary'): returned result
    
    """
    type_c_charp = wrap_as_pointer_handle(as_bytes(type))
    result = uchronia_so.CreateEnsembleDataset(type_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')



def GetEnsembleDatasetDataIdentifiers_py(dataLibrary:'TimeSeriesLibrary'):
    """GetEnsembleDatasetDataIdentifiers_py
    
    GetEnsembleDatasetDataIdentifiers_py: generated wrapper function for API function GetEnsembleDatasetDataIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = uchronia_so.GetEnsembleDatasetDataIdentifiers(dataLibrary_xptr.ptr, size)
    

    result = charp_array_to_py(values, size[0], True)
    return result


def GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary:'TimeSeriesLibrary', dataCollectionId:str):
    """GetEnsembleDatasetDataSubIdentifiers_py
    
    GetEnsembleDatasetDataSubIdentifiers_py: generated wrapper function for API function GetEnsembleDatasetDataSubIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)
    dataCollectionId_c_charp = wrap_as_pointer_handle(as_bytes(dataCollectionId))

    size = marshal.new_int_scalar_ptr()
    values = uchronia_so.GetEnsembleDatasetDataSubIdentifiers(dataLibrary_xptr.ptr, dataCollectionId_c_charp.ptr, size)
    
    # no cleanup for const char*

    result = charp_array_to_py(values, size[0], True)
    return result


def GetEnsembleDatasetDataSummaries_py(dataLibrary:'TimeSeriesLibrary'):
    """GetEnsembleDatasetDataSummaries_py
    
    GetEnsembleDatasetDataSummaries_py: generated wrapper function for API function GetEnsembleDatasetDataSummaries
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = uchronia_so.GetEnsembleDatasetDataSummaries(dataLibrary_xptr.ptr, size)
    

    result = charp_array_to_py(values, size[0], True)
    return result

@check_exceptions
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
    result = uchronia_so.GetDataDimensionsDescription(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return py_time_series_dimensions_description(result, dispose=True)


@check_exceptions
def EnsembleSizeEnsembleTimeSeries_py(ensSeries:'EnsemblePtrTimeSeries') -> int:
    """EnsembleSizeEnsembleTimeSeries_py
    
    EnsembleSizeEnsembleTimeSeries_py: generated wrapper function for API function EnsembleSizeEnsembleTimeSeries
    
    Args:
        ensSeries ('EnsemblePtrTimeSeries'): ensSeries
    
    Returns:
        (int): returned result
    
    """
    ensSeries_xptr = wrap_as_pointer_handle(ensSeries)
    result = uchronia_so.EnsembleSizeEnsembleTimeSeries(ensSeries_xptr.ptr)
    return result


@check_exceptions
def DisposeDataDimensionsDescriptions_py(data:List) -> None:
    """DisposeDataDimensionsDescriptions_py
    
    DisposeDataDimensionsDescriptions_py: generated wrapper function for API function DisposeDataDimensionsDescriptions
    
    Args:
        data (List): data
    
    """
    uchronia_so.DisposeDataDimensionsDescriptions(data)


@check_exceptions
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
    result = uchronia_so.CreateEnsembleForecastTimeSeries(start_datetime.obj, length, timeStepName_c_charp.ptr)
    # start_datetime - no cleanup needed?
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.GetDatasetSingleTimeSeries(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.GetDatasetEnsembleTimeSeries(dataLibrary_xptr.ptr, dataEnsembleId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.GetDatasetEnsembleForecastTimeSeries(dataLibrary_xptr.ptr, dataId_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@check_exceptions
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
    uchronia_so.SaveSingleTimeSeriesToNetcdf(timeSeries_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@check_exceptions
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
    uchronia_so.SaveEnsembleTimeSeriesToNetcdf(collection_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@check_exceptions
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
    uchronia_so.SaveEnsembleForecastTimeSeriesToNetcdf(tsEnsTs_xptr.ptr, filename_c_charp.ptr, overwrite)
    # no cleanup for const char*


@check_exceptions
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
    result = uchronia_so.IsMissingValueItemEnsembleForecastTimeSeries(series_xptr.ptr, i)
    return result


@check_exceptions
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
    result = uchronia_so.GetItemEnsembleForecastTimeSeries(efts_xptr.ptr, i)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.TimeSeriesFromEnsembleOfTimeSeries(collectionTs_xptr.ptr, index)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(efts_xptr.ptr, indexInIssueTime, indexInForecastTime)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@check_exceptions
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
    result = uchronia_so.GetValueFromUnivariateTimeSeries(ts_xptr.ptr, index)
    return result


@check_exceptions
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
    uchronia_so.TransformEachItem(tsEnsTs_xptr.ptr, method_c_charp.ptr, methodArgument_c_charp.ptr)
    # no cleanup for const char*
    # no cleanup for const char*


@check_exceptions
def SetValueToUnivariateTimeSeries_py(ts:'TimeSeries', index:int, value:float) -> None:
    """SetValueToUnivariateTimeSeries_py
    
    SetValueToUnivariateTimeSeries_py: generated wrapper function for API function SetValueToUnivariateTimeSeries
    
    Args:
        ts ('TimeSeries'): ts
        index (int): index
        value (float): value
    
    """
    ts_xptr = wrap_as_pointer_handle(ts)
    uchronia_so.SetValueToUnivariateTimeSeries(ts_xptr.ptr, index, value)


@check_exceptions
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
    result = uchronia_so.GetItemEnsembleForecastTimeSeriesAsStructure(series_xptr.ptr, i)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@check_exceptions
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
    result = uchronia_so.GetItemEnsembleTimeSeriesAsStructure(series_xptr.ptr, i)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@check_exceptions
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
    uchronia_so.SetItemEnsembleForecastTimeSeriesAsStructure(series_xptr.ptr, i, values_tsd_ptr.ptr)
    # values_tsd_ptr - no cleanup needed?


@check_exceptions
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
    uchronia_so.SetItemEnsembleTimeSeriesAsStructure(collection_xptr.ptr, i, values_tsd_ptr.ptr)
    # values_tsd_ptr - no cleanup needed?


@check_exceptions
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
    result = uchronia_so.CreatePerfectForecastTimeSeries(observations_xptr.ptr, start_datetime.obj, length, timeStepName_c_charp.ptr, offsetForecasts, leadTime)
    # start_datetime - no cleanup needed?
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


@check_exceptions
def ToStructEnsembleTimeSeriesData_py(ensSeries:'EnsemblePtrTimeSeries') -> xr.DataArray:
    """ToStructEnsembleTimeSeriesData_py
    
    ToStructEnsembleTimeSeriesData_py: generated wrapper function for API function ToStructEnsembleTimeSeriesData
    
    Args:
        ensSeries ('EnsemblePtrTimeSeries'): ensSeries
    
    Returns:
        (xr.DataArray): returned result
    
    """
    ensSeries_xptr = wrap_as_pointer_handle(ensSeries)
    result = uchronia_so.ToStructEnsembleTimeSeriesData(ensSeries_xptr.ptr)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@check_exceptions
def ToStructSingleTimeSeriesData_py(timeSeries:'TimeSeries') -> xr.DataArray:
    """ToStructSingleTimeSeriesData_py
    
    ToStructSingleTimeSeriesData_py: generated wrapper function for API function ToStructSingleTimeSeriesData
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
    
    Returns:
        (xr.DataArray): returned result
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    result = uchronia_so.ToStructSingleTimeSeriesData(timeSeries_xptr.ptr)
    return opaque_ts_as_xarray_time_series(result, dispose=True)


@check_exceptions
def CreateEnsembleTimeSeriesDataFromStruct_py(ensSeries:xr.DataArray) -> 'EnsemblePtrTimeSeries':
    """CreateEnsembleTimeSeriesDataFromStruct_py
    
    CreateEnsembleTimeSeriesDataFromStruct_py: generated wrapper function for API function CreateEnsembleTimeSeriesDataFromStruct
    
    Args:
        ensSeries (xr.DataArray): ensSeries
    
    Returns:
        ('EnsemblePtrTimeSeries'): returned result
    
    """
    ensSeries_tsd_ptr = marshal.as_native_time_series(ensSeries)
    result = uchronia_so.CreateEnsembleTimeSeriesDataFromStruct(ensSeries_tsd_ptr.ptr)
    # ensSeries_tsd_ptr - no cleanup needed?
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


@check_exceptions
def CreateSingleTimeSeriesDataFromStruct_py(timeSeries:xr.DataArray) -> 'TimeSeries':
    """CreateSingleTimeSeriesDataFromStruct_py
    
    CreateSingleTimeSeriesDataFromStruct_py: generated wrapper function for API function CreateSingleTimeSeriesDataFromStruct
    
    Args:
        timeSeries (xr.DataArray): timeSeries
    
    Returns:
        ('TimeSeries'): returned result
    
    """
    timeSeries_tsd_ptr = marshal.as_native_time_series(timeSeries)
    result = uchronia_so.CreateSingleTimeSeriesDataFromStruct(timeSeries_tsd_ptr.ptr)
    # timeSeries_tsd_ptr - no cleanup needed?
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


@check_exceptions
def DisposeMultiTimeSeriesData_py(data:xr.DataArray) -> None:
    """DisposeMultiTimeSeriesData_py
    
    DisposeMultiTimeSeriesData_py: generated wrapper function for API function DisposeMultiTimeSeriesData
    
    Args:
        data (xr.DataArray): data
    
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)


@check_exceptions
def GetTimeSeriesGeometry_py(timeSeries:'TimeSeries', geom:TimeSeriesGeometryNative) -> None:
    """GetTimeSeriesGeometry_py
    
    GetTimeSeriesGeometry_py: generated wrapper function for API function GetTimeSeriesGeometry
    
    Args:
        timeSeries ('TimeSeries'): timeSeries
        geom (TimeSeriesGeometryNative): geom
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    geom_xptr = wrap_as_pointer_handle(geom)
    uchronia_so.GetTimeSeriesGeometry(timeSeries_xptr.ptr, geom_xptr.ptr)


@check_exceptions
def GetEnsembleForecastTimeSeriesGeometry_py(timeSeries:'EnsembleForecastTimeSeries', geom:TimeSeriesGeometryNative) -> None:
    """GetEnsembleForecastTimeSeriesGeometry_py
    
    GetEnsembleForecastTimeSeriesGeometry_py: generated wrapper function for API function GetEnsembleForecastTimeSeriesGeometry
    
    Args:
        timeSeries ('EnsembleForecastTimeSeries'): timeSeries
        geom (TimeSeriesGeometryNative): geom
    
    """
    timeSeries_xptr = wrap_as_pointer_handle(timeSeries)
    geom_xptr = wrap_as_pointer_handle(geom)
    uchronia_so.GetEnsembleForecastTimeSeriesGeometry(timeSeries_xptr.ptr, geom_xptr.ptr)


@check_exceptions
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
    uchronia_so.GetTimeSeriesValues(timeSeries_xptr.ptr, values_numarray.ptr, arrayLength)
    # values_numarray - no cleanup needed?


@check_exceptions
def GetNumTimeSeries_py() -> int:
    """GetNumTimeSeries_py
    
    GetNumTimeSeries_py: generated wrapper function for API function GetNumTimeSeries
    
    Args:
    
    Returns:
        (int): returned result
    
    """
    result = uchronia_so.GetNumTimeSeries()
    return result


@check_exceptions
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
    uchronia_so.GetProviderTsGeometry(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr, geom_xptr.ptr)
    # no cleanup for const char*


@check_exceptions
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
    uchronia_so.GetProviderTimeSeriesValues(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr, values_numarray.ptr, arrayLength)
    # no cleanup for const char*
    # values_numarray - no cleanup needed?



def GetProviderTimeSeriesIdentifiers_py(dataLibrary:'TimeSeriesProvider'):
    """GetProviderTimeSeriesIdentifiers_py
    
    GetProviderTimeSeriesIdentifiers_py: generated wrapper function for API function GetProviderTimeSeriesIdentifiers
    
    
    """

    dataLibrary_xptr = wrap_as_pointer_handle(dataLibrary)

    size = marshal.new_int_scalar_ptr()
    values = uchronia_so.GetProviderTimeSeriesIdentifiers(dataLibrary_xptr.ptr, size)
    

    result = charp_array_to_py(values, size[0], True)
    return result

@check_exceptions
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
    result = uchronia_so.TimeSeriesFromProviderTs(dataLibrary_xptr.ptr, variableIdentifier_c_charp.ptr)
    # no cleanup for const char*
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


