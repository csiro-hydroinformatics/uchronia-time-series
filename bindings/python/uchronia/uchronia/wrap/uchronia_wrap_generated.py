##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

def GetLastStdExceptionMessage_py():
    """
    GetLastStdExceptionMessage_py
    
    GetLastStdExceptionMessage_py Wrapper function for GetLastStdExceptionMessage
    
    :export
    """
    result = nativelib.GetLastStdExceptionMessage()
    return cinterop.mkExternalObjRef(result, 'char*')


def RegisterExceptionCallback_py(callback):
    """
    RegisterExceptionCallback_py
    
    RegisterExceptionCallback_py Wrapper function for RegisterExceptionCallback
    
    :param callback Python type equivalent for C++ type const void*
    :export
    """
    nativelib.RegisterExceptionCallback(callback)


def DisposeSharedPointer_py(ptr):
    """
    DisposeSharedPointer_py
    
    DisposeSharedPointer_py Wrapper function for DisposeSharedPointer
    
    :param ptr Python type equivalent for C++ type VOID_PTR_PROVIDER_PTR
    :export
    """
    ptr_xptr = cinterop.getExternalXptr(ptr)
    nativelib.DisposeSharedPointer(ptr_xptr)


def DeleteAnsiStringArray_py(values, arrayLength):
    """
    DeleteAnsiStringArray_py
    
    DeleteAnsiStringArray_py Wrapper function for DeleteAnsiStringArray
    
    :param values Python type equivalent for C++ type char**
    :param arrayLength Python type equivalent for C++ type int
    :export
    """
    values_charpp = to_c_char_ptrptr(values)
    nativelib.DeleteAnsiStringArray(values_charpp, arrayLength)


def DeleteAnsiString_py(value):
    """
    DeleteAnsiString_py
    
    DeleteAnsiString_py Wrapper function for DeleteAnsiString
    
    :param value Python type equivalent for C++ type const char*
    :export
    """
    nativelib.DeleteAnsiString(value)


def DeleteDoubleArray_py(value):
    """
    DeleteDoubleArray_py
    
    DeleteDoubleArray_py Wrapper function for DeleteDoubleArray
    
    :param value Python type equivalent for C++ type double*
    :export
    """
    nativelib.DeleteDoubleArray(value)


def SetTimeSeriesMissingValueValue_py(missingValueValue):
    """
    SetTimeSeriesMissingValueValue_py
    
    SetTimeSeriesMissingValueValue_py Wrapper function for SetTimeSeriesMissingValueValue
    
    :param missingValueValue Python type equivalent for C++ type double
    :export
    """
    nativelib.SetTimeSeriesMissingValueValue(missingValueValue)


def LoadEnsembleDataset_py(filename, dataPath):
    """
    LoadEnsembleDataset_py
    
    LoadEnsembleDataset_py Wrapper function for LoadEnsembleDataset
    
    :param filename Python type equivalent for C++ type const char*
    :param dataPath Python type equivalent for C++ type const char*
    :export
    """
    result = nativelib.LoadEnsembleDataset(filename, dataPath)
    return cinterop.mkExternalObjRef(result, 'ENSEMBLE_DATA_SET_PTR')


def CreateEnsembleDataset_py(type):
    """
    CreateEnsembleDataset_py
    
    CreateEnsembleDataset_py Wrapper function for CreateEnsembleDataset
    
    :param type Python type equivalent for C++ type const char*
    :export
    """
    result = nativelib.CreateEnsembleDataset(type)
    return cinterop.mkExternalObjRef(result, 'ENSEMBLE_DATA_SET_PTR')



def GetEnsembleDatasetDataIdentifiers_py(dataLibrary):
    """
    GetEnsembleDatasetDataIdentifiers_py
    
    GetEnsembleDatasetDataIdentifiers_py Wrapper function for GetEnsembleDatasetDataIdentifiers
    
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetEnsembleDatasetDataIdentifiers(dataLibrary)
    return(cinterop.mkExternalObjRef(result,'dummytype'))


def GetEnsembleDatasetDataSubIdentifiers_py(dataLibrary, dataCollectionId):
    """
    GetEnsembleDatasetDataSubIdentifiers_py
    
    GetEnsembleDatasetDataSubIdentifiers_py Wrapper function for GetEnsembleDatasetDataSubIdentifiers
    
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetEnsembleDatasetDataSubIdentifiers(dataLibrary, dataCollectionId)
    return(cinterop.mkExternalObjRef(result,'dummytype'))


def GetEnsembleDatasetDataSummaries_py(dataLibrary):
    """
    GetEnsembleDatasetDataSummaries_py
    
    GetEnsembleDatasetDataSummaries_py Wrapper function for GetEnsembleDatasetDataSummaries
    
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetEnsembleDatasetDataSummaries(dataLibrary)
    return(cinterop.mkExternalObjRef(result,'dummytype'))

def GetDataDimensionsDescription_py(dataLibrary, dataId):
    """
    GetDataDimensionsDescription_py
    
    GetDataDimensionsDescription_py Wrapper function for GetDataDimensionsDescription
    
    :param dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
    :param dataId Python type equivalent for C++ type const char*
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetDataDimensionsDescription(dataLibrary_xptr, dataId)
    return cinterop.mkExternalObjRef(result, 'time_series_dimensions_description*')


def DisposeDataDimensionsDescriptions_py(data):
    """
    DisposeDataDimensionsDescriptions_py
    
    DisposeDataDimensionsDescriptions_py Wrapper function for DisposeDataDimensionsDescriptions
    
    :param data Python type equivalent for C++ type time_series_dimensions_description*
    :export
    """
    nativelib.DisposeDataDimensionsDescriptions(data)


def CreateEnsembleForecastTimeSeries_py(start, length, timeStepName):
    """
    CreateEnsembleForecastTimeSeries_py
    
    CreateEnsembleForecastTimeSeries_py Wrapper function for CreateEnsembleForecastTimeSeries
    
    :param start Python type equivalent for C++ type date_time_to_second
    :param length Python type equivalent for C++ type int
    :param timeStepName Python type equivalent for C++ type const char*
    :export
    """
    result = nativelib.CreateEnsembleForecastTimeSeries(start, length, timeStepName)
    return cinterop.mkExternalObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


def SetItemEnsembleForecastTimeSeries_py(series, i, values):
    """
    SetItemEnsembleForecastTimeSeries_py
    
    SetItemEnsembleForecastTimeSeries_py Wrapper function for SetItemEnsembleForecastTimeSeries
    
    :param series Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
    :param i Python type equivalent for C++ type int
    :param values Python type equivalent for C++ type multi_regular_time_series_data*
    :export
    """
    series_xptr = cinterop.getExternalXptr(series)
    nativelib.SetItemEnsembleForecastTimeSeries(series_xptr, i, values)


def GetItemEnsembleForecastTimeSeries_py(series, i):
    """
    GetItemEnsembleForecastTimeSeries_py
    
    GetItemEnsembleForecastTimeSeries_py Wrapper function for GetItemEnsembleForecastTimeSeries
    
    :param series Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
    :param i Python type equivalent for C++ type int
    :export
    """
    series_xptr = cinterop.getExternalXptr(series)
    result = nativelib.GetItemEnsembleForecastTimeSeries(series_xptr, i)
    return cinterop.mkExternalObjRef(result, 'multi_regular_time_series_data*')


def DisposeMultiTimeSeriesData_py(data):
    """
    DisposeMultiTimeSeriesData_py
    
    DisposeMultiTimeSeriesData_py Wrapper function for DisposeMultiTimeSeriesData
    
    :param data Python type equivalent for C++ type multi_regular_time_series_data*
    :export
    """
    nativelib.DisposeMultiTimeSeriesData(data)


def GetDatasetSingleTimeSeries_py(dataLibrary, dataId):
    """
    GetDatasetSingleTimeSeries_py
    
    GetDatasetSingleTimeSeries_py Wrapper function for GetDatasetSingleTimeSeries
    
    :param dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
    :param dataId Python type equivalent for C++ type const char*
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetDatasetSingleTimeSeries(dataLibrary_xptr, dataId)
    return cinterop.mkExternalObjRef(result, 'TIME_SERIES_PTR')


def GetDatasetEnsembleTimeSeries_py(dataLibrary, dataEnsembleId):
    """
    GetDatasetEnsembleTimeSeries_py
    
    GetDatasetEnsembleTimeSeries_py Wrapper function for GetDatasetEnsembleTimeSeries
    
    :param dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
    :param dataEnsembleId Python type equivalent for C++ type const char*
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetDatasetEnsembleTimeSeries(dataLibrary_xptr, dataEnsembleId)
    return cinterop.mkExternalObjRef(result, 'ENSEMBLE_PTR_TIME_SERIES_PTR')


def GetEnsembleTimeSeriesData_py(ensSeries):
    """
    GetEnsembleTimeSeriesData_py
    
    GetEnsembleTimeSeriesData_py Wrapper function for GetEnsembleTimeSeriesData
    
    :param ensSeries Python type equivalent for C++ type ENSEMBLE_PTR_TIME_SERIES_PTR
    :export
    """
    ensSeries_xptr = cinterop.getExternalXptr(ensSeries)
    result = nativelib.GetEnsembleTimeSeriesData(ensSeries_xptr)
    return cinterop.mkExternalObjRef(result, 'multi_regular_time_series_data*')


def GetDatasetEnsembleForecastTimeSeries_py(dataLibrary, dataId):
    """
    GetDatasetEnsembleForecastTimeSeries_py
    
    GetDatasetEnsembleForecastTimeSeries_py Wrapper function for GetDatasetEnsembleForecastTimeSeries
    
    :param dataLibrary Python type equivalent for C++ type ENSEMBLE_DATA_SET_PTR
    :param dataId Python type equivalent for C++ type const char*
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetDatasetEnsembleForecastTimeSeries(dataLibrary_xptr, dataId)
    return cinterop.mkExternalObjRef(result, 'ENSEMBLE_FORECAST_TIME_SERIES_PTR')


def GetTimeSeriesGeometry_py(timeSeries, geom):
    """
    GetTimeSeriesGeometry_py
    
    GetTimeSeriesGeometry_py Wrapper function for GetTimeSeriesGeometry
    
    :param timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
    :param geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    :export
    """
    timeSeries_xptr = cinterop.getExternalXptr(timeSeries)
    geom_xptr = cinterop.getExternalXptr(geom)
    nativelib.GetTimeSeriesGeometry(timeSeries_xptr, geom_xptr)


def GetEnsembleForecastTimeSeriesGeometry_py(timeSeries, geom):
    """
    GetEnsembleForecastTimeSeriesGeometry_py
    
    GetEnsembleForecastTimeSeriesGeometry_py Wrapper function for GetEnsembleForecastTimeSeriesGeometry
    
    :param timeSeries Python type equivalent for C++ type ENSEMBLE_FORECAST_TIME_SERIES_PTR
    :param geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    :export
    """
    timeSeries_xptr = cinterop.getExternalXptr(timeSeries)
    geom_xptr = cinterop.getExternalXptr(geom)
    nativelib.GetEnsembleForecastTimeSeriesGeometry(timeSeries_xptr, geom_xptr)


def GetTimeSeriesValues_py(timeSeries, values, arrayLength):
    """
    GetTimeSeriesValues_py
    
    GetTimeSeriesValues_py Wrapper function for GetTimeSeriesValues
    
    :param timeSeries Python type equivalent for C++ type TIME_SERIES_PTR
    :param values Python type equivalent for C++ type double*
    :param arrayLength Python type equivalent for C++ type int
    :export
    """
    timeSeries_xptr = cinterop.getExternalXptr(timeSeries)
    nativelib.GetTimeSeriesValues(timeSeries_xptr, values, arrayLength)


def GetNumTimeSeries_py():
    """
    GetNumTimeSeries_py
    
    GetNumTimeSeries_py Wrapper function for GetNumTimeSeries
    
    :export
    """
    result = nativelib.GetNumTimeSeries()
    return cinterop.mkExternalObjRef(result, 'int')


def GetProviderTsGeometry_py(dataLibrary, variableIdentifier, geom):
    """
    GetProviderTsGeometry_py
    
    GetProviderTsGeometry_py Wrapper function for GetProviderTsGeometry
    
    :param dataLibrary Python type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
    :param variableIdentifier Python type equivalent for C++ type const char*
    :param geom Python type equivalent for C++ type TS_GEOMETRY_PTR
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    geom_xptr = cinterop.getExternalXptr(geom)
    nativelib.GetProviderTsGeometry(dataLibrary_xptr, variableIdentifier, geom_xptr)


def GetProviderTimeSeriesValues_py(dataLibrary, variableIdentifier, values, arrayLength):
    """
    GetProviderTimeSeriesValues_py
    
    GetProviderTimeSeriesValues_py Wrapper function for GetProviderTimeSeriesValues
    
    :param dataLibrary Python type equivalent for C++ type TIME_SERIES_PROVIDER_PTR
    :param variableIdentifier Python type equivalent for C++ type const char*
    :param values Python type equivalent for C++ type double*
    :param arrayLength Python type equivalent for C++ type int
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    nativelib.GetProviderTimeSeriesValues(dataLibrary_xptr, variableIdentifier, values, arrayLength)



def GetProviderTimeSeriesIdentifiers_py(dataLibrary):
    """
    GetProviderTimeSeriesIdentifiers_py
    
    GetProviderTimeSeriesIdentifiers_py Wrapper function for GetProviderTimeSeriesIdentifiers
    
    :export
    """
    dataLibrary_xptr = cinterop.getExternalXptr(dataLibrary)
    result = nativelib.GetProviderTimeSeriesIdentifiers(dataLibrary)
    return(cinterop.mkExternalObjRef(result,'dummytype'))

