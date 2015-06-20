#pragma once

#include <stdexcept> 
#include <netcdf.h>
#include <map>
#include <boost/function.hpp>

#include "time_series.h"


namespace datatypes
{
	namespace timeseries
	{

		/** \brief	A class to hold the attributes of a netCDF variable stored in the SWIFT netCDF format*/
		class DATATYPES_DLL_LIB VariableAttributes
		{
			using string = std::string;

		public:
			string LongName;
			string Units;
			string Type;
			double FillValue;
			static const double DefaultFillValue() { return -9999.0; }
			VariableAttributes() { FillValue = DefaultFillValue(); }
		};

		namespace io
		{

			/** \brief	Class responsible for the low-level read/write operations from/to a SWIFT netCDF file */
			class DATATYPES_DLL_LIB SwiftNetCDFAccess
			{
				using string = std::string;

			private:
				void ThrowOnFileOpenFail(const string& filename, int code)
				{
					throw std::logic_error(string("Failed to open file ") + filename + " with code " + boost::lexical_cast<string>(code));
				}

				void ThrowOnVarInquiryFail(const string& varName, int code)
				{
					throw std::logic_error(string("Failed to query on variable ") + varName + " with code " + boost::lexical_cast<string>(code));
				}
			public:

				/**
				 * \brief	Constructor to wrap an existing SWIFT netCDF file
				 *
				 * \param	filename	SWIFT netCDF file.
				 */
				SwiftNetCDFAccess(const string& filename);

				/**
				 * \brief	Constructor to create a new SWIFT netCDF file
				 *
				 * \param	filename			 	name of the new file to create.
				 * \param	nEns				 	Size of the ensembles
				 * \param	nLead				 	Length of the lead time for each of the time series in the ensemble forecast for a given time.
				 * \param	timeUnits			 	Units of the temporal dimension(s).
				 * \param [in]	timeVar		 	The values of the "main" time dimension, consistent with the temporal units given with the previous parameter
				 * \param [in]	stationIds   	List of identifiers for the stations.
				 * \param [in]	varNames	 	List of names of the variables.
				 * \param [in]	varAttributes	Attributes for each variables; the keys of the dictionary must be found in the varNames parameter.
				 */
				SwiftNetCDFAccess(const string& filename, int nEns, int nLead, const string& timeUnits, std::vector<double>& timeVar, std::vector<string>& stationIds, std::vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);

				~SwiftNetCDFAccess();

				/**
				 * \brief	Gets ensemble size.
				 *
				 * \return	The ensemble size.
				 */
				int GetEnsembleSize();

				int GetEnsembleSize(const string& ncVarName);

				/**
				 * \brief	Gets the lengths of the lead time dimension.
				 *
				 * \return	The lead time length.
				 */
				int GetLeadTimeCount();

				int GetLeadTimeCount(const string& ncVarName);

				/**
				* \brief	Gets the lengths of the main time dimension.
				*
				* \return	The time length.
				*/
				int GetTimeLength();

				/**
				 * \brief	Gets the vector with the values of the main time dimension
				 *
				 * \return	the values of the main time dimension
				 */
				vector<ptime> * GetTimeDim();

				/**
				 * \brief	Gets the first time in the main time dimension of this netCDF data set.
				 *
				 * \return	The start date of the data set.
				 */
				ptime GetStart();

				/**
				 * \brief	Corresponding index for a string identifier for a variable in this data set.
				 *
				 * \param	identifier	The identifier.
				 *
				 * \return	An int.
				 */
				int IndexForIdentifier(const string& identifier);

				//int IndexForTime(const ptime* time);

				/**
				 * \brief	Time value for an index of the time dimension.
				 *
				 * \param	timeIndex	Zero-based index of the time.
				 *
				 * \return	A ptime.
				 */
				ptime TimeForIndex(int timeIndex);

				/**
				* \brief	Gets the values of an ensemble forecast for a variable, for a starting date in the main time dimension.
				*
				* \tparam	ElementType	type of element expected for this variable.
				* \param	varName		   	Name of the variable.
				* \param	catchmentNumber	The catchment number.
				* \param	timeIndex	   	Zero-based index of the time dimension.
				*
				* \return	null if it fails, else the forecasts.
				*/
				template<typename ElementType>
				std::vector<ElementType*> * GetForecasts(const string& varName, int catchmentNumber, int timeIndex)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[4];
					size_t countp[4];
					GetEnsFcastNetcdfWindow(catchmentNumber, timeIndex, startp, countp);
					auto vardata = GetForecastDataBuffer();

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_get_vara_double(ncid, dataVarId, startp, countp, vardata);

					auto result = new std::vector<ElementType*>();
					ElementType * dest;
					for (int i = 0; i < ensembleSize; i++)
					{
						dest = new ElementType[leadTimeLength];
						memcpy(dest, vardata + i * leadTimeLength, leadTimeLength * sizeof(ElementType));
						result->push_back(dest);
					}
					delete[] vardata;
					return result;
				}

				template<typename ElementType>
				std::vector<ElementType*> * GetEnsemble(const string& varName, int catchmentNumber)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[3];
					size_t countp[3];
					GetEnsNetcdfWindow(catchmentNumber, startp, countp);
					auto n = GetTimeLength();
					auto vardata = GetEnsembleDataBuffer(1, n);

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_get_vara_double(ncid, dataVarId, startp, countp, vardata);

					auto result = new std::vector<ElementType*>();
					ElementType * dest;
					auto maxT = this->GetTimeLength();
					for (int i = 0; i < ensembleSize; i++)
					{
						// double q_ens[station, ens_member, time]  in R; rev conventions for C
						// [time][ens_member][station]. There is only one station, so vardata is organised as
						// [time][ens_member]
						dest = new ElementType[n];
						for (size_t t = 0; t < maxT; t++)
						{
							dest[t] = vardata [ensembleSize * t + i];
						}
						result->push_back(dest);
					}
					delete[] vardata;
					return result;
				}

				/**
				* \brief	Gets the values of a variable stored as an non-ensemble series.
				*
				* \tparam	ElementType	type of element expected for this variable.
				* \param	varName		   	Name of the variable.
				* \param	catchmentNumber	The catchment number.
				*
				* \return	All the values for the time series defined with this variable name.
				*/
				template<typename ElementType>
				ElementType* GetValues(const string& varName, int catchmentNumber)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[3];
					size_t countp[3];
					GetNetcdfWindow(catchmentNumber, startp, countp);
					auto vardata = GetSingleSeriesDataBuffer(1, GetTimeLength());

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_get_vara_double(ncid, dataVarId, startp, countp, vardata);

					ElementType * dest = new ElementType[GetTimeLength()];
					memcpy(dest, vardata, GetTimeLength() * sizeof(ElementType));
					delete[] vardata;
					return dest;
				}
				
				/**
				* \brief	Sets the values for an ensemble forecast,  for a variable, for a starting date in the main time dimension.
				*
				* \tparam	ElementType	type of element expected for this variable.
				* \param	varName		   	Name of the variable.
				* \param	catchmentNumber	The catchment number.
				* \param	timeIndex	   	Zero-based index of the time.
				* \param [in]	values 	[in] the values to write to file.
				*/
				template<typename ElementType>
				void SetForecasts(const string& varName, int catchmentNumber, int timeIndex, std::vector<ElementType*> &values) 	// TODO: checks on 'values'
				{
					int dataVarId = GetVarId(varName);
					size_t startp[4];
					size_t countp[4];
					GetEnsFcastNetcdfWindow(catchmentNumber, timeIndex, startp, countp);
					auto vardata = GetForecastDataBuffer();

					ElementType * dest;
					for (int i = 0; i < ensembleSize; i++)
					{
						dest = vardata + i * leadTimeLength;
						memcpy(dest, values[i], leadTimeLength * sizeof(ElementType));
					}

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_put_vara_double(ncid, dataVarId, startp, countp, vardata);

					delete[] vardata;
				}

				template<typename ElementType>
				void SetEnsembles(const string& varName, int catchmentNumber, std::vector<ElementType*> &values)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[3];
					size_t countp[3];
					GetEnsNetcdfWindow(catchmentNumber, startp, countp);
					int n = GetTimeLength();
					auto vardata = GetEnsembleDataBuffer(1, n);
					
					for (int i = 0; i < ensembleSize; i++)
					{
						ElementType* ensData = values[i];
						for (int j = 0; j < n; j++)
						{
							vardata[ensembleSize * j + i] = ensData[j];
						}
					}

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_put_vara_double(ncid, dataVarId, startp, countp, vardata);

					delete vardata;
				}
				
				template<typename ElementType>
				void SetValues(const string& varName, int catchmentNumber, ElementType* values)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[2];
					size_t countp[2];
					GetNetcdfWindow(catchmentNumber, startp, countp);
					int n = GetTimeLength();
					auto vardata = GetSingleSeriesDataBuffer(1, n);

					memcpy(vardata, values, n * sizeof(ElementType));

					// KLUDGE
					// The use of nc_get_vara_double is clearly at odd with the declaration of GetForecasts as a template method 
					// This is a known limitation. This is likely something that can be overcome one way or another. Going with a generic approach for the API.
					int code = nc_put_vara_double(ncid, dataVarId, startp, countp, vardata);

					delete vardata;
				}

				void WriteSingleSeriesVarData();

				void WriteEnsembleVarData();

				void WriteForecastsVarData();

				static string CreateTimeUnitsAttribute(const ptime& utcStart, const string& units);
				static string CreateTimeUnitsAttribute(const ptime& utcStart, TimeStep& timeStep);
				static ptime ParseStartDate(const string& unitsAttribute);
				static string ParseTimeUnits(const string& unitsAttribute);

				TimeStep GetTimeStep();


			private:

				template<class TTo>
				static TTo* ConvertToArray(std::vector<string>& src);
				template<class TFrom, class TTo>
				static TTo* ConvertToArray(std::vector<TFrom>& src);

				const int kDefaultStrLength = 30;
				const string kTimeDimName = "time";
				const string kStationDimName = "station";
				const string kStrLenDimName = "strLen";
				const string kLeadTimeDimName = "lead_time";
				const string kEnsMemberDimName = "ens_member";

				const string kTimeVarName = "time";
				const string kLatVarName = "lat";
				const string kLonVarName = "lon";
				const string kElevationVarName = "elevation";
				const string kStationNameVarName = "station_name";
				const string kStationIdVarName = "station_id";
				const string kLeadTimeVarName = "lead_time";
				const string kEnsMemberVarName = "ens_member";

				const string kUnitsAttName = "units";
				const string kStandardNameAttName = "standard_name";
				const string kLongNameAttName = "long_name";
				const string kAxisAttName = "axis";
				const string kHistoryAttName = "history";
				const string kTypeAttName = "type";


				const string kCatchmentAttName = "Catchment";
				const string kFillValueAttName = "_FillValue";

				const double kDefaultFillValue = VariableAttributes::DefaultFillValue();

				void ReadGeometry();
				void WriteGeometry(int nEns, int nLead, const string& timeUnits, std::vector<double>& timeVar, std::vector<string>& stationIds, std::vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);
				void DefineMandatoryDimensions(int nEns, int nLead, int nStations);
				void DefineDimVariables();
				void DefineVariables(const int varNumDims, int* varDimIDs);
				void WriteCommonVarData();
				int AddGlobalAttribute(const string& attName, const string& attValue);
				int AddGlobalAttribute(char * attName, char * attValue);
				int AddAttribute(int varId, const string& attName, const string& attValue);
				void AddVariableAttributes(int varId, const string& longName, const string& units, const string& type, double fillValue);
				void AddAttributes(int varId, string& varName, std::map<string, VariableAttributes>& varAttributes);
				void FillAttributes(const string& timeUnits);
				void SetTimeVar(std::vector<double>& timeVar);
				void InquireDimIds();
				void InquireDimVarIds();
				int InquireDimLength(int dimId);
				int GetNumDims(const string& ncVarName);
				void ReadTimeUnits();
				std::vector<string> * GetStringVariable(int strLen, int varId, int n);
				string GetStringAttribute(const string& attName);
				bool HasAttribute(const string& attName);
				nc_type GetDataType(int variableId);
				int * GetIntVariable(int varId, int size);

				template<class T>
				T* GetVariable(int varId, int n);

				template<class T, class TStored>
				T* GetVariable(int varId, int n);

				/**
				 * \brief	Gets the NetCDF start/count specifications for an ensemble forecast time series.
				 *
				 * \param	catchmentNumber	The catchment number.
				 * \param	timeIndex	   	Zero-based index of the time.
				 * \param [in,out]	startp 	the dimensions start indices.
				 * \param [in,out]	countp 	the dimensions counts.
				 */
				void GetEnsFcastNetcdfWindow(int catchmentNumber, int timeIndex, size_t *startp, size_t *countp);

				/**
				* \brief	Gets the NetCDF start/count specifications for the data pertaining to an ensemble of time series.
				*
				* \param	catchmentNumber	The catchment number.
				* \param [in,out]	startp 	the dimensions start indices.
				* \param [in,out]	countp 	the dimensions counts.
				*/
				void GetEnsNetcdfWindow(int catchmentNumber, size_t *startp, size_t *countp);

				/**
				* \brief	Gets the NetCDF start/count specifications for the data pertaining to a time index.
				*
				* \param	catchmentNumber	The catchment number.
				* \param [in,out]	startp 	the dimensions start indices.
				* \param [in,out]	countp 	the dimensions counts.
				*/
				void GetNetcdfWindow(int catchmentNumber, size_t *startp, size_t *countp);
				int GetVarId(const string& varName);
				double * GetForecastDataBuffer(int numStations = 1, int numTimeSteps = 1);
				double * GetEnsembleDataBuffer(int numStations, int numTimeSteps);
				double * GetSingleSeriesDataBuffer(int numStations, int numTimeSteps);
				int ncid = -1;
				int timeDimId, stationDimId, leadTimeDimId, ensMemberDimId, strLenDimId;
				int timeVarId = -1, stationNameVarId = -1, stationIdVarId = -1, leadTimeVarId = -1, ensMemberVarId = -1, latVarId = -1, lonVarId = -1, elevationVarId = -1;

				ptime startDate;
				string timeUnits;
				int numTimeSteps, numStations, strLen;

				std::vector<string> * stationNames = nullptr;
				std::vector<string> * variableVarNames = nullptr;

				// TODO suggest this is strings
				int * stationIds = nullptr,
					*leadTimeVec = nullptr;
				double * stationLat = nullptr;
				double * stationLon = nullptr;
				double * stationElevation = nullptr;
				int * timeVec = nullptr;
				int * variableVarIds = nullptr;
				int leadTimeLength;
				//int stepMultiplier ;
				int ensembleSize;
				string catchmentName;

				std::vector<string> variableNames;
				std::map<string, VariableAttributes> variableAttributes;
			};
		}

		using namespace datatypes::timeseries::io;

		/**
		 * \brief	Representation of an univariate, ensemble time series with a SWIFT netCDF back end.
		 *
		 * \tparam	T	The type of the elements in the series; typically double or float.
		 */
		template <typename T>
		class DATATYPES_DLL_LIB SwiftNetCDFTimeSeries
		{
			using string = std::string;
		public:

			/**
			 * \brief	Constructor.
			 *
			 * \param [in]          	dataAccess	SWIFT netCDF data access object to read/write the back end file.
			 * \param	varName			  	Name of the variable for this time series (the netCDF back end may have several variables).
			 * \param	identifier		  	The identifier.
			 */
			SwiftNetCDFTimeSeries(SwiftNetCDFAccess * dataAccess, const string& varName, const string& identifier);

			/**
			 * \brief	Gets the ensemble forecast for a given index in the time dimension
			 *
			 * \param	i	Zero-based index of the time step of interest.
			 *
			 * \return	a pointer to a new MultiTimeSeries.
			 */
			MultiTimeSeries<TimeSeries*> * GetForecasts(int i);

			/**
			 * \brief	Gets a non-ensemble time series. There must be such a record, otherwise an exception is thrown.
			 *
			 * \return	null if it fails, else the series.
			 */
			TTimeSeries<T> * GetSeries();

			MultiTimeSeries<TimeSeries*> * GetEnsembleSeries();

			void SetForecasts(int i, MultiTimeSeries<TimeSeries*> * forecasts);
			void SetEnsemble(MultiTimeSeries<TimeSeries*> * ensemble);
			void SetSeries(TTimeSeries<T> * timeSeries);
			int GetEnsembleSize();
			int GetLeadTimeCount();
			int GetTimeLength();
			TimeStep GetTimeStep();
			ptime TimeForIndex(int timeIndex);

		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
			string varName;
			string identifier;
			int catchmentNumber;
		};


		template <typename T>
		class DATATYPES_DLL_LIB SwiftNetCDFTimeSeriesStore
		{
			using string = std::string;
		public:

			/**
			 * \brief	Create a wrapper time series store around an existing SWIFT netCDF file.
			 *
			 * \param	filename	Filename of the file.
			 */
			SwiftNetCDFTimeSeriesStore(const string& filename);

			/**
             *\brief	Constructor to create a new SWIFT netCDF file
             *
             * \param	filename			 	name of the new file to create.
             * \param	nEns				 	Size of the ensembles
             * \param	nLead				 	Length of the lead time for each of the time series in the ensemble forecast for a given time.
             * \param	timeUnits			 	Units of the temporal dimension(s).
             * \param[in]	timeVar		 	The values of the "main" time dimension, consistent with the temporal units given with the previous parameter
             * \param[in]	stationIds   	List of identifiers for the stations.
             * \param[in]	varNames	 	List of names of the variables.
             * \param[in]	varAttributes	Attributes for each variables; the keys of the dictionary must be found in the varNames parameter.
			 */
			SwiftNetCDFTimeSeriesStore(const string& filename, int nEns, int nLead, const string& timeUnits, std::vector<double>& timeVar, std::vector<string>& stationIds, std::vector<string>& varNames,
				std::map<string, VariableAttributes>& varAttributes = std::map<string, VariableAttributes>());
			
			~SwiftNetCDFTimeSeriesStore();

			void Close();


			/**
			 * \brief	Create an univariate SWIFT netCDF time series using this netCDF time series store.
			 *
			 * \param	varName		 	Name of the variable.
			 * \param	identifier   	The identifier; e.g. a catchment identifier.
			 * \param	dimIdent	 	(Optional) name of the dimension in which to look for the location identifier (e.g. catchment identifier). Defaults to 'station_id' as per SWIFT netCDF schema
			 * \param	startTime	 	(Optional) the start time. If null (default), the start time of the whole data set is used.
			 * \param	leadTimeCount	(Optional) number of lead times to read. If negative (default), the maximum number of lead times returned by GetLeadTimeCount()
			 *
			 * \return	null if it fails, else a SwiftNetCDFTimeSeries&lt;T&gt;*.
			 */
			SwiftNetCDFTimeSeries<T> * Get(const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1);
			//void Set(SwiftNetCDFTimeSeries<T> * series, const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1);

			int GetEnsembleSize();
			int GetLeadTimeCount();
			vector<ptime> * GetTimeDim();
			int GetTimeLength();
			ptime GetStart();
			int IndexForIdentifier(const string& identifier);
			//int IndexForTime(const ptime* time);

		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
		};


		template <typename T>
		class DATATYPES_DLL_LIB EnsembleTimeSeriesStore
		{
		public:
			virtual ~EnsembleTimeSeriesStore() {};
			virtual MultiTimeSeries<TimeSeries*>* Read(const std::string& ensembleIdentifier) = 0;
		};


		template <typename T>
		class DATATYPES_DLL_LIB MultiFileEnsembleTimeSeriesStore : public EnsembleTimeSeriesStore < T >
		{
			using string = std::string;
		public:
			MultiFileEnsembleTimeSeriesStore(const string& forecastDataFiles, const string& varName, const string& varIdentifier, int index);
			MultiTimeSeries<TimeSeries*>* Read(const string& fileIdentifier) override;
			virtual ~MultiFileEnsembleTimeSeriesStore() {/*TODO*/ };
		private:
			string forecastDataFiles;
			string varName;
			string varIdentifier;
			int index;
		};


		template <typename T>
		class SingleSeriesInformation
		{
			using string = std::string;
		public:
			SingleSeriesInformation(SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& identifier) :
				dataAccess(dataAccess), ncVarName(ncVarName), identifier(identifier) 
			{
			}
			SwiftNetCDFTimeSeriesStore<T> * dataAccess;
			string ncVarName;
			string identifier;
			SwiftNetCDFTimeSeries<T> * GetSwiftNetCDFTimeSeries()
			{
				return dataAccess->Get(ncVarName, identifier);
			}

		};

		class DATATYPES_DLL_LIB TimeSeriesLibraryDescription
		{
			using string = std::string;
		public:
			void AddSingle(const string& dataId, const string& fileName, const string& ncVarName, const string& identifier);
			void AddEnsembleForecast(const string& dataId, const string& fileName, const string& ncVarName, const string& identifier, int index);

			std::vector<string> GetDataIdSingle() const;
			std::vector<string> GetDataIdEnsemble() const;

			string GetFilename(const string& dataId) const;
			string GetFileVariableName(const string& dataId) const;
			string GetIdentifier(const string& dataId) const;
			int GetIndex(const string& dataId) const;
		private:
			class SourceInfo
			{
			public:
				SourceInfo(){}
				SourceInfo(const string& dataId, const string& fileName, const string& ncVarName, const string& identifier, int index = -1)
				{
					this->dataId = dataId;
					this->fileName = fileName;
					this->ncVarName = ncVarName;
					this->identifier = identifier;
					this->index = index;
				}
				string dataId;
				string fileName;
				string ncVarName;
				string identifier;
				int index;
			};
			std::map < std::string, SourceInfo > ensProviders;
			std::map < std::string, SourceInfo > singleProviders;
			SourceInfo GetInfo(const string& dataId) const;
		};



		/**
		 * \brief	Library of time series, for high level access to sources of time series that nmay have varying on-disk representations
		 *
		 * \tparam	T	The element type of the time series dealt with, typically double or float.
		 */
		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesLibrary
		{
			using string = std::string;
		public:
			TimeSeriesLibrary() {}
			TimeSeriesLibrary(const TimeSeriesLibraryDescription& description);
			TimeSeriesLibrary<T>& operator=(TimeSeriesLibrary<T>&& src);
			/**
			* \brief	Constructor using the move semantics
			*
			* \param [in,out]	src	time series library from which to move data.
			* \remarks 		C++ for the Impatient
			* 					Appendix A. A Painless Introduction to Rvalue References (C++11)
			* 					See also http://stackoverflow.com/a/3109981/2752565 for information on move semantic
			*/
			TimeSeriesLibrary(TimeSeriesLibrary<T>&& src); // Move constructor


			// Maybe the following,m but may introduce too much coupling with on disk representation with SwiftNetCDFAccess.
			// TTimeSeries<T>* GetSingle(const string& dataId, boost::function<TTimeSeries<T>*(SwiftNetCDFAccess * dataAccess)> tsTransform);

			/**
			 * \brief	Gets a single time series out of the library
			 *
			 * \param	dataId			   	Identifier for the time series
			 * \param [in]	tsTransform	    If non-null, a time series transformation to perform on the raw data retrieved for dataId.
			 *
			 * \return	The univariate, single realization time series 
			 */
			TTimeSeries<T>* GetSingle(const string& dataId, boost::function<TTimeSeries<T>*(TTimeSeries<T>*)>& tsTransform);

			/**
			* \brief	Gets a single time series out of the library
			*
			* \param	dataId			   	Identifier for the time series
			*
			* \return	The univariate, single realization time series
			*/
			TTimeSeries<T>* GetSingle(const string& dataId);

			MultiTimeSeries<TTimeSeries<T>*>* GetEnsemble(const string& dataId, const string& dataItemIdentifier);

			MultiTimeSeries<TTimeSeries<T>*>* GetEnsembleTimeSeries(const string& dataId);
			EnsembleForecastTimeSeries<TTimeSeries<T>>* GetEnsembleForecasts(const string& dataId);

			void AddSwiftNetCDFSource(const string& dataId, const string& fileName, const string& ncVarName, const string& identifier);
			void AddSource(const string& dataId, SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& identifier);
			void AddSource(const string& dataId, SwiftNetCDFTimeSeries<T> * timeSeries);
			void AddSource(const string& dataId, EnsembleTimeSeriesStore<T> * dataAccess);

		private:
			std::map < string, EnsembleTimeSeriesStore<T>* > ensTimeSeriesProviders;
			std::map < string, SingleSeriesInformation<T>* > timeSeriesProviders;
			SwiftNetCDFTimeSeries<T> * GetSwiftNetCDFTimeSeries(const string& dataId);
			SingleSeriesInformation<T> * GetSeriesInformation(const string& dataId);

		};

		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesIOHelper
		{
		public:
			using Tts = TTimeSeries<T>;
			static Tts* Read(const std::string& netCdfFilePath, const std::string& varName, const std::string& identifier);
			static MultiTimeSeries<TimeSeries*>* ReadForecastRainfallTimeSeries(const std::string& netCdfFilepath, const std::string& varName, const std::string& identifier, int index);
			static Tts* Read(const std::string& netCdfFilePath, const std::string& varName, const std::string& identifier, const TimeWindow<Tts>& window);
			static Tts* ReadDailyToHourly(const std::string& netCdfFilePath, const std::string& varName, const std::string& identifier, const TimeWindow<Tts>& window);
			static TimeSeriesLibrary<T>* CreateTimeSeriesLibrary(const TimeSeriesLibraryDescription& description);
		};

	}
}