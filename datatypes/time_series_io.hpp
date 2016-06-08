#pragma once

#include <stdexcept> 
#include <netcdf.h>
#include <map>
#ifdef __GNUC__
// https ://jira.csiro.au/browse/WIRADA-350  GNU gcc regex bug; use boost instead
#if (__GNUC__ <= 4 && __GNUC_MINOR__ < 9)
#include <boost/regex.hpp>
#else
#include <regex>
#endif
#else
#include <regex>
#endif // __GNUC__
#include <algorithm>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>    

#include "time_series.hpp"
#include "time_series_store.hpp"


namespace datatypes
{
	namespace timeseries
	{
		using namespace datatypes::exceptions;


		/** \brief	A class to hold the attributes of a netCDF variable stored in the SWIFT netCDF format*/
		class DATATYPES_DLL_LIB VariableAttributes
		{
		public:
			string LongName;
			string Units;
			string Type;
			double FillValue;
			static const double DefaultFillValue() { return DEFAULT_MISSING_DATA_VALUE; }
			VariableAttributes() { FillValue = DefaultFillValue(); }
		};

		namespace io
		{

			template<typename ElementType>
			class DATATYPES_DLL_LIB SwiftNetCDFVariablePersister
			{
			public:
				static int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, ElementType *op)
				{
					throw std::logic_error(string("No template specialization found for SwiftNetCDFVariablePersister::NcGetVara for type") + typeid(ElementType).name());
				}
				static int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const ElementType *op)
				{
					throw std::logic_error(string("No template specialization found for SwiftNetCDFVariablePersister::NcPutVara for type") + typeid(ElementType).name());
				}
			};

			template<>
			class DATATYPES_DLL_LIB SwiftNetCDFVariablePersister<double>
			{
			public:
				static int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, double *op)
				{
					return nc_get_vara_double(ncid, varid, startp, countp, op);
				}
				static int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const double *op)
				{
					return nc_put_vara_double(ncid, varid, startp, countp, op);
				}
			};


			/** \brief	Class responsible for the low-level read/write operations from/to a SWIFT netCDF file */
			class DATATYPES_DLL_LIB SwiftNetCDFAccess
			{
				//

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
				SwiftNetCDFAccess(const string& filename, int nEns, int nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds,
					vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);

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
				vector<ptime> GetTimeDim();

				/**
				* \brief	Gets the first time in the main time dimension of this netCDF data set.
				*
				* \return	The start date of the data set.
				*/
				ptime GetStart();

				/**
				* \brief	Gets the last time in the main time dimension of this netCDF data set.
				*
				* \return	The end date of the data set.
				*/
				ptime GetEnd();

				/**
				 * \brief	Corresponding index for a string identifier for a variable in this data set.
				 *
				 * \param	identifier	The identifier.
				 *
				 * \return	An int.
				 */
				int IndexForIdentifier(const string& identifier) const;

				vector<string> GetIdentifiers() const;

				//int IndexForTime(const ptime* time);

				/**
				 * \brief	Time value for an index of the time dimension.
				 *
				 * \param	timeIndex	Zero-based index of the time.
				 *
				 * \return	A ptime.
				 */
				ptime TimeForIndex(int timeIndex);

			private:
				template<typename ElementType>
				int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, ElementType *op)
				{
					return SwiftNetCDFVariablePersister<ElementType>::NcGetVara(ncid, varid, startp, countp, op);
				}

				template<typename ElementType>
				int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const ElementType *op)
				{
					return SwiftNetCDFVariablePersister<ElementType>::NcPutVara(ncid, varid, startp, countp, op);
				}

			public:

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
				vector<ElementType*> * GetForecasts(const string& varName, int catchmentNumber, int timeIndex)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[4];
					size_t countp[4];
					GetEnsFcastNetcdfWindow(catchmentNumber, timeIndex, startp, countp);
					auto vardata = GetForecastDataBuffer();

					int code = NcGetVara(ncid, dataVarId, startp, countp, vardata);

					auto result = new vector<ElementType*>();
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
				vector<ElementType*> * GetEnsemble(const string& varName, int catchmentNumber)
				{
					int dataVarId = GetVarId(varName);
					size_t startp[3];
					size_t countp[3];
					GetEnsNetcdfWindow(catchmentNumber, startp, countp);
					auto n = GetTimeLength();
					auto vardata = GetEnsembleDataBuffer(1, n);

					int code = NcGetVara(ncid, dataVarId, startp, countp, vardata);

					auto result = new vector<ElementType*>();
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
							dest[t] = vardata[ensembleSize * t + i];
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

					int code = NcGetVara(ncid, dataVarId, startp, countp, vardata);

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
				void SetForecasts(const string& varName, int catchmentNumber, int timeIndex, vector<ElementType*> &values) 	// TODO: checks on 'values'
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

					int code = NcPutVara(ncid, dataVarId, startp, countp, vardata);

					delete[] vardata;
				}

				template<typename ElementType>
				void SetEnsembles(const string& varName, int catchmentNumber, vector<ElementType*> &values)
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

					int code = NcPutVara(ncid, dataVarId, startp, countp, vardata);

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

					int code = NcPutVara(ncid, dataVarId, startp, countp, vardata);

					delete vardata;
				}

				void WriteSingleSeriesVarData();

				void WriteEnsembleVarData();

				void WriteForecastsVarData();

				vector<string> ReadVariableNames();
				vector<string> ReadAttributeNames(const string& varName);
				string ReadStringAttribute(const string& varName, const string& attName);
				double ReadNumericAttribute(const string& varName, const string& attName);

				static string CreateTimeUnitsAttribute(const ptime& utcStart, const string& units);
				static string CreateTimeUnitsAttribute(const ptime& utcStart, TimeStep& timeStep);
				static ptime ParseStartDate(const string& unitsAttribute);
				static string ParseTimeUnits(const string& unitsAttribute);

				TimeStep GetTimeStep();

				template<class TFrom, class TTo>
				static vector<TTo> Convert(const vector<TFrom>& from, const std::function<TTo(const TFrom&)>& f)
				{
					vector<TTo> result(from.size());
					std::transform<vector<TFrom>::const_iterator, vector<TTo>::iterator, std::function<TTo(TFrom)>>
						(from.begin(), from.end(), result.begin(), f);
					return result;
				}

			private:

				template<class TTo>
				static TTo* ConvertToArray(vector<string>& src)
				{
					using boost::lexical_cast;
					TTo* result = new TTo[src.size()];
					for (int i = 0; i < src.size(); i++)
					{
						result[i] = lexical_cast<TTo>(src[i]);
					}
					return result;
				}

				template<class TFrom, class TTo>
				static TTo* ConvertToArray(vector<TFrom>& src)
				{
					TTo* result = new TTo[src.size()];
					for (int i = 0; i < src.size(); i++)
					{
						result[i] = (TTo)src[i];
					}
					return result;
				}

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
				void WriteGeometry(int nEns, int nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds, vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);
				void DefineMandatoryDimensions(int nEns, int nLead, int nStations);
				void DefineDimVariables();
				void DefineVariables(const int varNumDims, int* varDimIDs);
				void WriteCommonVarData();
				int AddGlobalAttribute(const string& attName, const string& attValue);
				int AddGlobalAttribute(char * attName, char * attValue);
				int AddAttribute(int varId, const string& attName, const string& attValue);
				void AddVariableAttributes(int varId, const string& longName, const string& units, const string& type, double fillValue);
				void AddAttributes(int varId, const string& varName, std::map<string, VariableAttributes>& varAttributes);
				void FillAttributes(const string& timeUnits);
				void SetTimeVar(vector<double>& timeVar);
				void InquireDimIds();
				void InquireDimVarIds();
				int InquireDimLength(int dimId);
				int GetNumDims(const string& ncVarName);
				void ReadTimeUnits();
				vector<string> * GetStringVariable(int strLen, int varId, int n);
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

				vector<string> * stationNames = nullptr;
				vector<string> * variableVarNames = nullptr;

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

				vector<string> variableNames;
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
			//
		public:

			/**
			 * \brief	Constructor.
			 *
			 * \param [in]          	dataAccess	SWIFT netCDF data access object to read/write the back end file.
			 * \param	varName			  	Name of the variable for this time series (the netCDF back end may have several variables).
			 * \param	identifier		  	The identifier.
			 */
			SwiftNetCDFTimeSeries(SwiftNetCDFAccess * dataAccess, const string& varName, const string& identifier)
			{
				this->dataAccess = dataAccess;
				this->varName = varName;
				this->identifier = identifier;
				this->catchmentNumber = dataAccess->IndexForIdentifier(identifier);
			}

			/**
			 * \brief	Gets the ensemble forecast for a given index in the time dimension
			 *
			 * \param	i	Zero-based index of the time step of interest.
			 *
			 * \return	a pointer to a new MultiTimeSeries.
			 */
			MultiTimeSeries<TimeSeries*> * GetForecasts(int i)

			{
				auto series = dataAccess->GetForecasts<T>(varName, catchmentNumber, i);
				auto result = new MultiTimeSeries<TimeSeries*>(*series, this->GetLeadTimeCount(), this->dataAccess->TimeForIndex(i), this->GetTimeStep());
				for (auto& d : (*series))
				{
					if (d != nullptr) delete[] d;
				}
				delete series;
				return result;
			}

			/**
			 * \brief	Gets a non-ensemble time series. There must be such a record, otherwise an exception is thrown.
			 *
			 * \return	null if it fails, else the series.
			 */
			TTimeSeries<T> * GetSeries()
			{
				auto values = dataAccess->GetValues<T>(varName, catchmentNumber);
				auto result = new TTimeSeries<T>(values, this->GetTimeLength(), this->TimeForIndex(0), this->GetTimeStep());
				delete values;
				return result;
			}

			MultiTimeSeries<TimeSeries*> * GetEnsembleSeries()
			{
				auto series = dataAccess->GetEnsemble<T>(varName, catchmentNumber);
				auto result = new MultiTimeSeries<TimeSeries*>(*series, this->GetTimeLength(), this->TimeForIndex(0), this->GetTimeStep());
				for (auto& d : (*series))
				{
					if (d != nullptr) delete[] d;
				}
				delete series;
				return result;
			}

			void SetForecasts(int i, MultiTimeSeries<TimeSeries*> * forecasts)
			{
				dataAccess->WriteForecastsVarData();
				auto values = forecasts->GetValues();
				dataAccess->SetForecasts(varName, catchmentNumber, i, (*values));
				for (auto& d : (*values))
				{
					if (d != nullptr) delete[] d;
				}
				delete values;
			}

			void SetEnsemble(MultiTimeSeries<TimeSeries*> * ensemble)
			{
				dataAccess->WriteEnsembleVarData();
				vector<T*>* values = ensemble->GetValues();
				dataAccess->SetEnsembles(varName, catchmentNumber, *values);
				for (T* data : *values)
					if (data != nullptr) delete[] data;
				delete values;
			}

			void SetSeries(TTimeSeries<T> * timeSeries)
			{
				dataAccess->WriteSingleSeriesVarData();
				T* values = timeSeries->GetValues();
				dataAccess->SetValues(varName, catchmentNumber, values);
				delete[] values;
			}

			int GetEnsembleSize()
			{
				return dataAccess->GetEnsembleSize(varName);
			}

			int GetLeadTimeCount()
			{
				return dataAccess->GetLeadTimeCount(varName);
			}

			int GetTimeLength()
			{
				return dataAccess->GetTimeLength();
			}

			TimeStep GetTimeStep()
			{
				return dataAccess->GetTimeStep();
			}

			ptime TimeForIndex(int timeIndex)
			{
				return dataAccess->TimeForIndex(timeIndex);
			}

		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
			string varName;
			string identifier;
			int catchmentNumber;
		};

		template <typename T = double>
		class DATATYPES_DLL_LIB SwiftNetCDFTimeSeriesStore
		{
			//
		public:

			/**
			 * \brief	Create a wrapper time series store around an existing SWIFT netCDF file.
			 *
			 * \param	filename	Filename of the file.
			 */
			SwiftNetCDFTimeSeriesStore(const string& filename)
			{
				dataAccess = new SwiftNetCDFAccess(filename);
			}

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
			SwiftNetCDFTimeSeriesStore(const string& filename, int nEns, int nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds, vector<string>& varNames,
				std::map<string, VariableAttributes>& varAttributes = std::map<string, VariableAttributes>())
			{
				dataAccess = new SwiftNetCDFAccess(filename, nEns, nLead, timeUnits, timeVar, stationIds, varNames, varAttributes);
			}

			~SwiftNetCDFTimeSeriesStore()
			{
				Close();
			}

			void Close()
			{
				if (dataAccess != nullptr)
				{
					delete dataAccess;
					dataAccess = nullptr;
				}
			}


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
			SwiftNetCDFTimeSeries<T> * Get(const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1)
			{
				return new SwiftNetCDFTimeSeries<T>(dataAccess, varName, identifier);
			}

			//void Set(SwiftNetCDFTimeSeries<T> * series, const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1);

			int GetEnsembleSize()
			{
				return dataAccess->GetEnsembleSize();
			}

			int GetLeadTimeCount()
			{
				return dataAccess->GetLeadTimeCount();
			}

			vector<ptime> GetTimeDim()
			{
				return dataAccess->GetTimeDim();
			}

			int GetTimeLength()
			{
				return dataAccess->GetTimeLength();
			}

			TimeStep GetTimeStep()
			{
				return dataAccess->GetTimeStep();
			}

			ptime GetStart()
			{
				return dataAccess->GetStart();
			}

			ptime GetEnd()
			{
				return dataAccess->GetEnd();
			}

			int IndexForIdentifier(const string& identifier)
			{
				return dataAccess->IndexForIdentifier(identifier);
			}

			vector<string> GetIdentifiers() const
			{
				return dataAccess->GetIdentifiers();
			}


		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
		};

		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesIOHelper
		{
		public:
			using Tts = TTimeSeries<T>;
			static Tts* Read(const string& netCdfFilePath, const string& varName, const string& identifier)
			{
				SwiftNetCDFTimeSeriesStore<T> store(netCdfFilePath);

				SwiftNetCDFTimeSeries<T>* netCdfTimeSeries = store.Get(varName, identifier);
				auto result = netCdfTimeSeries->GetSeries();
				delete netCdfTimeSeries;
				return result;
			}

			static MultiTimeSeries<TimeSeries*>* ReadForecastRainfallTimeSeries(const string& netCdfFilepath, const string& varName, const string& identifier, int index)
			{
				SwiftNetCDFTimeSeriesStore<T> rainfallStore(netCdfFilepath);
				SwiftNetCDFTimeSeries<T>* forecastRainEnsemble = rainfallStore.Get(varName, identifier);
				MultiTimeSeries<TimeSeries*>* forecastRainMultiTimeSeries = forecastRainEnsemble->GetForecasts(index);
				delete forecastRainEnsemble;
				return forecastRainMultiTimeSeries;
			}

			static Tts* Read(const string& netCdfFilePath, const string& varName, const string& identifier, const TimeWindow<Tts>& window)
			{
				auto tmp = Read(netCdfFilePath, varName, identifier);
				auto result = window.Trim(*tmp);
				delete tmp;
				return result;
			}

			static Tts* ReadDailyToHourly(const string& netCdfFilePath, const string& varName, const string& identifier, const TimeWindow<Tts>& window)
			{
				TTimeSeries<T>* fullDailyObsPetTimeSeries = Read(netCdfFilePath, varName, identifier);
				TTimeSeries<T>* fullHourlyObsPetTimeSeries = TimeSeriesOperations<Tts>::DailyToHourly(*fullDailyObsPetTimeSeries);
				auto result = window.Trim(*fullHourlyObsPetTimeSeries);
				delete fullDailyObsPetTimeSeries;
				delete fullHourlyObsPetTimeSeries;
				return result;
			}
		};

		template <typename T>
		class DATATYPES_DLL_LIB SingleNetCdfFileStore
		{
			//
		protected:
			SingleNetCdfFileStore(SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& identifier) :
				dataAccess(dataAccess), ncVarName(ncVarName), identifier(identifier)
			{
			}
			SwiftNetCDFTimeSeries<T> * GetSwiftNetCDFTimeSeries()
			{
				return dataAccess->Get(ncVarName, identifier);
			}

			vector<string> GetIdentifiers() const
			{
				return dataAccess->GetIdentifiers();
			}

			SwiftNetCDFTimeSeriesStore<T> * dataAccess = nullptr;
			string ncVarName;
			string identifier;

		public:
			virtual ~SingleNetCdfFileStore()
			{
				if (dataAccess != nullptr)
				{
					delete dataAccess;
					dataAccess = nullptr;
				}
			}
		};


		template <typename T>
		class DATATYPES_DLL_LIB NetCdfSingleSeriesStore :
			public SingleTimeSeriesStore<T>,
			public SingleNetCdfFileStore<T>
		{
			//
		public:
			NetCdfSingleSeriesStore(SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(dataAccess, ncVarName, identifier)
			{
			}

			SwiftNetCDFTimeSeries<T> * GetSwiftNetCDFTimeSeries()
			{
				if (this->identifier == string(""))
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Cannot get a time series if no default identifier is provided.");
				return this->dataAccess->Get(this->ncVarName, this->identifier);
			}

			string GetDataSummary() const
			{
				auto start = this->dataAccess->GetStart();
				auto end = this->dataAccess->GetEnd();
				string result =
					string("variable name: ") + this->ncVarName +
					string(", identifier: ") + this->identifier +
					string(", start: ") + to_iso_extended_string(start) +
					string(", end: ") + to_iso_extended_string(end) +
					string(", time length: ") + boost::lexical_cast<string>(this->dataAccess->GetTimeLength()) +
					string(", time step: ") + this->dataAccess->GetTimeStep().GetName();
				return result;
			}

			TTimeSeries<T>* Read()
			{
				return GetSwiftNetCDFTimeSeries()->GetSeries();
			}

			TTimeSeries<T>* Read(const string& collectionIdentifier)
			{
				return this->dataAccess->Get(this->ncVarName, collectionIdentifier)->GetSeries();
			}

			vector<string> GetIdentifiers() const
			{
				return dataAccess->GetIdentifiers();
			}
		};

		template <typename T>
		class DATATYPES_DLL_LIB NetCdfEnsembleTimeSeriesStore : public EnsembleTimeSeriesStore < T >, public SingleNetCdfFileStore<T>
		{
			//
		public:
			NetCdfEnsembleTimeSeriesStore(SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& identifier) :
				SingleNetCdfFileStore<T>(dataAccess, ncVarName, identifier)
			{
			}
			string GetDataSummary() const
			{
				auto start = this->dataAccess->GetStart();
				auto end = this->dataAccess->GetEnd();
				string result =
					string("variable name: ") + this->ncVarName +
					string(", identifier: ") + this->identifier +
					string(", start: ") + to_iso_extended_string(start) +
					string(", end: ") + to_iso_extended_string(end) +
					string(", time length: ") + boost::lexical_cast<string>(this->dataAccess->GetTimeLength()) +
					string(", time step: ") + this->dataAccess->GetTimeStep().GetName();
				return result;
			}

			MultiTimeSeries<TTimeSeries<T>*>* Read()
			{
				return this->dataAccess->Get(this->ncVarName, this->identifier)->GetEnsembleSeries();
			}
		};

		template <typename T>
		class DATATYPES_DLL_LIB NetCdfTimeSeriesEnsembleTimeSeriesStore :
			public TimeSeriesEnsembleTimeSeriesStore < T >,
			public SingleNetCdfFileStore<T>
		{
			//
		public:
			NetCdfTimeSeriesEnsembleTimeSeriesStore(SwiftNetCDFTimeSeriesStore<T> * dataAccess, const string& ncVarName, const string& varIdentifier) :
				SingleNetCdfFileStore<T>(dataAccess, ncVarName, varIdentifier)
			{
				netCdfTseries = dataAccess->Get(ncVarName, varIdentifier, "station_id", nullptr, /*int leadTimeCount = */-1);
			}

			virtual ~NetCdfTimeSeriesEnsembleTimeSeriesStore()
			{
				if (netCdfTseries != nullptr)
				{
					delete netCdfTseries;
					netCdfTseries = nullptr;
				}
				if (indexForTime != nullptr)
				{
					delete indexForTime;
					indexForTime = nullptr;
				}
			};

			MultiTimeSeries<TimeSeries*>* Read(const string& ensembleIdentifier) override
			{
				char * end;
				const char* q = ensembleIdentifier.c_str();
				int index = -1;
				ptime dateIndex = from_iso_string(ensembleIdentifier);
				if (dateIndex == not_a_date_time)
				{
					index = (int)std::strtol(q, &end, 10);
					const char* r = q + ensembleIdentifier.size() * sizeof(char*);
					if (end == q || end != r)
					{
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Could not parse as integer or a (ptime) date specification: " + ensembleIdentifier);
					}
				}
				else
				{
					index = GetIndexForTime(dateIndex);
				}

				// check on index validity
				// datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(
				MultiTimeSeries<TimeSeries*>* forecastRainMultiTimeSeries = netCdfTseries->GetForecasts(index);
				return forecastRainMultiTimeSeries;
			}

			int GetIndexForTime(const ptime& dateIndex)
			{
				if (indexForTime == nullptr)
				{
					vector<ptime> p = this->dataAccess->GetTimeDim();
					indexForTime = new std::map<ptime, int>();
					for (size_t i = 0; i < p.size(); i++)
					{
						(*indexForTime)[p[i]] = i;
					}
				}
				return indexForTime->at(dateIndex);
			}

			vector<string> GetItemIdentifiers() const
			{
				vector<ptime> times = this->dataAccess->GetTimeDim();
				std::function<string(const ptime&)> f = [&](const ptime& p) { return boost::posix_time::to_iso_string(p); };
				return SwiftNetCDFAccess::Convert<ptime, string>(times, f);
			}

			size_t GetLength() const
			{
				return this->dataAccess->GetTimeLength();
			}

			TimeStep GetTimeStep() const
			{
				// TODO
				return TimeStep::GetDaily();
			}

			ptime GetStart() const
			{
				vector<string> ids = GetItemIdentifiers();
				return from_iso_string(ids[0] + "T000000"); // HACK - cannot parse date otherwise.
			}

			ptime GetEnd() const
			{
				vector<string> ids = GetItemIdentifiers();
				return from_iso_string(ids[ids.size() - 1] + "T000000"); // HACK - cannot parse date otherwise.
			}

			string GetDataSummary() const
			{
				auto start = GetStart();
				auto end = GetEnd();
				string result =
					string("variable name: ") + this->ncVarName +
					string(", identifier: ") + this->identifier +
					string(", start: ") + to_iso_extended_string(start) +
					string(", end: ") + to_iso_extended_string(end) +
					string(", time length: ") + boost::lexical_cast<string>(this->dataAccess->GetTimeLength()) +
					string(", time step: ") + this->dataAccess->GetTimeStep().GetName();
				return result;
			}

		private:
			//int index;
			SwiftNetCDFTimeSeries<T>* netCdfTseries = nullptr;
		 std::map<ptime, int> * indexForTime = nullptr;
		};


		template <typename T>
		class DATATYPES_DLL_LIB MultiFileTimeSeriesEnsembleTimeSeriesStore : public TimeSeriesEnsembleTimeSeriesStore < T >
		{
			//
		public:
			MultiFileTimeSeriesEnsembleTimeSeriesStore(const string& forecastDataFiles, const string& varName, const string& varIdentifier, int index)
			{
				this->forecastDataFiles = forecastDataFiles;
				this->varName = varName;
				this->varIdentifier = varIdentifier;
				this->index = index;
			}

			MultiTimeSeries<TimeSeries*>* Read(const string& fileIdentifier) override
			{
				string fileName(forecastDataFiles);
				boost::algorithm::replace_first(fileName, "{0}", fileIdentifier);
				return TimeSeriesIOHelper<T>::ReadForecastRainfallTimeSeries(fileName, varName, varIdentifier, index);
			}

			virtual ~MultiFileTimeSeriesEnsembleTimeSeriesStore() {/*TODO*/ };

			size_t GetLength() const
			{
				return GetMatchingFiles().size();
			}

			TimeStep GetTimeStep() const
			{
				// TODO
				return TimeStep::GetDaily();
			}

			ptime GetStart() const
			{
				vector<string> ids = GetItemIdentifiers();
				if (ids.size() == 0)
					return not_a_date_time;
				return from_iso_string(ids[0] + "T000000"); // HACK - cannot parse date otherwise.
			}

			ptime GetEnd() const
			{
				vector<string> ids = GetItemIdentifiers();
				if (ids.size() == 0)
					return not_a_date_time;
				return from_iso_string(ids[ids.size() - 1] + "T000000"); // HACK - cannot parse date otherwise.
			}

			string ShortFileNamePattern() const
			{
				namespace fs = boost::filesystem;
				fs::path filesPath(forecastDataFiles);
				auto someDir = filesPath.parent_path();
				string fileNamePattern(filesPath.filename().string());
				return fileNamePattern;
			}

			vector<string> GetItemIdentifiers() const
			{
				vector<string> files = GetMatchingFiles();
				string fileNamePattern = ShortFileNamePattern();
				boost::algorithm::replace_first(fileNamePattern, "{0}", "|");
				vector<string> fixed = Split(fileNamePattern, "|");
				vector<string> ids = files;
				for (int i = 0; i < files.size(); i++)
				{
					string s = files[i];
					for (int j = 0; j < fixed.size(); j++)
						boost::algorithm::replace_all(s, fixed[j], "");
					ids[i] = s;
				}
				std::sort(ids.begin(), ids.end());
				return ids;
			}

			string GetDataSummary() const
			{
				auto start = GetStart();
				auto end = GetEnd();
				string result =
					string("variable name: ") + varName +
					string(", identifier: ") + varIdentifier +
					string(", index: ") + boost::lexical_cast<string>(index) +
					string(", start: ") + to_iso_extended_string(start) +
					string(", end: ") + to_iso_extended_string(end) +
					string(", time length: ") + boost::lexical_cast<string>(GetLength()) +
					string(", time step: <not yet supported>");
				return result;
			}

		private:
			string forecastDataFiles;
			string varName;
			string varIdentifier;
			int index;

			vector<string> Split(const string& s, const string& separators) const
			{
				vector<string> tokens;
				boost::split(tokens, s, boost::is_any_of(separators));
				return tokens;
			}

			vector<string> GetMatchingFiles() const
			{
				namespace fs = boost::filesystem;

				boost::filesystem::path filesPath(forecastDataFiles);
				auto someDir = filesPath.parent_path();

				string fileNamePattern(filesPath.filename().string());
				boost::algorithm::replace_first(fileNamePattern, "{0}", ".*");
#ifdef __GNUC__
				// https ://jira.csiro.au/browse/WIRADA-350  GNU gcc regex bug; use boost instead
#if (__GNUC__ <= 4 && __GNUC_MINOR__ < 9)
				using boost::regex;
				using boost::regex_constants::icase;
				using boost::regex_search;
#else
				using std::regex;
				using std::regex_constants::icase;
				using std::regex_search;
#endif
#else
				using std::regex;
				using std::regex_constants::icase;
				using std::regex_search;
#endif // __GNUC__
				regex rex(fileNamePattern, icase);

				vector<string> files;

				if (fs::exists(someDir) && fs::is_directory(someDir))
				{
					fs::directory_iterator end;
					for (fs::directory_iterator dirIter(someDir); dirIter != end; ++dirIter)
					{
						auto f = dirIter->path().filename();
						if (regex_search(f.string(), rex) &&
							fs::is_regular_file(dirIter->status()))
						{
							files.push_back(f.string());
						}
					}
				}
				return files;
			}
		};

		class DATATYPES_DLL_LIB TimeSeriesLibraryFactory
		{
		public:
			static TimeSeriesLibrary CreateLibrary(const TimeSeriesLibraryDescription& description);

			static SingleTimeSeriesStore<double>* CreateTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static EnsembleTimeSeriesStore<double>* CreateEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static TimeSeriesEnsembleTimeSeriesStore<double>* CreateTsEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);

			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier, int index,
				const string& timeStep, const string& start, int length, int ensembleSize);
			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier, int index,
				const string& timeStep, const string& start, int length, int ensembleSize, int ensembleLength,
				const string& ensembleTimeStep);

		};

	}
}