/** \file extern_c_api.h
 * API for libswift.
 * This file exposes a common API for interacting with libswift from other
 * tools.
*/

#pragma once

#include "setup_exports.h"

#ifndef DATATYPES_USE_OPAQUE_POINTERS
#ifndef DATATYPES_USE_CPP_POINTERS
#ifndef USING_DATATYPES_CORE
#define DATATYPES_USE_CPP_POINTERS
#endif
#endif
#endif

#include "interop_struct.h"

#if defined(DATATYPES_USE_CPP_POINTERS)

#include "datatypes/extern_c_api_as_transparent.h"

#elif defined(DATATYPES_USE_OPAQUE_POINTERS)

#include "datatypes/extern_c_api_as_opaque.h"

#else
#error macro DATATYPES_USE_OPAQUE_POINTERS or DATATYPES_USE_CPP_POINTERS must be defined
#endif

// see http://msdn.microsoft.com/en-us/library/as6wyhwt.aspx, best practice
#define DATATYPES_API  DATATYPES_DLL_LIB 

#ifdef __cplusplus
extern "C" {
#endif

	////////////////////////////////
	// Error handling
	////////////////////////////////

	/**
	 * \fn	DATATYPES_API char* GetLastStdExceptionMessage();
	 *
	 * \brief	Gets the last message from an std::exception caught by the SWIFT API.
	 *
	 * \return        the last standard exception message.
	 */
	DATATYPES_API char* GetLastStdExceptionMessage();

	/**
	 * \fn	DATATYPES_API void RegisterExceptionCallback(void* callback);
	 *
	 * \brief	Registers a function to call when SWIFT has generated a std::exception that has been caught by the API.
	 *
	 * \param [in]	callback function pointer. The function must have a signature like void on_error_message(const char* msg);
	 */
	DATATYPES_API void RegisterExceptionCallback(const void* callback);

	////////////////////////////////
	// Generic memory management.
	////////////////////////////////

	/**
	 * \fn	DATATYPES_API void DisposeSharedPointer(VOID_PTR_PROVIDER_PTR ptr);
	 *
	 * \brief	Notifies SWIFT that an object managed by an opaque pointer is not used by the caller anymore.
	 *
	 * \param	ptr	pointer obtained via the API, such as a MODEL_SIMULATION_PTR.
	 */

	DATATYPES_API void DisposeSharedPointer(VOID_PTR_PROVIDER_PTR ptr);

	/**
	 * \fn	DATATYPES_API void DeleteAnsiStringArray(char** values, int arrayLength);
	 *
	 * \brief	Deletes the ANSI string array , which has been create by SWIFT. Do not use for char** created outside libswift.
	 *
	 * \param [in]	values	pointer to the array to delete (its elements and the array itself).
	 * \param	arrayLength   	Length of the array.
	 */

	DATATYPES_API void DeleteAnsiStringArray(char** values, int arrayLength);

	/**
	 * \fn	DATATYPES_API void DeleteAnsiString(const char* value);
	 *
	 * \brief	Deletes the ANSI string which has been create by SWIFT. Do not use for char* created outside libswift.
	 *
	 * \param [in]	value	a C-style string, which has been create by SWIFT. Do not use for char* created elsewhere.
	 */

	DATATYPES_API void DeleteAnsiString(const char* value);

	DATATYPES_API void DeleteDoubleArray(double* value);

	////////////////////////////////
	// Global settings for the C SWIFT API
	////////////////////////////////

	DATATYPES_API void SetTimeSeriesMissingValueValue(double missingValueValue);

	////////////////////////////////
	// Model simulations
	////////////////////////////////

	/**
	 * \fn	DATATYPES_API MODEL_SIMULATION_PTR CloneModel(MODEL_SIMULATION_PTR simulation);
	 *
	 * \brief	Clone a model simulation
	 *
	 * \param	simulation	the simulation to clone.
	 *
	 * \return	A MODEL_SIMULATION_PTR.
	 */

	DATATYPES_API ENSEMBLE_DATA_SET_PTR LoadEnsembleDataset(const char* filename, const char* dataPath);

	DATATYPES_API ENSEMBLE_DATA_SET_PTR CreateEnsembleDataset(const char* type);

	DATATYPES_API char** GetEnsembleDatasetDataIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size);

	DATATYPES_API char** GetEnsembleDatasetDataSubIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataCollectionId, int* size);

	DATATYPES_API char** GetEnsembleDatasetDataSummaries(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size);

	DATATYPES_API time_series_dimensions_description* GetDataDimensionsDescription(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);

	DATATYPES_API void DisposeDataDimensionsDescriptions(time_series_dimensions_description* data);

	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR CreateEnsembleForecastTimeSeries(date_time_to_second start, int length, const char* timeStepName);



	DATATYPES_API TIME_SERIES_PTR GetDatasetSingleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);
	DATATYPES_API ENSEMBLE_PTR_TIME_SERIES_PTR GetDatasetEnsembleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataEnsembleId);
	// DATATYPES_API FORECAST_TIME_SERIES_DOUBLE_PTR GetDatasetForecastTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, char** identifiers, char** dataId, int size);
	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR GetDatasetEnsembleForecastTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);


	////////////////
	//Functions that reduce the dimensionality of data, forms of splicing
	////////////////
	DATATYPES_API ENSEMBLE_PTR_TIME_SERIES_PTR GetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR efts, int i);
	DATATYPES_API TIME_SERIES_PTR TimeSeriesFromEnsembleOfTimeSeries(ENSEMBLE_PTR_TIME_SERIES_PTR collectionTs, int index);
	DATATYPES_API TIME_SERIES_PTR TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR efts, int indexInIssueTime, int indexInForecastTime );

	////////////////
	// Functions that augment the dimensionality of data
	////////////////
	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR CreatePerfectForecastTimeSeries(TIME_SERIES_PTR observations, date_time_to_second start, int length, const char* timeStepName, int offsetForecasts, int leadTime);

	////////////////
	// Handling elements in ensemble time series forecasts.
	////////////////
	
	//DATATYPES_API void SetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, ENSEMBLE_FORECAST_TIME_SERIES_PTR data);
	DATATYPES_API void SetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, multi_regular_time_series_data* values);
	DATATYPES_API multi_regular_time_series_data* GetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i);

	////////////////
	// conversions to structures for conversions to other representations in the language using this C API
	////////////////
	DATATYPES_API multi_regular_time_series_data* ToStructEnsembleTimeSeriesData(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries);
	DATATYPES_API multi_regular_time_series_data* ToStructSingleTimeSeriesData(TIME_SERIES_PTR timeSeries);
	DATATYPES_API void DisposeMultiTimeSeriesData(multi_regular_time_series_data* data);

	DATATYPES_API void GetTimeSeriesGeometry(TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom);
	DATATYPES_API void GetEnsembleForecastTimeSeriesGeometry(ENSEMBLE_FORECAST_TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom);
	DATATYPES_API void GetTimeSeriesValues(TIME_SERIES_PTR timeSeries, double * values, int arrayLength);
	DATATYPES_API int GetNumTimeSeries();
	////////////////////////////////
	// Backward compatibility with SWIFTv1, but functions that may be generic to other persistent data sources
	////////////////////////////////

	DATATYPES_API void GetProviderTsGeometry(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, TS_GEOMETRY_PTR geom);
	DATATYPES_API void GetProviderTimeSeriesValues(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, double * values, int arrayLength);
	DATATYPES_API char** GetProviderTimeSeriesIdentifiers(TIME_SERIES_PROVIDER_PTR dataLibrary, int* size);

	////////////////////////////////
	// Obsolete and deprecated functions
	////////////////////////////////

#ifdef __cplusplus
}
#endif
