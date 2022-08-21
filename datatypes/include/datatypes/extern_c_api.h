/** \file extern_c_api.h
 * C API for uchronia (formerly 'datatypes').
 * This file exposes a common C API for interacting with uchronia from other
 * tools, typically foreign language bindings (Python, R, C# etc.)
*/

#pragma once

#include "datatypes/setup_exports.h"

#ifndef DATATYPES_USE_OPAQUE_POINTERS
#ifndef DATATYPES_USE_CPP_POINTERS
#ifndef USING_DATATYPES_CORE
#define DATATYPES_USE_CPP_POINTERS
#endif
#endif
#endif

#include "datatypes/interop_struct.h"

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
	 *
	 * \brief	Gets the last message from an std::exception caught by the uchronia API.
	 *
	 * \return        the last standard exception message.
	 */
	DATATYPES_API char* GetLastStdExceptionMessage();

	/**
	 *
	 * \brief	Registers a function to call when uchronia has generated a std::exception that has been caught by the API.
	 *
	 * \param [in]	callback function pointer. The function must have a signature like void on_error_message(const char* msg);
	 */
	DATATYPES_API void RegisterExceptionCallback(const void* callback);

	////////////////////////////////
	// Generic memory management.
	////////////////////////////////

	/**
	 *
	 * \brief	Notifies uchronia that an object managed by an opaque pointer is not used by the caller anymore.
	 *
	 * \param	ptr	pointer obtained via the API, such as a MODEL_SIMULATION_PTR.
	 */

	DATATYPES_API void DisposeSharedPointer(VOID_PTR_PROVIDER_PTR ptr);

	/**
	 *
	 * \brief	Deletes the ANSI string array , which has been create by uchronia. Do not use for char** created outside uchronia.
	 *
	 * \param [in]	values	pointer to the array to delete (its elements and the array itself).
	 * \param	arrayLength   	Length of the array.
	 */

	DATATYPES_API void DeleteAnsiStringArray(char** values, int arrayLength);

	/**
	 *
	 * \brief	Deletes the ANSI string which has been create by uchronia. Do not use for char* created outside uchronia.
	 *
	 * \param [in]	value	a C-style string, which has been create by uchronia. Do not use for char* created elsewhere.
	 */

	DATATYPES_API void DeleteAnsiString(const char* value);

	/**
	 *
	 * \brief	Dispose of an array of double created via this C API.
	 *
	 * \param [in,out]	value	If non-null, the value.
	 */

	DATATYPES_API void DeleteDoubleArray(double* value);


	////////////////////////////////
	// Global settings for the C uchronia API
	////////////////////////////////


	/**
	 *
	 * \brief	Sets a value whoch should be considered as a missing value when data is passed to this C API.
	 *
	 * \param	missingValueValue	The missing value.
	 */

	DATATYPES_API void SetTimeSeriesMissingValueValue(double missingValueValue);

	/**
	 *
	 * \brief	Creates a time series library, defined (for now) by a YAML descriptor.
	 *
	 * \param	libraryIdentifier	an ID for the library (currently path to a YAML file)
	 * \param	dataPath	optional root data path for on-disk data, if the YAML file uses relative paths.
	 *
	 * \return	The ensemble dataset.
	 */

	DATATYPES_API ENSEMBLE_DATA_SET_PTR LoadEnsembleDataset(const char* libraryIdentifier, const char* dataPath);

	/**
	 *
	 * \brief	Creates a time series library
	 *
	 * \param	type	The type of library to create. Currently supports cases for unit tests
	 * 					and time series libraries that can be "recorded to" by modelling engines.
	 *
	 * \return	The new ensemble dataset.
	 */

	DATATYPES_API ENSEMBLE_DATA_SET_PTR CreateEnsembleDataset(const char* type);

	/**
	 *
	 * \brief	Gets the highest level datasets' IDs known to this library.
	 *
	 * \param 		  	dataLibrary	The data library.
	 * \param [out]	size	   	Size of the list of IDs
	 *
	 * \return	Null if it fails, else the ensemble dataset data identifiers.
	 */

	DATATYPES_API char** GetEnsembleDatasetDataIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size);

	/**
	 *
	 * \brief	Gets sub identifiers, if any, in a hierarchical ID scheme 
	 * 			(e.g. if a collection of streamflows is such that you can ID each series as "streamflow.gauge_id")
	 *
	 * \param 		  	dataLibrary			The data library.
	 * \param 		  	dataCollectionId	Main identifier within which to query for sub-identifgiers
	 * \param [out]	size	   	Size of the list of sub-IDs
	 *
	 * \return	Null if it fails, else the dataset data sub identifiers. (e.g. the streamflow gauge idenfiers)
	 */

	DATATYPES_API char** GetEnsembleDatasetDataSubIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataCollectionId, int* size);

	/**
	 * \brief Get brief summary descriptions of each dataset in a data library
	 * 
	 * \param dataLibrary data library
	 * \param [out] size number of summaries returned 
	 * \return char** 
	 */
	DATATYPES_API char** GetEnsembleDatasetDataSummaries(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size);

	/**
	 *
	 * \brief	Gets data dimensions description. This function is useful for wrappers 
	 * 			to discover the dimensionality of a data set before trying to load it in memory.
	 *
	 * \param	dataLibrary	The data library.
	 * \param	dataId	   	Identifier for the data.
	 *
	 * \return	A struct describing the dimensionality of the data set in the library.
	 */

	DATATYPES_API time_series_dimensions_description* GetDataDimensionsDescription(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);

	/**
	 * \brief Gets the number of ensembles in an ensemble time series
	 * 
	 * \param ensSeries ensemble time series 
	 * \return number of ensembles 
	 */
	DATATYPES_API int EnsembleSizeEnsembleTimeSeries(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries);

	/**
	 * \brief Dispose of a time_series_dimensions_description previously returned by this API
	 * 
	 * \param data pointer to the struct to dispose of
	 */
	DATATYPES_API void DisposeDataDimensionsDescriptions(time_series_dimensions_description* data);

	/**
	 * \brief Create a Ensemble Forecast Time Series object. This is a time series of ensembles of time series.
	 * The conceptual dimensions are time (forecast issue time), ensemble, and forecast lead time.
	 * 
	 * \param start Start time of the time series along the forecast issue time dimension
	 * \param length Length of the time series along the forecast issue time dimension
	 * \param timeStepName time step identifier along the forecast issue time dimension. 
	 * Supported includes 'daily', 'hourly', 'monthly', and regular time durations such as '03:00:00'
	 * \return reference counted wrapper to an ensemble forecast time series.
	 */
	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR CreateEnsembleForecastTimeSeries(date_time_to_second start, int length, const char* timeStepName);

	/**
	 * \brief Gets a univariate Time Series out of a data library
	 * 
	 * \param dataLibrary data library
	 * \param dataId identifier
	 * \return reference counted wrapper to the new object.
	 */
	DATATYPES_API TIME_SERIES_PTR GetDatasetSingleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);

	/**
	 * \brief Gets a multivariate (ensemble) Time Series out of a data library
	 * 
	 * \param dataLibrary data library
	 * \param dataEnsembleId  identifier
	 * \return reference counted wrapper to the new object.
	 */
	DATATYPES_API ENSEMBLE_PTR_TIME_SERIES_PTR GetDatasetEnsembleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataEnsembleId);

	/**
	 * \brief Gets an ensemble forecast time serie (time series of ensemble time series ) out of a data library
	 * 
	 * \param dataLibrary data library
	 * \param dataId  identifier
	 * \return reference counted wrapper to the new object.
	 */
	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR GetDatasetEnsembleForecastTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId);

	/**
	 * \brief Save a time series to a netcdf file with a custom convention on disk. May be deprecated.
	 * 
	 * \param timeSeries data to save
	 * \param filename file name
	 * \param overwrite if true, overwrite if the file already exists. 
	 */
	DATATYPES_API void SaveSingleTimeSeriesToNetcdf(TIME_SERIES_PTR timeSeries, const char* filename, bool overwrite);
	/**
	 * \brief Save a multivariate time series to a netcdf file with a custom convention on disk. May be deprecated.
	 * 
	 * \param collection ensemble time series
	 * \param filename file name
	 * \param overwrite if true, overwrite if the file already exists. 
	 */
	DATATYPES_API void SaveEnsembleTimeSeriesToNetcdf(ENSEMBLE_PTR_TIME_SERIES_PTR collection, const char* filename, bool overwrite);
	/**
	 * \brief Save a ensemble forecast time series to a netcdf file with a custom convention on disk. May be deprecated.
	 * 
	 * \param tsEnsTs data to save
	 * \param filename file name
	 * \param overwrite if true, overwrite if the file already exists. 
	 */
	DATATYPES_API void SaveEnsembleForecastTimeSeriesToNetcdf(ENSEMBLE_FORECAST_TIME_SERIES_PTR tsEnsTs, const char* filename, bool overwrite);

	/**
	 * \brief Is a value missing, at an issue time in an ensemble forecast time series.
	 * 
	 * \param series ensemble forecast time series
	 * \param i item index to check
	 * \return true (non-zero) if the value is missing (e.g. nullptr i.e. no forecast for that issue time)
	 */
	DATATYPES_API bool IsMissingValueItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i);

	////////////////
	//Functions that reduce the dimensionality of data, forms of splicing
	////////////////

	/**
	 * \brief Get an item in an ensemble forecast time series object. See also \ref GetItemEnsembleForecastTimeSeriesAsStructure
	 * 
	 * \param efts ensemble forecast time series
	 * \param i index (issue time)
	 * \return a multivariate time series, the forecasts for issue time i
	 */
	DATATYPES_API ENSEMBLE_PTR_TIME_SERIES_PTR GetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR efts, int i);

	/**
	 * \brief Gets an univariate time series from an ensemble time series. See also \ref GetItemEnsembleTimeSeriesAsStructure
	 * 
	 * \param collectionTs ensemble time series
	 * \param index index in the ensemble dimension
	 * \return univariate time series
	 */
	DATATYPES_API TIME_SERIES_PTR TimeSeriesFromEnsembleOfTimeSeries(ENSEMBLE_PTR_TIME_SERIES_PTR collectionTs, int index);

	/**
	 * \brief Gets a forecast realisation in an ensemble forecast time series object
	 * 
	 * \param efts ensemble forecast time series object
	 * \param indexInIssueTime index (issue time)
	 * \param indexInForecastTime index in the ensemble dimension
	 * \return univariate time series
	 */
	DATATYPES_API TIME_SERIES_PTR TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR efts, int indexInIssueTime, int indexInForecastTime );
	/**
	 * \brief Get the value from univariate time series object
	 * 
	 * \param ts time series
	 * \param index index of interest
	 * \return time series value at that index
	 */
	DATATYPES_API double GetValueFromUnivariateTimeSeries(TIME_SERIES_PTR ts, int index);

	/**
	 * \brief Transform in-place each item (ensemble forecasts) of an ensemble forecast time series
	 * 
	 * \param tsEnsTs ensemble forecast time series
	 * \param method a valid transformation method: either 'aggregate' or 'disaggregate' at the time of writing. Exception thrown otherwise.
	 * \param methodArgument a string description of the arguments to the method. 
	 * Currently supported is the target time step in the aggregation/disaggregation, e.g. to three-hourly with '03:00:00'
	 */
	DATATYPES_API void TransformEachItem(ENSEMBLE_FORECAST_TIME_SERIES_PTR tsEnsTs, const char* method, const char* methodArgument);

	////////////////
	//Functions that set items in data (along the 'first' dimension)
	////////////////

	/**
	 * \brief Sets a value of an item of a time series
	 */
	DATATYPES_API void SetValueToUnivariateTimeSeries(TIME_SERIES_PTR ts, int index, double value);


	////////////////
	// Handling elements in ensemble time series forecasts.
	////////////////
	
	/**
	 * \brief Get the item of an ensemble forecast time series, as an interoperable struct
	 * 
	 * \param series ensemble forecast time series
	 * \param i item index
	 * \return pointer to a new multi_regular_time_series_data 
	 */
	DATATYPES_API multi_regular_time_series_data* GetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i);

	/**
	 * \brief Get the item of an ensemble time series, as an interoperable struct
	 * 
	 * \param series ensemble time series
	 * \param i item index
	 * \return pointer to a new multi_regular_time_series_data (univariate series in effect)
	 */
	DATATYPES_API multi_regular_time_series_data* GetItemEnsembleTimeSeriesAsStructure(ENSEMBLE_PTR_TIME_SERIES_PTR series, int i);
	//DATATYPES_API void SetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, ENSEMBLE_FORECAST_TIME_SERIES_PTR data);

	/**
	 * \brief Set the item of an ensemble forecast time series, as an interoperable struct
	 * 
	 * \param series ensemble forecast time series
	 * \param i item index
	 * \param values interoperable multi_regular_time_series_data, expected to represent an ensemble (multivariate) time series
	 */
	DATATYPES_API void SetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, const multi_regular_time_series_data*  values);

	/**
	 * \brief Set the item of an ensemble time series, as an interoperable struct
	 * 
	 * \param series ensemble time series
	 * \param i item index, along the ensemble dimension (i.e. one of the realisations of the ensemble)
	 * \param values interoperable multi_regular_time_series_data, expected to represent an univariate time series
	 */
	DATATYPES_API void SetItemEnsembleTimeSeriesAsStructure(ENSEMBLE_PTR_TIME_SERIES_PTR collection, int i, const multi_regular_time_series_data*  values);

	////////////////
	// Functions that augment the dimensionality of data
	////////////////
	DATATYPES_API ENSEMBLE_FORECAST_TIME_SERIES_PTR CreatePerfectForecastTimeSeries(TIME_SERIES_PTR observations, date_time_to_second start, int length, const char* timeStepName, int offsetForecasts, int leadTime);

	////////////////
	// conversions to structures for conversions to other representations in the language using this C API
	////////////////

	/**
	 * \brief Copy conversion to an interop struct representation
	 */
	DATATYPES_API multi_regular_time_series_data* ToStructEnsembleTimeSeriesData(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries);
	/**
	 * \brief Copy conversion to an interop struct representation
	 */
	DATATYPES_API multi_regular_time_series_data* ToStructSingleTimeSeriesData(TIME_SERIES_PTR timeSeries);
	/**
	 * \brief Copy conversion from an interop struct representation to an opaque object representation
	 */
	DATATYPES_API ENSEMBLE_PTR_TIME_SERIES_PTR CreateEnsembleTimeSeriesDataFromStruct(const multi_regular_time_series_data* ensSeries);
	/**
	 * \brief Copy conversion from an interop struct representation to an opaque object representation
	 */
	DATATYPES_API TIME_SERIES_PTR CreateSingleTimeSeriesDataFromStruct(const multi_regular_time_series_data* timeSeries);

	/**
	 * \brief Dispose of the memory held by a multi_regular_time_series_data struct. 
	 * This multi_regular_time_series_data must have been created via this library API, not elsewhere.
	 */
	DATATYPES_API void DisposeMultiTimeSeriesData(multi_regular_time_series_data* data);

	/**
	 * \brief Get the temporal geometry of a time series object
	 * 
	 * \param timeSeries time series
	 * \param geom pointer to a regular_time_series_geometry struct, whose values will be written to
	 */
	DATATYPES_API void GetTimeSeriesGeometry(TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom);
	/**
	 * \brief Get the temporal geometry of a the main time dimension of an ensemble forecast time series object
	 * 
	 * \param timeSeries ensemble forecast time series
	 * \param geom pointer to a regular_time_series_geometry struct, whose values will be written to
	 */
	DATATYPES_API void GetEnsembleForecastTimeSeriesGeometry(ENSEMBLE_FORECAST_TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom);

	/**
	 * \brief Retrieve the values of a time series. See also \ref ToStructSingleTimeSeriesData
	 * 
	 * \param timeSeries time series
	 * \param [out] values allocated array where values will be written to
	 * \param arrayLength allocated array size.
	 */
	DATATYPES_API void GetTimeSeriesValues(TIME_SERIES_PTR timeSeries, double * values, int arrayLength);

	/**
	 * \brief IGNORE - function unused but for old optional unit tests on reference counting and memory management.
	 */
	DATATYPES_API int GetNumTimeSeries();

	////////////////////////////////
	// Backward compatibility with SWIFTv1, but functions that may be generic to other persistent data sources
	////////////////////////////////

	/**
	 * \brief Get the temporal geometry of a time series, available by an identifier in a time series provider.
	 * Time series providers are a class of objects (data libraries, model simulations) that can provide time series for a given string identifier.
	 * 
	 * \param dataLibrary data librarytime series provider object, e.g. via an \ref ENSEMBLE_DATA_SET_PTR, or a model simulation in a modelling system such as SWIFT2.
	 * \param variableIdentifier variable identifier
	 * \param geom pointer to a regular_time_series_geometry struct, whose values will be written to
	 */
	DATATYPES_API void GetProviderTsGeometry(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, TS_GEOMETRY_PTR geom);
	/**
	 * \brief Get the values a time series, available by an identifier in a time series provider.
	 * Time series providers are a class of objects (data libraries, model simulations) that can provide time series for a given string identifier.
	 * 
	 * \param dataLibrary data librarytime series provider object, e.g. via an \ref ENSEMBLE_DATA_SET_PTR, or a model simulation in a modelling system such as SWIFT2.
	 * \param variableIdentifier variable identifier
	 * \param [out] values allocated array where values will be written to
	 * \param arrayLength allocated array size, gotten from \ref GetProviderTsGeometry
	 */
	DATATYPES_API void GetProviderTimeSeriesValues(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, double * values, int arrayLength);
	/**
	 * \brief Gets the known identifiers of a time series provider (data library items, or model simulation with recorded state variables)
	 * 
	 * \param dataLibrary data librarytime series provider object, e.g. via an \ref ENSEMBLE_DATA_SET_PTR, or a model simulation in a modelling system such as SWIFT2.
	 * \param [out] size number of data identifiers
	 * \return char** of size 'size' 
	 */
	DATATYPES_API char** GetProviderTimeSeriesIdentifiers(TIME_SERIES_PROVIDER_PTR dataLibrary, int* size);

	/**
	 * \brief Gets a univariate time series from a time series provider
	 * 
	 * \param dataLibrary data librarytime series provider object, e.g. via an \ref ENSEMBLE_DATA_SET_PTR, or a model simulation in a modelling system such as SWIFT2.
	 * \param variableIdentifier variable identifier
	 * \return reference counted wrapper to a new time series.
	 */
	DATATYPES_API TIME_SERIES_PTR TimeSeriesFromProviderTs(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier);


	////////////////////////////////
	// Obsolete and deprecated functions
	////////////////////////////////

#ifdef __cplusplus
}
#endif
