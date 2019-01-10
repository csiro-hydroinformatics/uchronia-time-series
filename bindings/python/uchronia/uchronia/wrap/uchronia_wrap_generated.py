##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################



from refcount.interop import *


def custom_wrap_cffi_native_handle(obj, type_id="", release_callback = None):
    if release_callback is None:
        release_callback = DisposeSharedPointer_py
    return wrap_cffi_native_handle(obj, type_id, release_callback)

def GetLastStdExceptionMessage_py():
    """
    GetLastStdExceptionMessage_py
    
    GetLastStdExceptionMessage_py Wrapper function for GetLastStdExceptionMessage
    
    
    """
    result = uchronia_so.GetLastStdExceptionMessage()
    return custom_wrap_cffi_native_handle(result, 'char*')


def RegisterExceptionCallback_py(callback):
    """
    RegisterExceptionCallback_py
    
    RegisterExceptionCallback_py Wrapper function for RegisterExceptionCallback
    
     callback Python type equivalent for C++ type const void*
    
    """
    uchronia_so.RegisterExceptionCallback(callback)


def DisposeSharedPointer_py(ptr):
    """
    DisposeSharedPointer_py
    
    DisposeSharedPointer_py Wrapper function for DisposeSharedPointer
    
     ptr Python type equivalent for C++ type VOID_PTR_PROVIDER_PTR
    
    """
    ptr_xptr = unwrap_cffi_native_handle(ptr)
    uchronia_so.DisposeSharedPointer(ptr_xptr)


def DeleteAnsiStringArray_py(values, arrayLength):
    """
    DeleteAnsiStringArray_py
    
    DeleteAnsiStringArray_py Wrapper function for DeleteAnsiStringArray
    
     values Python type equivalent for C++ type char**
     arrayLength Python type equivalent for C++ type int
    
    """
    values_charpp = to_c_char_ptrptr(values)
    uchronia_so.DeleteAnsiStringArray(values_charpp, arrayLength)


def DeleteAnsiString_py(value):
    """
    DeleteAnsiString_py
    
    DeleteAnsiString_py Wrapper function for DeleteAnsiString
    
     value Python type equivalent for C++ type const char*
    
    """
    uchronia_so.DeleteAnsiString(value)


def DeleteDoubleArray_py(value):
    """
    DeleteDoubleArray_py
    
    DeleteDoubleArray_py Wrapper function for DeleteDoubleArray
    
     value Python type equivalent for C++ type double*
    
    """
    uchronia_so.DeleteDoubleArray(value)


def SetTimeSeriesMissingValueValue_py(missingValueValue):
    """
    SetTimeSeriesMissingValueValue_py
    
    SetTimeSeriesMissingValueValue_py Wrapper function for SetTimeSeriesMissingValueValue
    
     missingValueValue Python type equivalent for C++ type double
    
    """
    uchronia_so.SetTimeSeriesMissingValueValue(missingValueValue)


def LoadEnsembleDataset_py(libraryIdentifier, dataPath):
    """
    LoadEnsembleDataset_py
    
    LoadEnsembleDataset_py Wrapper function for LoadEnsembleDataset
    
     libraryIdentifier Python type equivalent for C++ type const char*
     dataPath Python type equivalent for C++ type const char*
    
    """
    result = uchronia_so.LoadEnsembleDataset(libraryIdentifier, dataPath)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')


def CreateEnsembleDataset_py(type):
    """
    CreateEnsembleDataset_py
    
    CreateEnsembleDataset_py Wrapper function for CreateEnsembleDataset
    
     type Python type equivalent for C++ type const char*
    
    """
    result = uchronia_so.CreateEnsembleDataset(type)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_DATA_SET_PTR')



def GetEnsembleDatasetDataIdentifiers_py(dataLibrary):
    """
    GetEnsembleDatasetDataIdentifiers_py
    
    GetEnsembleDatasetDataIdentifiers_py Wrapper function for GetEnsembleDatasetDataIdentifiers
    
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetEnsembleDatasetDataIdentifiers(dataLibrary)
    return(custom_wrap_cffi_native_handle(result,'dummytype'))


def GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary, dataCollectionId):
    """
    GetEnsembleDatasetDataSubIdentifiers_py
    
    GetEnsembleDatasetDataSubIdentifiers_py Wrapper function for GetEnsembleDatasetDataSubIdentifiers
    
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetEnsembleDatasetDataSubIdentifiers(dataLibrary, dataCollectionId)
    return(custom_wrap_cffi_native_handle(result,'dummytype'))


def GetEnsembleDatasetDataSummaries_py(dataLibrary):
    """
    GetEnsembleDatasetDataSummaries_py
    
    GetEnsembleDatasetDataSummaries_py Wrapper function for GetEnsembleDatasetDataSummaries
    
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetEnsembleDatasetDataSummaries(dataLibrary)
    return(custom_wrap_cffi_native_handle(result,'dummytype'))

def GetDataDimensionsDescription_py(dataLibrary, dataId):
    """
    GetDataDimensionsDescription_py
    
    GetDataDimensionsDescription_py Wrapper function for GetDataDimensionsDescription
    
     dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
     dataId Python type equivalent for C++ type const char*
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetDataDimensionsDescription(dataLibrary_xptr, dataId)
    return custom_wrap_cffi_native_handle(result, 'time_series_dimensions_description*')


def EnsembleSizeEnsembleTimeSeries_py(ensSeries):
    """
    EnsembleSizeEnsembleTimeSeries_py
    
    EnsembleSizeEnsembleTimeSeries_py Wrapper function for EnsembleSizeEnsembleTimeSeries
    
     ensSeries Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
    
    """
    ensSeries_xptr = unwrap_cffi_native_handle(ensSeries)
    result = uchronia_so.EnsembleSizeEnsembleTimeSeries(ensSeries_xptr)
    return custom_wrap_cffi_native_handle(result, 'int')


def DisposeDataDimensionsDescriptions_py(data):
    """
    DisposeDataDimensionsDescriptions_py
    
    DisposeDataDimensionsDescriptions_py Wrapper function for DisposeDataDimensionsDescriptions
    
     data Python type equivalent for C++ type time_series_dimensions_description*
    
    """
    uchronia_so.DisposeDataDimensionsDescriptions(data)


def CreateEnsembleForecastTimeSeries_py(start, length, timeStepName):
    """
    CreateEnsembleForecastTimeSeries_py
    
    CreateEnsembleForecastTimeSeries_py Wrapper function for CreateEnsembleForecastTimeSeries
    
     start Python type equivalent for C++ type date_time_to_second
     length Python type equivalent for C++ type int
     timeStepName Python type equivalent for C++ type const char*
    
    """
    start_datetime = to_date_time_to_second(start)
    result = uchronia_so.CreateEnsembleForecastTimeSeries(start_datetime, length, timeStepName)
    # start_datetime - no cleanup needed
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


def GetDatasetSingleTimeSeries_py(dataLibrary, dataId):
    """
    GetDatasetSingleTimeSeries_py
    
    GetDatasetSingleTimeSeries_py Wrapper function for GetDatasetSingleTimeSeries
    
     dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
     dataId Python type equivalent for C++ type const char*
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetDatasetSingleTimeSeries(dataLibrary_xptr, dataId)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


def GetDatasetEnsembleTimeSeries_py(dataLibrary, dataEnsembleId):
    """
    GetDatasetEnsembleTimeSeries_py
    
    GetDatasetEnsembleTimeSeries_py Wrapper function for GetDatasetEnsembleTimeSeries
    
     dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
     dataEnsembleId Python type equivalent for C++ type const char*
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetDatasetEnsembleTimeSeries(dataLibrary_xptr, dataEnsembleId)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


def GetDatasetEnsembleForecastTimeSeries_py(dataLibrary, dataId):
    """
    GetDatasetEnsembleForecastTimeSeries_py
    
    GetDatasetEnsembleForecastTimeSeries_py Wrapper function for GetDatasetEnsembleForecastTimeSeries
    
     dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
     dataId Python type equivalent for C++ type const char*
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetDatasetEnsembleForecastTimeSeries(dataLibrary_xptr, dataId)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


def SaveSingleTimeSeriesToNetcdf_py(timeSeries, filename, overwrite):
    """
    SaveSingleTimeSeriesToNetcdf_py
    
    SaveSingleTimeSeriesToNetcdf_py Wrapper function for SaveSingleTimeSeriesToNetcdf
    
     timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
     filename Python type equivalent for C++ type const char*
     overwrite Python type equivalent for C++ type bool
    
    """
    timeSeries_xptr = unwrap_cffi_native_handle(timeSeries)
    uchronia_so.SaveSingleTimeSeriesToNetcdf(timeSeries_xptr, filename, overwrite)


def SaveEnsembleTimeSeriesToNetcdf_py(collection, filename, overwrite):
    """
    SaveEnsembleTimeSeriesToNetcdf_py
    
    SaveEnsembleTimeSeriesToNetcdf_py Wrapper function for SaveEnsembleTimeSeriesToNetcdf
    
     collection Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
     filename Python type equivalent for C++ type const char*
     overwrite Python type equivalent for C++ type bool
    
    """
    collection_xptr = unwrap_cffi_native_handle(collection)
    uchronia_so.SaveEnsembleTimeSeriesToNetcdf(collection_xptr, filename, overwrite)


def SaveEnsembleForecastTimeSeriesToNetcdf_py(tsEnsTs, filename, overwrite):
    """
    SaveEnsembleForecastTimeSeriesToNetcdf_py
    
    SaveEnsembleForecastTimeSeriesToNetcdf_py Wrapper function for SaveEnsembleForecastTimeSeriesToNetcdf
    
     tsEnsTs Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     filename Python type equivalent for C++ type const char*
     overwrite Python type equivalent for C++ type bool
    
    """
    tsEnsTs_xptr = unwrap_cffi_native_handle(tsEnsTs)
    uchronia_so.SaveEnsembleForecastTimeSeriesToNetcdf(tsEnsTs_xptr, filename, overwrite)


def IsMissingValueItemEnsembleForecastTimeSeries_py(series, i):
    """
    IsMissingValueItemEnsembleForecastTimeSeries_py
    
    IsMissingValueItemEnsembleForecastTimeSeries_py Wrapper function for IsMissingValueItemEnsembleForecastTimeSeries
    
     series Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
    
    """
    series_xptr = unwrap_cffi_native_handle(series)
    result = uchronia_so.IsMissingValueItemEnsembleForecastTimeSeries(series_xptr, i)
    return custom_wrap_cffi_native_handle(result, 'bool')


def GetItemEnsembleForecastTimeSeries_py(efts, i):
    """
    GetItemEnsembleForecastTimeSeries_py
    
    GetItemEnsembleForecastTimeSeries_py Wrapper function for GetItemEnsembleForecastTimeSeries
    
     efts Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
    
    """
    efts_xptr = unwrap_cffi_native_handle(efts)
    result = uchronia_so.GetItemEnsembleForecastTimeSeries(efts_xptr, i)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


def TimeSeriesFromEnsembleOfTimeSeries_py(collectionTs, index):
    """
    TimeSeriesFromEnsembleOfTimeSeries_py
    
    TimeSeriesFromEnsembleOfTimeSeries_py Wrapper function for TimeSeriesFromEnsembleOfTimeSeries
    
     collectionTs Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
     index Python type equivalent for C++ type int
    
    """
    collectionTs_xptr = unwrap_cffi_native_handle(collectionTs)
    result = uchronia_so.TimeSeriesFromEnsembleOfTimeSeries(collectionTs_xptr, index)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


def TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py(efts, indexInIssueTime, indexInForecastTime):
    """
    TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py
    
    TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_py Wrapper function for TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries
    
     efts Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     indexInIssueTime Python type equivalent for C++ type int
     indexInForecastTime Python type equivalent for C++ type int
    
    """
    efts_xptr = unwrap_cffi_native_handle(efts)
    result = uchronia_so.TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(efts_xptr, indexInIssueTime, indexInForecastTime)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


def GetValueFromUnivariateTimeSeries_py(ts, index):
    """
    GetValueFromUnivariateTimeSeries_py
    
    GetValueFromUnivariateTimeSeries_py Wrapper function for GetValueFromUnivariateTimeSeries
    
     ts Python type equivalent for C++ type TIME_SERIES_PTR
     index Python type equivalent for C++ type int
    
    """
    ts_xptr = unwrap_cffi_native_handle(ts)
    result = uchronia_so.GetValueFromUnivariateTimeSeries(ts_xptr, index)
    return custom_wrap_cffi_native_handle(result, 'double')


def TransformEachItem_py(tsEnsTs, method, methodArgument):
    """
    TransformEachItem_py
    
    TransformEachItem_py Wrapper function for TransformEachItem
    
     tsEnsTs Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     method Python type equivalent for C++ type const char*
     methodArgument Python type equivalent for C++ type const char*
    
    """
    tsEnsTs_xptr = unwrap_cffi_native_handle(tsEnsTs)
    uchronia_so.TransformEachItem(tsEnsTs_xptr, method, methodArgument)


def SetValueToUnivariateTimeSeries_py(ts, index, value):
    """
    SetValueToUnivariateTimeSeries_py
    
    SetValueToUnivariateTimeSeries_py Wrapper function for SetValueToUnivariateTimeSeries
    
     ts Python type equivalent for C++ type TIME_SERIES_PTR
     index Python type equivalent for C++ type int
     value Python type equivalent for C++ type double
    
    """
    ts_xptr = unwrap_cffi_native_handle(ts)
    uchronia_so.SetValueToUnivariateTimeSeries(ts_xptr, index, value)


def GetItemEnsembleForecastTimeSeriesAsStructure_py(series, i):
    """
    GetItemEnsembleForecastTimeSeriesAsStructure_py
    
    GetItemEnsembleForecastTimeSeriesAsStructure_py Wrapper function for GetItemEnsembleForecastTimeSeriesAsStructure
    
     series Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
    
    """
    series_xptr = unwrap_cffi_native_handle(series)
    result = uchronia_so.GetItemEnsembleForecastTimeSeriesAsStructure(series_xptr, i)
    return custom_wrap_cffi_native_handle(result, 'multi_regular_time_series_data*')


def GetItemEnsembleTimeSeriesAsStructure_py(series, i):
    """
    GetItemEnsembleTimeSeriesAsStructure_py
    
    GetItemEnsembleTimeSeriesAsStructure_py Wrapper function for GetItemEnsembleTimeSeriesAsStructure
    
     series Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
    
    """
    series_xptr = unwrap_cffi_native_handle(series)
    result = uchronia_so.GetItemEnsembleTimeSeriesAsStructure(series_xptr, i)
    return custom_wrap_cffi_native_handle(result, 'multi_regular_time_series_data*')


def SetItemEnsembleForecastTimeSeriesAsStructure_py(series, i, values):
    """
    SetItemEnsembleForecastTimeSeriesAsStructure_py
    
    SetItemEnsembleForecastTimeSeriesAsStructure_py Wrapper function for SetItemEnsembleForecastTimeSeriesAsStructure
    
     series Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
     values Python type equivalent for C++ type const multi_regular_time_series_data*
    
    """
    series_xptr = unwrap_cffi_native_handle(series)
    values_tsd_ptr = cinterop.timeseries.to_multi_regular_time_series_data(values)
    uchronia_so.SetItemEnsembleForecastTimeSeriesAsStructure(series_xptr, i, values_tsd_ptr)
    # cinterop::disposal::dispose_of<multi_regular_time_series_data>(values_tsd_ptr_x)


def SetItemEnsembleTimeSeriesAsStructure_py(collection, i, values):
    """
    SetItemEnsembleTimeSeriesAsStructure_py
    
    SetItemEnsembleTimeSeriesAsStructure_py Wrapper function for SetItemEnsembleTimeSeriesAsStructure
    
     collection Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
     i Python type equivalent for C++ type int
     values Python type equivalent for C++ type const multi_regular_time_series_data*
    
    """
    collection_xptr = unwrap_cffi_native_handle(collection)
    values_tsd_ptr = cinterop.timeseries.to_multi_regular_time_series_data(values)
    uchronia_so.SetItemEnsembleTimeSeriesAsStructure(collection_xptr, i, values_tsd_ptr)
    # cinterop::disposal::dispose_of<multi_regular_time_series_data>(values_tsd_ptr_x)


def CreatePerfectForecastTimeSeries_py(observations, start, length, timeStepName, offsetForecasts, leadTime):
    """
    CreatePerfectForecastTimeSeries_py
    
    CreatePerfectForecastTimeSeries_py Wrapper function for CreatePerfectForecastTimeSeries
    
     observations Python type equivalent for C++ type TIME_SERIES_PTR
     start Python type equivalent for C++ type date_time_to_second
     length Python type equivalent for C++ type int
     timeStepName Python type equivalent for C++ type const char*
     offsetForecasts Python type equivalent for C++ type int
     leadTime Python type equivalent for C++ type int
    
    """
    observations_xptr = unwrap_cffi_native_handle(observations)
    start_datetime = to_date_time_to_second(start)
    result = uchronia_so.CreatePerfectForecastTimeSeries(observations_xptr, start_datetime, length, timeStepName, offsetForecasts, leadTime)
    # start_datetime - no cleanup needed
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


def ToStructEnsembleTimeSeriesData_py(ensSeries):
    """
    ToStructEnsembleTimeSeriesData_py
    
    ToStructEnsembleTimeSeriesData_py Wrapper function for ToStructEnsembleTimeSeriesData
    
     ensSeries Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
    
    """
    ensSeries_xptr = unwrap_cffi_native_handle(ensSeries)
    result = uchronia_so.ToStructEnsembleTimeSeriesData(ensSeries_xptr)
    return custom_wrap_cffi_native_handle(result, 'multi_regular_time_series_data*')


def ToStructSingleTimeSeriesData_py(timeSeries):
    """
    ToStructSingleTimeSeriesData_py
    
    ToStructSingleTimeSeriesData_py Wrapper function for ToStructSingleTimeSeriesData
    
     timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
    
    """
    timeSeries_xptr = unwrap_cffi_native_handle(timeSeries)
    result = uchronia_so.ToStructSingleTimeSeriesData(timeSeries_xptr)
    return custom_wrap_cffi_native_handle(result, 'multi_regular_time_series_data*')


def CreateEnsembleTimeSeriesDataFromStruct_py(ensSeries):
    """
    CreateEnsembleTimeSeriesDataFromStruct_py
    
    CreateEnsembleTimeSeriesDataFromStruct_py Wrapper function for CreateEnsembleTimeSeriesDataFromStruct
    
     ensSeries Python type equivalent for C++ type const multi_regular_time_series_data*
    
    """
    ensSeries_tsd_ptr = cinterop.timeseries.to_multi_regular_time_series_data(ensSeries)
    result = uchronia_so.CreateEnsembleTimeSeriesDataFromStruct(ensSeries_tsd_ptr)
    # cinterop::disposal::dispose_of<multi_regular_time_series_data>(ensSeries_tsd_ptr_x)
    return custom_wrap_cffi_native_handle(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


def CreateSingleTimeSeriesDataFromStruct_py(timeSeries):
    """
    CreateSingleTimeSeriesDataFromStruct_py
    
    CreateSingleTimeSeriesDataFromStruct_py Wrapper function for CreateSingleTimeSeriesDataFromStruct
    
     timeSeries Python type equivalent for C++ type const multi_regular_time_series_data*
    
    """
    timeSeries_tsd_ptr = cinterop.timeseries.to_multi_regular_time_series_data(timeSeries)
    result = uchronia_so.CreateSingleTimeSeriesDataFromStruct(timeSeries_tsd_ptr)
    # cinterop::disposal::dispose_of<multi_regular_time_series_data>(timeSeries_tsd_ptr_x)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


def DisposeMultiTimeSeriesData_py(data):
    """
    DisposeMultiTimeSeriesData_py
    
    DisposeMultiTimeSeriesData_py Wrapper function for DisposeMultiTimeSeriesData
    
     data Python type equivalent for C++ type multi_regular_time_series_data*
    
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)


def GetTimeSeriesGeometry_py(timeSeries, geom):
    """
    GetTimeSeriesGeometry_py
    
    GetTimeSeriesGeometry_py Wrapper function for GetTimeSeriesGeometry
    
     timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
     geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    
    """
    timeSeries_xptr = unwrap_cffi_native_handle(timeSeries)
    geom_tsgeom = cinterop.timeseries.to_regular_time_series_geometry_ptr(geom)
    uchronia_so.GetTimeSeriesGeometry(timeSeries_xptr, geom_tsgeom)
    # delete geom_tsgeom


def GetEnsembleForecastTimeSeriesGeometry_py(timeSeries, geom):
    """
    GetEnsembleForecastTimeSeriesGeometry_py
    
    GetEnsembleForecastTimeSeriesGeometry_py Wrapper function for GetEnsembleForecastTimeSeriesGeometry
    
     timeSeries Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
     geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    
    """
    timeSeries_xptr = unwrap_cffi_native_handle(timeSeries)
    geom_tsgeom = cinterop.timeseries.to_regular_time_series_geometry_ptr(geom)
    uchronia_so.GetEnsembleForecastTimeSeriesGeometry(timeSeries_xptr, geom_tsgeom)
    # delete geom_tsgeom


def GetTimeSeriesValues_py(timeSeries, values, arrayLength):
    """
    GetTimeSeriesValues_py
    
    GetTimeSeriesValues_py Wrapper function for GetTimeSeriesValues
    
     timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
     values Python type equivalent for C++ type double*
     arrayLength Python type equivalent for C++ type int
    
    """
    timeSeries_xptr = unwrap_cffi_native_handle(timeSeries)
    uchronia_so.GetTimeSeriesValues(timeSeries_xptr, values, arrayLength)


def GetNumTimeSeries_py():
    """
    GetNumTimeSeries_py
    
    GetNumTimeSeries_py Wrapper function for GetNumTimeSeries
    
    
    """
    result = uchronia_so.GetNumTimeSeries()
    return custom_wrap_cffi_native_handle(result, 'int')


def GetProviderTsGeometry_py(dataLibrary, variableIdentifier, geom):
    """
    GetProviderTsGeometry_py
    
    GetProviderTsGeometry_py Wrapper function for GetProviderTsGeometry
    
     dataLibrary Python type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
     variableIdentifier Python type equivalent for C++ type const char*
     geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    geom_tsgeom = cinterop.timeseries.to_regular_time_series_geometry_ptr(geom)
    uchronia_so.GetProviderTsGeometry(dataLibrary_xptr, variableIdentifier, geom_tsgeom)
    # delete geom_tsgeom


def GetProviderTimeSeriesValues_py(dataLibrary, variableIdentifier, values, arrayLength):
    """
    GetProviderTimeSeriesValues_py
    
    GetProviderTimeSeriesValues_py Wrapper function for GetProviderTimeSeriesValues
    
     dataLibrary Python type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
     variableIdentifier Python type equivalent for C++ type const char*
     values Python type equivalent for C++ type double*
     arrayLength Python type equivalent for C++ type int
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    uchronia_so.GetProviderTimeSeriesValues(dataLibrary_xptr, variableIdentifier, values, arrayLength)



def GetProviderTimeSeriesIdentifiers_py(dataLibrary):
    """
    GetProviderTimeSeriesIdentifiers_py
    
    GetProviderTimeSeriesIdentifiers_py Wrapper function for GetProviderTimeSeriesIdentifiers
    
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.GetProviderTimeSeriesIdentifiers(dataLibrary)
    return(custom_wrap_cffi_native_handle(result,'dummytype'))

def TimeSeriesFromProviderTs_py(dataLibrary, variableIdentifier):
    """
    TimeSeriesFromProviderTs_py
    
    TimeSeriesFromProviderTs_py Wrapper function for TimeSeriesFromProviderTs
    
     dataLibrary Python type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
     variableIdentifier Python type equivalent for C++ type const char*
    
    """
    dataLibrary_xptr = unwrap_cffi_native_handle(dataLibrary)
    result = uchronia_so.TimeSeriesFromProviderTs(dataLibrary_xptr, variableIdentifier)
    return custom_wrap_cffi_native_handle(result, 'TIME_SERIES_PTR')


