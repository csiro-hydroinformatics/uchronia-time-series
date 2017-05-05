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

		/** \brief	A class to hold the global attributes of a file stored in the SWIFT netCDF format*/
		class DATATYPES_DLL_LIB GlobalAttributes
		{
		public:
			string Title;
			string Institution;
			string Source;
			string Catchment;
			double STFConventionVersion;
			string STFNCSpec;
			string Comment;
			string History;

			GlobalAttributes();
			GlobalAttributes(const string& title, const string& institution, const string& source, const string& catchment, double stfConventionVersion, const string& stfNcSpec, const string& comment, const string& history);
			GlobalAttributes(GlobalAttributes&& src);
			GlobalAttributes(const GlobalAttributes& src);
			GlobalAttributes& operator=(const GlobalAttributes& src);

			static GlobalAttributes CreateDefault();
			static GlobalAttributes CreateDefault(const string& catchment);
		};

		/** \brief	A class to hold the attributes of a netCDF variable stored in the SWIFT netCDF format*/
		class DATATYPES_DLL_LIB VariableAttributes
		{
		public:
			string LongName;
			string Units;
			int Type = 0;
			string TypeDescription;
			string LocationType;
			string DatType;
			string DatDescription;
			double FillValue;
			static const double DefaultFillValue();
			VariableAttributes();
			VariableAttributes(const string& longName, const string& units, int type, const string& typeDescription, const string& datType, const string& datDescription, const string& locationType, double fillValue);
			VariableAttributes(VariableAttributes&& src);
			VariableAttributes(const VariableAttributes& src);
			VariableAttributes& operator=(const VariableAttributes& src);
		};

		class DATATYPES_DLL_LIB VariableDefinition
		{
			const string kDoublePrecision = DATATYPES_DOUBLE_PRECISION_ID;
			const string kSinglePrecision = DATATYPES_SINGLE_PRECISION_ID;
		public:

			VariableDefinition(const string& name, const string& precision, const string& dimensions, const string& longName, const string& units, double fillValue, int type, const string& typeDescription, const string& datType, const string& datDescription, const string& locationType);
			VariableDefinition(const string& name, const VariableAttributes& attribs, const string& dimensions, const string& precision=DATATYPES_DOUBLE_PRECISION_ID);
			VariableAttributes attributes;
			string Name;
			string Precision;
			string Dimensions;
			int GetPrecision() const;
			static void Split(const std::map<string, VariableDefinition>& varDefinitions, vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);
			//static std::map<string, VariableDefinition> Join(const vector<string>& varNames, const std::map<string, VariableAttributes>& varAttributes);

			VariableDefinition();
			VariableDefinition(VariableDefinition&& src);
			VariableDefinition(const VariableDefinition& src);
			VariableDefinition& operator=(VariableDefinition&& src);
			VariableDefinition& operator=(const VariableDefinition& src);

			static VariableDefinition PointTimeSeries(const string& name, const string& units, const string& longName, int type = 0, const string& typeDescription = "<NA>",
				const string& datType = "<NA>", const string& datDescription = "<NA>", const string& precision = DATATYPES_DOUBLE_PRECISION_ID, double fillValue = DEFAULT_MISSING_DATA_VALUE, const string& locationType = "Point");

			static VariableDefinition TimeSeriesEnsembleTimeSeries(const string& name, const string& units, const string& longName, int type = 0, const string& typeDescription = "<NA>",
				const string& datType = "<NA>", const string& datDescription = "<NA>", const string& precision = DATATYPES_DOUBLE_PRECISION_ID, double fillValue = DEFAULT_MISSING_DATA_VALUE, const string& locationType = "Point");

		};

		class DATATYPES_DLL_LIB DimensionsDefinitions
		{
		public:
			DimensionsDefinitions(const size_t ensembleSize, const vector<double>& leadTimeVar,
				const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER, const string& leadTimeUnits = "");
			DimensionsDefinitions(const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER, const string& leadTimeUnits = "");
			DimensionsDefinitions(ptime tsEnsStart, const TimeStep& mainTimeStep, size_t tsLength, size_t ensembleSize, const TimeStep& fcastTimeStep, 
				size_t leadTimeSize, int fcastOffset = 1, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER, const string& leadTimeUnits = "");
			DimensionsDefinitions();
			DimensionsDefinitions(DimensionsDefinitions&& src);
			DimensionsDefinitions(const DimensionsDefinitions& src);
			DimensionsDefinitions& operator=(DimensionsDefinitions&& src);
			DimensionsDefinitions& operator=(const DimensionsDefinitions& src);

			size_t EnsembleSize;
			vector<double> LeadTimeVar;
			string TimeUnits; 
			vector<double> TimeVar;
			vector<string> StationIds;
			string LeadTimeUnits;
		};

		template<typename ElementType>
		DimensionsDefinitions DimensionsFromSeries(const TTimeSeries<ElementType>& ts, const size_t ensembleSize = 1, const size_t leadTimeSize = 1, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER)
		{
			const int fcastOffset = 1;
			return DimensionsDefinitions(ts.GetStartDate(), ts.GetTimeStep(), ts.GetLength(), ensembleSize, ts.GetTimeStep(), leadTimeSize, fcastOffset, stationIds);
		}

		template<typename ElementType>
		DimensionsDefinitions DimensionsFromSeries(EnsembleForecastTimeSeries<>& tsEns, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER)
		{
			if (tsEns.GetLength() == 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("time series of ensemble forecasts must not be empty to retrieve dimensions");
			auto ens = tsEns.GetValue((size_t)0);
			if (ens == nullptr)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("DimensionsDefinitions - time series of ensemble forecasts: first value must not be null");
			auto s = ens->Get(0);
			if (s == nullptr)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("DimensionsDefinitions - time series of ensemble forecasts: first value in first ensemble must not be null");

			auto tsEnsStart = tsEns.GetStartDate();
			const size_t ensembleSize = ens->Size();
			TimeStep fcastTimeStep = ens->GetTimeStep();
			const size_t leadTimeSize = s->GetLength();
			const ptrdiff_t fcastOffset = fcastTimeStep.GetNumSteps(tsEnsStart, s->GetStartDate()) - 1;
			return DimensionsDefinitions(tsEnsStart, tsEns.GetTimeStep(), tsEns.GetLength(), ensembleSize, ens->GetTimeStep(), leadTimeSize, fcastOffset, stationIds);
		}

		template<typename ElementType>
		DimensionsDefinitions DimensionsFromSeries(
			const vector<EnsembleForecastTimeSeries<>::ElementType>& values, const TimeStep& tsEnsTstep = TimeStep::GetUnknown(), const ptime& tsEnsStart = not_a_date_time, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER, int fcastOffset = 1)
		{
			if (values.empty())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("time series of ensemble forecasts must not be empty to retrieve dimensions");
			auto ens = values[0];
			if (ens == nullptr)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("DimensionsDefinitions - time series of ensemble forecasts: first value must not be null");
			auto s = ens->Get(0);
			if (s == nullptr)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("DimensionsDefinitions - time series of ensemble forecasts: first value in first ensemble must not be null");

			//ptime tsEnsStart = tsEnsStart;
			const ptime& fcastUtcStart = s->GetStartDate();
			const TimeStep& fcastTimeStep = s->GetTimeStep();
			TimeStep mainTimeStep = tsEnsTstep;

			ptime tsOrigin = tsEnsStart;
			if (tsOrigin == not_a_date_time)
				tsOrigin = fcastTimeStep.AddSteps(fcastUtcStart, -fcastOffset);

			if (mainTimeStep.IsUnknown())
				if (values.size() > 1 && values[1] != nullptr)
					mainTimeStep = TimeStep(values[1]->GetStartDate() - values[0]->GetStartDate());
				else
					mainTimeStep = fcastTimeStep;

			const size_t leadTimeSize = s->GetLength();
			const size_t ensembleSize = ens->Size();
			return DimensionsDefinitions(tsOrigin, mainTimeStep, values.size(), ensembleSize, ens->GetTimeStep(), leadTimeSize, fcastOffset, stationIds);
		}

		// A class that can provide the characteristics of a netCDF file for saving time series. 
		// This class is a provision for future feature allowing for a late definition on disk
		// of the file geometry, e.g. when time series to record are reset after creation of the recorder. 
		// Initially, not used to the full "lazy" extent as this is overly complicated for marginal/potential benefit.
		class DATATYPES_DLL_LIB DataGeometryProvider
		{
		public:
			virtual DimensionsDefinitions GetDimensions() const = 0;
			virtual ~DataGeometryProvider();
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

			template<>
			class DATATYPES_DLL_LIB SwiftNetCDFVariablePersister<float>
			{
			public:
				static int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, float *op)
				{
					return nc_get_vara_float(ncid, varid, startp, countp, op);
				}
				static int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const float *op)
				{
					return nc_put_vara_float(ncid, varid, startp, countp, op);
				}
			};

			template<>
			class DATATYPES_DLL_LIB SwiftNetCDFVariablePersister<long>
			{
			public:
				static int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, long *op)
				{
					return nc_get_vara_long(ncid, varid, startp, countp, op);
				}
				static int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const long *op)
				{
					return nc_put_vara_long(ncid, varid, startp, countp, op);
				}
			};

			template<>
			class DATATYPES_DLL_LIB SwiftNetCDFVariablePersister<int>
			{
			public:
				static int NcGetVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, int *op)
				{
					return nc_get_vara_int(ncid, varid, startp, countp, op);
				}
				static int NcPutVara(int ncid, int varid, const size_t *startp,
					const size_t *countp, const int *op)
				{
					return nc_put_vara_int(ncid, varid, startp, countp, op);
				}
			};


			/** \brief	Class responsible for the low-level read/write operations from/to a SWIFT netCDF file */
			class DATATYPES_DLL_LIB SwiftNetCDFAccess
			{
				//

			private:
				static void ThrowOnFileOpenFail(const string& filename, int code);
				static void ThrowOnVarInquiryFail(const string& varName, int code);
				void Init(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
					const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits);
			public:

				/**
				 * \brief	Constructor to wrap an existing SWIFT netCDF file
				 *
				 * \param	filename	SWIFT netCDF file.
				 */
				SwiftNetCDFAccess(const string& filename, bool lazyLoad=false);

				///**
				// * \brief	Constructor to create a new SWIFT netCDF file
				// *
				// * \param	filename			 	name of the new file to create.
				// * \param	nEns				 	Size of the ensembles
				// * \param	nLead				 	Length of the lead time for each of the time series in the ensemble forecast for a given time.
				// * \param	timeUnits			 	Units of the temporal dimension(s).
				// * \param [in]	timeVar		 	The values of the "main" time dimension, consistent with the temporal units given with the previous parameter
				// * \param [in]	stationIds   	List of identifiers for the stations.
				// * \param [in]	varNames	 	List of names of the variables.
				// * \param [in]	varAttributes	Attributes for each variables; the keys of the dictionary must be found in the varNames parameter.
				// */
				//SwiftNetCDFAccess(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				//	const vector<string>& varNames, const std::map<string, VariableAttributes>& varAttributes);

				SwiftNetCDFAccess(const string& filename, const size_t nEns, const vector<double>& leadTimeVar,
					const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
					const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits = "");

				SwiftNetCDFAccess(const string& filename, const DimensionsDefinitions& dimDefinitions,
					const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes);

				~SwiftNetCDFAccess();

				static void CheckCompliance(const string& filename, int majorVersion, int minorVersion, vector<string>& warnings, vector<string>& errors);

				/**
				 * \brief	Gets ensemble size.
				 *
				 * \return	The ensemble size.
				 */
				size_t GetEnsembleSize();

				size_t GetEnsembleSize(const string& ncVarName);

				/**
				 * \brief	Gets the lengths of the lead time dimension.
				 *
				 * \return	The lead time length.
				 */
				size_t GetLeadTimeCount();

				size_t GetLeadTimeCount(const string& ncVarName);

				/**
				* \brief	Gets the lengths of the main time dimension.
				*
				* \return	The time length.
				*/
				size_t GetTimeLength() const;

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
				size_t IndexForIdentifier(const string& identifier) const;

				size_t GetNumIdentifiers() const;

				vector<string> GetIdentifiers() const;

				//int IndexForTime(const ptime* time);

				/**
				 * \brief	Time value for an index of the time dimension.
				 *
				 * \param	timeIndex	Zero-based index of the time.
				 *
				 * \return	A ptime.
				 */
				ptime TimeForIndex(size_t timeIndex);

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

				bool isTimeFastestVarying(const vector<int>& varDimIds)
				{
					return(varDimIds[varDimIds.size() - 1] == this->timeDimId);
				}

			public:

				/**
				* \brief	Gets the values of an ensemble forecast for a variable, for a starting date in the main time dimension.
				*
				* \tparam	ElementType	type of element expected for this variable.
				* \param	varName		   	Name of the variable.
				* \param	stationIndex	The catchment number.
				* \param	timeIndex	   	Zero-based index of the time dimension.
				*
				* \return	null if it fails, else the forecasts.
				*/
				template<typename ElementType>
				vector<ElementType*> * GetForecasts(const string& varName, size_t stationIndex, size_t timeIndex)
				{
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetEnsFcastNetcdfWindow(varName, stationIndex, timeIndex, startp, countp);
					auto vardata = GetForecastDataBuffer();
					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					int code = NcGetVara(ncid, dataVarId, s, c, vardata);
					if (code != NC_NOERR)
					{
						ExceptionUtilities::ThrowInvalidOperation("GetForecasts failed for variable " + varName);
					}
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
				vector<ElementType*> * GetEnsemble(const string& varName, size_t stationIndex)
				{
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, stationIndex, startp, countp);
					auto n = GetTimeLength();
					auto vardata = GetEnsembleDataBuffer(1, n);

					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					int code = NcGetVara(ncid, dataVarId, s, c, vardata);

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
				* \param	stationIndex	The catchment number.
				*
				* \return	All the values for the time series defined with this variable name.
				*/
				template<typename ElementType>
				vector<ElementType> GetValues(const string& varName, size_t stationIndex)
				{
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, stationIndex, startp, countp);
					vector<ElementType> vardata (GetTimeLength());
					ElementType* op = &(vardata[0]);
					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					int code = NcGetVara(ncid, dataVarId, s, c, op);
					if (code != NC_NOERR)
					{
						ExceptionUtilities::ThrowInvalidOperation("GetValues failed for variable " + varName);
					}
					return vardata;
				}

				template<typename ElementType>
				vector<ElementType> GetValues(const string& varName)
				{
					size_t n = GetTimeLength();
					size_t nIds = GetNumIdentifiers();
					size_t N = n * nIds;
					vector<ElementType> values(N);
					int dataVarId = GetVarId(varName);

					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, startp, countp);
					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					ElementType* vardata = values.data();
					int code = NcGetVara(ncid, dataVarId, s, c, vardata);
					if (code != NC_NOERR)
					{
						ExceptionUtilities::ThrowInvalidOperation("GetValues failed for variable " + varName);
					}
					return values;
				}

				template<typename ElementType>
				TTimeSeries<ElementType> * GetSeries(const string& varName, size_t stationIndex)
				{
					auto values = GetValues<ElementType>(varName, stationIndex);
					auto result = new TTimeSeries<ElementType>(values, this->TimeForIndex(0), this->GetTimeStep());
					return result;
				}

				template<typename ElementType>
				MultiTimeSeries<TTimeSeries<ElementType>*>* GetSeries(const string& varName)
				{
					vector<ElementType> values = GetValues<ElementType>(varName);
					size_t tlen = GetTimeLength();
					size_t n = GetNumIdentifiers();
					size_t N = n * tlen;
					if (values.size() != N)
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Inconsistencies between values length and expected total size of data");

					auto varDims = GetVarDims(varName);
					ElementType* d = values.data();
					auto start = TimeForIndex(0);
					auto tstep = GetTimeStep();
					if (isTimeFastestVarying(varDims))
					{
						vector<ElementType*> vv(n);
						for (size_t i = 0; i < n; i++)
						{
							vv[i] = d;
							d += tlen;
						}
						return new MultiTimeSeries<TTimeSeries<ElementType>*>(vv, tlen, GetStart(), GetTimeStep());
					}
					else
					{
						vector<ElementType> v(tlen);
						vector<vector<ElementType>> vv;
						vv.assign(n, v);
						for (size_t t = 0; t < tlen; t++)
						{
							for (size_t s = 0; s < n; s++)
							{
								vv[s][t] = *d;
								d += 1;
							}
						}
						return new MultiTimeSeries<TTimeSeries<ElementType>*>(vv, GetStart(), GetTimeStep());
					}
				}

				template<typename ElementType>
				MultiTimeSeries<TTimeSeries<ElementType>*> * GetEnsembleSeries(const string& varName, size_t stationIndex)
				{
					auto series = GetEnsemble<ElementType>(varName, stationIndex);
					auto result = new MultiTimeSeries<TTimeSeries<ElementType>*>(*series, this->GetTimeLength(), this->TimeForIndex(0), this->GetTimeStep());
					for (auto& d : (*series))
					{
						if (d != nullptr) delete[] d;
					}
					delete series;
					return result;
				}

				/**
				* \brief	Sets the values for an ensemble forecast,  for a variable, for a starting date in the main time dimension.
				*
				* \tparam	ElementType	type of element expected for this variable.
				* \param	varName		   	Name of the variable.
				* \param	stationIndex	The catchment number.
				* \param	timeIndex	   	Zero-based index of the time.
				* \param [in]	values 	[in] the values to write to file.
				*/
				template<typename ElementType>
				void SetForecasts(const string& varName, size_t stationIndex, size_t timeIndex, vector<ElementType*> &values) 	// TODO: checks on 'values'
				{
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetEnsFcastNetcdfWindow(varName, stationIndex, timeIndex, startp, countp);
					auto vardata = GetForecastDataBuffer();

					ElementType * dest;
					for (int i = 0; i < ensembleSize; i++)
					{
						dest = vardata + i * leadTimeLength;
						memcpy(dest, values[i], leadTimeLength * sizeof(ElementType));
					}

					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					int code = NcPutVara(ncid, dataVarId, s, c, vardata);

					delete[] vardata;
				}

				template<typename ElementType>
				void SetEnsembles(const string& varName, size_t stationIndex, vector<ElementType*> &values)
				{
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, stationIndex, startp, countp);
					size_t n = GetTimeLength();
					auto vardata = GetEnsembleDataBuffer(1, n);

					for (int i = 0; i < ensembleSize; i++)
					{
						ElementType* ensData = values[i];
						for (int j = 0; j < n; j++)
						{
							vardata[ensembleSize * j + i] = ensData[j];
						}
					}

					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					int code = NcPutVara(ncid, dataVarId, s, c, vardata);

					delete vardata;
				}

				template<typename ElementType>
				void SetValues(const string& varName, size_t stationIndex, const vector<ElementType>& values)
				{
					size_t n = GetTimeLength();
					if (values.size() != n)
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("vector passed does not have the length of the main time dimension");
					int dataVarId = GetVarId(varName);
					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, stationIndex, startp, countp);
					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					const double* vardata = values.data();
					int code = NcPutVara(ncid, dataVarId, s, c, vardata);
				}

				template<typename ElementType>
				void SetValues(const string& varName, const vector<ElementType>& values)
				{
					size_t n = GetTimeLength();
					size_t nIds = GetNumIdentifiers();
					size_t N = n * nIds;
					if (values.size() != N)
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Inconsistencies between values length and expected total size of data");
					int dataVarId = GetVarId(varName);

					vector<size_t> startp;
					vector<size_t> countp;
					GetNetcdfWindow(varName, startp, countp);
					size_t* s = &(startp[0]);
					size_t* c = &(countp[0]);
					// double q_der[station, time]  in R; rev conventions for C
					const double* vardata = values.data();
					int code = NcPutVara(ncid, dataVarId, s, c, vardata);
					if (code != NC_NOERR)
					{
						ExceptionUtilities::ThrowInvalidOperation("SetValues failed for variable " + varName);
					}
				}

				vector<int> GetVarDims(int varNumDims);
				vector<int> GetVarDims(const string& varName);

				vector<string> ReadVariableNames(bool removeDimVars=true);
				vector<string> ReadAttributeNames(const string& varName);

				string ReadStringAttribute(int varId, const string& attName, bool throwIfNotFound = false, string defaultValue = "");
				string ReadStringAttribute(const string& varName, const string& attName, bool throwIfNotFound = false, string defaultValue = "");
				double ReadNumericAttribute(int varId, const string& attName, bool throwIfNotFound = false, double defaultValue = 0.0);
				double ReadNumericAttribute(const string& varName, const string& attName, bool throwIfNotFound = false, double defaultValue = 0.0);

				VariableAttributes ReadAttributes(const string& varName);
				GlobalAttributes ReadGlobalAttributes();

				template<typename ElementType>
				static string CreateTimeUnitsAttribute(const TTimeSeries<ElementType>& tSeries)
				{
					return CreateTimeUnitsAttribute(tSeries.GetStartDate(), tSeries.GetTimeStep());
				}

				static std::pair<vector<double>, vector<double>> CreateTimeVectors(const ptime& start, const TimeStep& timeStep, size_t tsLength, const TimeStep& leadTimeStep, size_t leadTimeSize, int fcastOffset=1);
				static vector<double> CreateTimeVector(const ptime& start, const TimeStep& timeStep, const ptime& origin, const time_duration& timeStepAxis, const size_t length);
				static vector<double> CreateTimeVector(const ptime& start, const TimeStep& timeStep, const ptime& origin, const TimeStep& timeStepAxis, const size_t length);
				static vector<double> CreateTimeVector(const ptime& start, const TimeStep& timeStep, const size_t length);

				template<typename ElementType>
				static vector<double> CreateTimeVector(const TTimeSeries<ElementType>& tSeries)
				{
					return CreateTimeVector(tSeries.GetStartDate(), tSeries.GetTimeStep(), tSeries.GetLength());
				}

				template<typename ElementType>
				static vector<double> CreateTimeVector(const TTimeSeries<ElementType>& tSeries, const ptime& origin, const TimeStep& timeStepAxis)
				{
					return CreateTimeVector(tSeries.GetStartDate(), tSeries.GetTimeStep(), origin, timeStepAxis, tSeries.GetLength());
				}

				template<typename T>
				static ptime StartCoordinate(const ptime& origin, const TimeStep& timeStep, const vector<T>& timeCoords)
				{
					if (timeCoords.empty())
						ExceptionUtilities::ThrowInvalidArgument("Empty time coordinates");
					return timeStep.AddSteps(origin, timeCoords[0]);
				}

				static std::pair<ptime, TimeStep> CreateTimeGeometry(const string& axisDefinition, const vector<double>& timeCoords);

				static string GetTimeStepName(const TimeStep& timeStep);
				static string CreateTimeUnitsAttribute(const ptime& utcStart, const string& units);
				static string CreateTimeUnitsAttribute(const ptime& utcStart, const TimeStep& timeStep);
				static ptime ParseStartDate(const string& unitsAttribute);
				static string ParseTimeUnits(const string& unitsAttribute);
				static string CreateLeadTimeUnitsAttribute(const TimeStep& timeStep);

				TimeStep GetTimeStep();
				TimeStep GetLeadTimeStep();
				std::pair<ptime, TimeStep> GetLeadTimeGeometry(const ptime& issueTime);
				vector<double> GetLeadTimeDim();

				template<class TFrom, class TTo>
				static vector<TTo> Convert(const vector<TFrom>& from, const std::function<TTo(const TFrom&)>& f)
				{
				  using IterFrom = typename vector<TFrom>::const_iterator;
				  using IterTo = typename vector<TTo>::iterator;
					vector<TTo> result(from.size());
					std::transform<IterFrom, IterTo, std::function<TTo(TFrom)>>
						(from.begin(), from.end(), result.begin(), f);
					return result;
				}

				static time_duration CreateTimeUnits(const TimeStep& timeStep);

				static time_duration TimeOffsetIn;
				static time_duration TimeOffsetOut;

				static void SetTimeOffsetIn(const time_duration& td);
				static void SetTimeOffsetOut(const time_duration& td);

				GlobalAttributes GetGlobalAttributes();
				VariableAttributes GetAttributes(const string& varName);

			private:
				TimeStep GetTimeUnitTimeStep() const;
				void DefineVariable(const VariableDefinition& definition, int& code);
				static TimeStep GetTimeUnitTimeStep(const string& timeUnits);

				template<class TTo>
				static TTo* ConvertToArray(const vector<string>& src)
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
				static TTo* ConvertToArray(const vector<TFrom>& src)
				{
					TTo* result = new TTo[src.size()];
					for (int i = 0; i < src.size(); i++)
					{
						result[i] = (TTo)src[i];
					}
					return result;
				}

				template<class TFrom, class TTo>
				static vector<TTo> Convert(const vector<TFrom>& src)
				{
					vector<TTo> result(src.size());
					for (int i = 0; i < src.size(); i++)
						result[i] = (TTo)src[i];
					return result;
				}

				template<class TTo>
				vector<TTo> Convert(const vector<string>& src)
				{
					using boost::lexical_cast;
					vector<TTo> result(src.size());
					for (int i = 0; i < src.size(); i++)
					{
						result[i] = lexical_cast<TTo>(src[i]);
					}
					return result;
				}


				template<class T>
				static vector<T> ToVector(T* values, size_t n)
				{
					vector<T> result(n);
					result.assign(values, values + n);
					return result;
				}

				template<class T>
				TimeStep FindTimeStep(bool& cachedFlag, TimeStep& timeStep, const vector<T>& timeVec)
				{
					if (cachedFlag)
						return timeStep;
					auto tsu = GetTimeUnitTimeStep();

					if (timeVec.size() <= 1)
					{
						timeStep = tsu;
					}
					else
					{
						if (tsu.IsRegular())
						{
							timeStep = tsu * (timeVec[1] - timeVec[0]);
						}
						else
						{
							if ((timeVec[1] - timeVec[0]) == 1)
							{
								timeStep = tsu;
							}
							else
							{
								exceptions::ExceptionUtilities::ThrowNotSupported("No support for multiple increments of an irregular time step");
							}
						}
					}

					cachedFlag = true;
					return timeStep;
				}

				template<class T>
				static TimeStep FindTimeStep(const TimeStep& axisTimeStep, const vector<T>& timeVec)
				{
					TimeStep timeStep;
					auto tsu = axisTimeStep;
					if (timeVec.size() <= 1)
						timeStep = tsu;
					else
						timeStep = tsu * (timeVec[1] - timeVec[0]);

					return timeStep;
				}

				template<class T>
				static TimeStep FindTimeStep(const string& axisDefinition, const vector<T>& timeVec)
				{
					TimeStep timeStep;
					auto utstep = GetTimeUnitTimeStep(ParseTimeUnits(axisDefinition));
					return FindTimeStep<T>(utstep, timeVec);
				}

				template<class T>
				TimeStep FindTimeStep(const vector<T>& timeVec)
				{
					TimeStep timeStep;
					bool ignored = false;
					return FindTimeStep<double>(ignored, timeStep, timeVec);
				}

				const int kDefaultStrLength = 30;
				static const string kTimeDimName;
				static const string kStationDimName;
				static const string kStrLenDimName;
				static const string kLeadTimeDimName;
				static const string kEnsMemberDimName;

				static const string kTimeVarName;
				static const string kLatVarName;
				static const string kLonVarName;
				static const string kElevationVarName;
				static const string kStationNameVarName;
				static const string kStationIdVarName;
				static const string kLeadTimeVarName;
				static const string kEnsMemberVarName;
				// The following two may be used only as filtering out results of var names queries
				static const string kStrLenVarName;
				static const string kStationVarName;

				vector<string> ReservedVariableNames() const;

				static const string kUnitsAttName;
				static const string kStandardNameAttName;
				static const string kLongNameAttName;
				static const string kAxisAttName;
				static const string kTypeAttName;
				static const string kTypeDescriptionAttName;
				static const string kDatTypeAttName;
				static const string kDatDescriptionAttName;
				static const string kLocationTypeAttName;

				static const string kCatchmentAttName;
				static const string kFillValueAttName;

				static const string kGlobalAttNameTitle;
				static const string kGlobalAttNameInstitution;
				static const string kGlobalAttNameSource;
				static const string kGlobalAttNameCatchment;
				static const string kGlobalAttNameSTF_convention_version;
				static const string kGlobalAttNameSTF_nc_spec;
				static const string kGlobalAttNameComment;
				static const string kGlobalAttNameHistory;

				static const string kAttNameTimeStandard;

				const double kDefaultFillValue = VariableAttributes::DefaultFillValue();

				void ReadGeometry();
				void ReadGeometryDimensions();
				void ReadGeometryVariables();
				void WriteGeometry(size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits = "");
				void DefineMandatoryDimensions(size_t nEns, size_t nLead, size_t nStations);
				void DefineDimVariables();
				void DefineVariables();
				void WriteCommonVarData();
				int AddGlobalAttribute(const string& attName, const string& attValue);
				int AddGlobalAttribute(const string& attName, double attValue);
				int AddGlobalAttribute(char * attName, char * attValue);
				int AddAttribute(int varId, const string& attName, const string& attValue);
				int AddAttribute(int varId, const string& attName, int attValue);
				int AddAttribute(int varId, const string& attName, double attValue);
				void AddVariableAttributes(int varId, const VariableAttributes& varAttributes);
				void AddVariableAttributes(int varId, const string& longName, const string& units, int type, const string& typeDescription, const string& datType, const string& datDescription, double fillValue, const string& locationType);
				void AddAttributes(int varId, const string& varName, std::map<string, VariableAttributes>& varAttributes);
				void FillAttributes(const GlobalAttributes& globalAttributes, const string& timeUnits, const string& leadTimeUnits);
				void SetTimeVar(const vector<double>& timeVar);
				void SetLeadTimeVar(const vector<double>& leadTimeVar);
				void InquireDimIds();
				void InquireDimVarIds();
				size_t InquireDimLength(int dimId);
				size_t GetNumDims(const string& ncVarName);
				void ReadTimeUnits();
				vector<string> * GetStringVariable(size_t strLen, int varId, size_t n);
				nc_type GetDataType(int variableId);
				vector<int> ReadAsInt(int varId, size_t size);
				vector<float> ReadAsFloat(int varId, size_t size, bool strict=true);
				vector<double> ReadAsDouble(int varId, size_t size);

				void ErrorLossPrecision(int varId);

/*				template<class T>
				vector<T> GetVariable(int varId, size_t n)
				{
					if (varId < 0) return nullptr;
					T* result = new T[n];
					int code = nc_get_var(ncid, varId, result);
					if (code != NC_NOERR)
					{
						delete[] result;
						return nullptr;
					}
					return result;
				}
*/
				template<class T>
				void SetVariableDimOne(int varId, T* values, size_t n)
				{
					if (values == nullptr)
						ExceptionUtilities::ThrowInvalidArgument("SwiftNetCDFAccess::SetVariableDimOne: values cannot be a nullptr");
					if (n <= 0)
						ExceptionUtilities::ThrowInvalidArgument("SwiftNetCDFAccess::SetVariableDimOne: data size must be strictly positive");
					//if (varId < 0) return nullptr;
					const size_t startp[1] = { 0 };
					const size_t countp[1] = { (size_t)n };
					int code = NcPutVara<T>(ncid, varId, &(startp[0]), &(countp[0]), values);
					if (code != NC_NOERR)
					{
						ExceptionUtilities::ThrowInvalidOperation("SetVariableDimOne failed for variable ID " + std::to_string(varId));
					}
				}

				template<class T>
				void SetVariableDimOne(int varId, vector<T> values)
				{
					SetVariableDimOne<T>(varId, values.data(), values.size());
				}

				template<class T>
				vector<T> GetVariableDimOne(int varId, size_t n)
				{
					if (n <= 0)
						ExceptionUtilities::ThrowInvalidArgument("SwiftNetCDFAccess::GetVariableDimOne: data size must be strictly positive");
					//if (varId < 0) return nullptr;
					const size_t startp[1] = { 0 };
					const size_t countp[1] = { (size_t)n };
					T* values = new T[n];
					int code = NcGetVara<T>(ncid, varId, &(startp[0]), &(countp[0]), values);
					if (code != NC_NOERR)
					{
						delete values;
						ExceptionUtilities::ThrowInvalidOperation("SetVariableDimOne failed for variable ID " + std::to_string(varId));
					}
					vector<T> result = ToVector(values, n);
					delete values;
					return result;
				}

				template<class T, class TStored>
				vector<T> GetVariableDimOne(int varId, size_t n)
				{
					vector<T> result(n);
					vector<TStored> tmp = GetVariableDimOne<TStored>(varId, n);
					for (int i = 0; i < n; i++)
					{
						result[i] = static_cast<T>(tmp[i]);
					}
					return result;
				}

				template<class T, class TStored>
				void SetVariable(int varId, T* values, size_t n)
				{
					if (values == nullptr)
						ExceptionUtilities::ThrowInvalidArgument("SwiftNetCDFAccess::SetVariable: values cannot be a nullptr");
					TStored* tmp = new TStored[n];
					for (int i = 0; i < n; i++)
					{
						tmp[i] = static_cast<TStored>(values[i]);
					}
					SetVariable<TStored>(varId, tmp, n);
					delete[] tmp;
				}

				/**
				 * \brief	Gets the NetCDF start/count specifications for an ensemble forecast time series.
				 *
				 * \param	stationIndex	The catchment number.
				 * \param	timeIndex	   	Zero-based index of the time.
				 * \param [in,out]	startp 	the dimensions start indices.
				 * \param [in,out]	countp 	the dimensions counts.
				 */
				void GetEnsFcastNetcdfWindow(const string& varName, size_t stationIndex, size_t timeIndex, vector<size_t>& startp, vector<size_t>& countp);

				/**
				* \brief	Gets the NetCDF start/count specifications for the data pertaining to an ensemble of time series.
				*
				* \param	stationIndex	The catchment number.
				* \param [in,out]	startp 	the dimensions start indices.
				* \param [in,out]	countp 	the dimensions counts.
				*/
//				void GetEnsNetcdfWindow(size_t stationIndex, vector<size_t>& startp, vector<size_t>& countp);

				/**
				* \brief	Gets the NetCDF start/count specifications for the data pertaining to a time index.
				*
				* \param	stationIndex	The catchment number.
				* \param [in,out]	startp 	the dimensions start indices.
				* \param [in,out]	countp 	the dimensions counts.
				*/
				void GetNetcdfWindow(const string& varName, size_t stationIndex, vector<size_t>& startp, vector<size_t>& countp);

				void GetNetcdfWindow(const string& varName, vector<size_t>& startp, vector<size_t>& countp);

				int GetVarId(const string& varName);
				double * GetForecastDataBuffer(size_t numStations = 1, size_t numTimeSteps = 1);
				double * GetEnsembleDataBuffer(size_t numStations, size_t numTimeSteps);
				double * GetSingleSeriesDataBuffer(size_t numStations, size_t numTimeSteps);
				int ncid = -1;
				int timeDimId, stationDimId, leadTimeDimId, ensMemberDimId, strLenDimId;
				int timeVarId = -1, stationNameVarId = -1, stationIdVarId = -1, leadTimeVarId = -1, ensMemberVarId = -1, latVarId = -1, lonVarId = -1, elevationVarId = -1;

				ptime startDate;
				string timeUnits;
				TimeStep timeStep;
				TimeStep leadTimeStep;
				bool cachedTimeStep = false;
				bool cachedLeadTimeStep = false;
				size_t /*numTimeSteps, */numStations, strLen;
				bool cachedTimeVector = false;
				vector<ptime> ptimeVec;

				vector<string> * stationNames = nullptr;
				vector<string> * variableVarNames = nullptr;

				// TODO suggest this is strings
				vector<int> stationIds ;

				vector<double> stationLat ;
				vector<double> stationLon ;
				vector<double> stationElevation ;

				vector<float> leadTimeVec /*= nullptr*/;
				size_t leadTimeLength;
				vector<float> timeVec /*= nullptr*/;
				size_t numTimeSteps;

				bool geometryRead = false;

				vector<int> variableVarIds ;

				//int stepMultiplier ;
				size_t ensembleSize;
				string catchmentName;

				//vector<string> variableNames;
				//std::map<string, VariableAttributes> variableAttributes;
				std::map<string, VariableDefinition> variableDefinitions;
			};


			class DATATYPES_DLL_LIB ConfigFileHelper
			{
				using string = std::string;
			private:
				ConfigFileHelper();
			public:
				static const string FileKey;
				static const string VarKey;
				static const string IdentifierKey;
				static const string IdDataKey;
				static const string IndexKey;
				static const string TypeKey;
				static const string TimeStepKey;
				static const string StartKey;
				static const string LengthKey;
				static const string EnsembleSizeKey;
				static const string EnsembleLengthKey;
				static const string EnsembleTimeStepKey;
				static const string FilePatternKey;
				static const string MappingKey;
				static const string StorageKey;

				static const string SingleSeriesTypeId;
				static const string EnsembleSeriesTypeId;
				static const string TimeSeriesEnsemblesTypeId;
				static const string SingleSeriesCollectionTypeId;


				static const string StorageTypeSingleNetcdfFile;
				static const string StorageTypeMultipleNetcdfFiles;

				static TimeSeriesLibraryDescription LoadTimeSeriesLibraryDescription(const string& filename, const string& dataPath = "", TimeSeriesSourceInfoBuilder* srcBuilder = nullptr);
				static void SaveTimeSeriesLibraryDescription(const TimeSeriesLibraryDescription& config, const string& filename);

				static string FileName(const YAML::Node& storage);
				static string FullFileName(const YAML::Node& storage, const TimeSeriesLibraryDescription& tsl);

			private:
				static bool PreCheckStorageType(const string& storageType, TimeSeriesSourceInfoBuilder* srcBuilder = nullptr);

			};

		}

		using namespace datatypes::timeseries::io;

		template<typename TElement>
		DimensionsDefinitions DimensionsFromPointTimeSeries(const TTimeSeries<TElement>& ts)
		{
			string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(ts.GetStartDate(), ts.GetTimeStep());
			auto timeVar = datatypes::utils::SeqVec<double>(0, 1, ts.GetLength());
			return DimensionsDefinitions(timeUnits, timeVar);
		}


		/**
		 * \brief	Representation of an univariate, ensemble time series with a SWIFT netCDF back end.
		 *
		 * \tparam	T	The type of the elements in the series; typically double or float.
		 */
		//template <typename T>
		//class DATATYPES_DLL_LIB NetCdfSingleSeriesStore
		//{
		//	//
		//public:

		//	/**
		//	 * \brief	Constructor.
		//	 *
		//	 * \param [in]          	dataAccess	SWIFT netCDF data access object to read/write the back end file.
		//	 * \param	varName			  	Name of the variable for this time series (the netCDF back end may have several variables).
		//	 * \param	identifier		  	The identifier.
		//	 */
		//	NetCdfSingleSeriesStore(SwiftNetCDFAccess * dataAccess, const string& varName, const string& identifier)
		//	{
		//		this->dataAccess = dataAccess;
		//		this->varName = varName;
		//		this->identifier = identifier;
		//		this->stationIndex = this->GetNcAccess()->IndexForIdentifier(identifier);
		//	}

		//	/**
		//	 * \brief	Gets the ensemble forecast for a given index in the time dimension
		//	 *
		//	 * \param	i	Zero-based index of the time step of interest.
		//	 *
		//	 * \return	a pointer to a new MultiTimeSeries.
		//	 */
		//	MultiTimeSeries<TimeSeries*> * GetForecasts(int i)

		//	{
		//		auto series = this->GetNcAccess()->GetForecasts<T>(varName, stationIndex, i);
		//		auto result = new MultiTimeSeries<TimeSeries*>(*series, this->GetLeadTimeCount(), this->GetNcAccess()->TimeForIndex(i), this->GetTimeStep());
		//		for (auto& d : (*series))
		//		{
		//			if (d != nullptr) delete[] d;
		//		}
		//		delete series;
		//		return result;
		//	}

		//	/**
		//	 * \brief	Gets a non-ensemble time series. There must be such a record, otherwise an exception is thrown.
		//	 *
		//	 * \return	null if it fails, else the series.
		//	 */
		//	TTimeSeries<T> * GetSeries()
		//	{
		//		auto values = this->GetNcAccess()->GetValues<T>(varName, stationIndex);
		//		auto result = new TTimeSeries<T>(values, this->GetTimeLength(), this->TimeForIndex(0), this->GetTimeStep());
		//		delete values;
		//		return result;
		//	}

		//	MultiTimeSeries<TimeSeries*> * GetEnsembleSeries()
		//	{
		//		auto series = this->GetNcAccess()->GetEnsemble<T>(varName, stationIndex);
		//		auto result = new MultiTimeSeries<TimeSeries*>(*series, this->GetTimeLength(), this->TimeForIndex(0), this->GetTimeStep());
		//		for (auto& d : (*series))
		//		{
		//			if (d != nullptr) delete[] d;
		//		}
		//		delete series;
		//		return result;
		//	}

		//	void SetForecasts(int i, MultiTimeSeries<TimeSeries*> * forecasts)
		//	{
		//		this->GetNcAccess()->WriteForecastsVarData();
		//		auto values = forecasts->GetValues();
		//		this->GetNcAccess()->SetForecasts(varName, stationIndex, i, (*values));
		//		for (auto& d : (*values))
		//		{
		//			if (d != nullptr) delete[] d;
		//		}
		//		delete values;
		//	}

		//	void SetEnsemble(MultiTimeSeries<TimeSeries*> * ensemble)
		//	{
		//		this->GetNcAccess()->WriteEnsembleVarData();
		//		vector<T*>* values = ensemble->GetValues();
		//		this->GetNcAccess()->SetEnsembles(varName, stationIndex, *values);
		//		for (T* data : *values)
		//			if (data != nullptr) delete[] data;
		//		delete values;
		//	}

		//	void SetSeries(TTimeSeries<T> * timeSeries)
		//	{
		//		this->GetNcAccess()->WriteSingleSeriesVarData();
		//		T* values = timeSeries->GetValues();
		//		this->GetNcAccess()->SetValues(varName, stationIndex, values);
		//		delete[] values;
		//	}

		//	int GetEnsembleSize()
		//	{
		//		return this->GetNcAccess()->GetEnsembleSize(varName);
		//	}

		//	int GetLeadTimeCount()
		//	{
		//		return this->GetNcAccess()->GetLeadTimeCount(varName);
		//	}

		//	int GetTimeLength()
		//	{
		//		return this->GetNcAccess()->GetTimeLength();
		//	}

		//	TimeStep GetTimeStep()
		//	{
		//		return this->GetNcAccess()->GetTimeStep();
		//	}

		//	ptime TimeForIndex(size_t timeIndex)
		//	{
		//		return this->GetNcAccess()->TimeForIndex(timeIndex);
		//	}

		//private:
		//	SwiftNetCDFAccess * dataAccess = nullptr;
		//	string varName;
		//	string identifier;
		//	size_t stationIndex;
		//};

		//template <typename T = double>
		//class DATATYPES_DLL_LIB NetCdfSingleSeriesStoreStore
		//{
		//	//
		//public:

		//	/**
		//	 * \brief	Create a wrapper time series store around an existing SWIFT netCDF file.
		//	 *
		//	 * \param	filename	Filename of the file.
		//	 */
		//	NetCdfSingleSeriesStoreStore(const string& filename)
		//	{
		//		dataAccess = new SwiftNetCDFAccess(filename);
		//	}

		//	/**
		//	 *\brief	Constructor to create a new SWIFT netCDF file
		//	 *
		//	 * \param	filename			 	name of the new file to create.
		//	 * \param	nEns				 	Size of the ensembles
		//	 * \param	nLead				 	Length of the lead time for each of the time series in the ensemble forecast for a given time.
		//	 * \param	timeUnits			 	Units of the temporal dimension(s).
		//	 * \param[in]	timeVar		 	The values of the "main" time dimension, consistent with the temporal units given with the previous parameter
		//	 * \param[in]	stationIds   	List of identifiers for the stations.
		//	 * \param[in]	varNames	 	List of names of the variables.
		//	 * \param[in]	varAttributes	Attributes for each variables; the keys of the dictionary must be found in the varNames parameter.
		//	 */
		//	NetCdfSingleSeriesStoreStore(const string& filename, size_t nEns, size_t nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds, vector<string>& varNames,
		//		std::map<string, VariableAttributes>& varAttributes = std::map<string, VariableAttributes>())
		//	{
		//		dataAccess = new SwiftNetCDFAccess(filename, nEns, nLead, timeUnits, timeVar, stationIds, varNames, varAttributes);
		//	}

		//	~NetCdfSingleSeriesStoreStore()
		//	{
		//		Close();
		//	}

		//	void Close()
		//	{
		//		if (dataAccess != nullptr)
		//		{
		//			delete dataAccess;
		//			dataAccess = nullptr;
		//		}
		//	}

		//	/**
		//	 * \brief	Create an univariate SWIFT netCDF time series using this netCDF time series store.
		//	 *
		//	 * \param	varName		 	Name of the variable.
		//	 * \param	identifier   	The identifier; e.g. a catchment identifier.
		//	 * \param	dimIdent	 	(Optional) name of the dimension in which to look for the location identifier (e.g. catchment identifier). Defaults to 'station_id' as per SWIFT netCDF schema
		//	 * \param	startTime	 	(Optional) the start time. If null (default), the start time of the whole data set is used.
		//	 * \param	leadTimeCount	(Optional) number of lead times to read. If negative (default), the maximum number of lead times returned by GetLeadTimeCount()
		//	 *
		//	 * \return	null if it fails, else a NetCdfSingleSeriesStore&lt;T&gt;*.
		//	 */
		//	NetCdfSingleSeriesStore<T> * Get(const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1)
		//	{
		//		return new NetCdfSingleSeriesStore<T>(dataAccess, varName, identifier);
		//	}

		//	//void Set(NetCdfSingleSeriesStore<T> * series, const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1);


		//private:
		//	SwiftNetCDFAccess * dataAccess = nullptr;
		//};

		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesIOHelper
		{
		public:
			using SeriesType = typename CommonTypes<T>::SeriesType;
			using PtrSeriesType = typename CommonTypes<T>::PtrSeriesType;
			using EnsemblePtrType = typename CommonTypes<T>::EnsemblePtrType;
			using PtrEnsemblePtrType = typename CommonTypes<T>::PtrEnsemblePtrType;
			using TSeriesEnsemblePtrType = typename CommonTypes<T>::TSeriesEnsemblePtrType;
			using PtrTSeriesEnsemblePtrType = typename CommonTypes<T>::PtrTSeriesEnsemblePtrType;

			static SeriesType* Read(const string& netCdfFilePath, const string& varName, const string& identifier);

			static void Write(const string& varName, std::map<string, TTimeSeries<T>*>& recordedTimeSeries,
				const std::map<string, string>& idMap, const string& filePath);

			static void Write(DimensionsDefinitions& dimDefinitions, const map<std::string, VariableDefinition>& varDefinitions, const GlobalAttributes& GlobalAttributes,
				std::map<string, TTimeSeries<T>*>& recordedTimeSeries, const string& filePath);

			static PtrEnsemblePtrType ReadForecastTimeSeries(const string& netCdfFilepath, const string& varName, const string& identifier, int index);

			static PtrTSeriesEnsemblePtrType ReadForecastTimeSeries(const string& netCdfFilepath, const string& varName, const string& identifier);

			static SeriesType* Read(const string& netCdfFilePath, const string& varName, const string& identifier, const TimeWindow<SeriesType>& window)
			{
				auto tmp = Read(netCdfFilePath, varName, identifier);
				auto result = window.Trim(*tmp);
				delete tmp;
				return result;
			}

			static SeriesType* ReadDailyToHourly(const string& netCdfFilePath, const string& varName, const string& identifier, const TimeWindow<SeriesType>& window)
			{
				TTimeSeries<T>* fullDailyObsPetTimeSeries = Read(netCdfFilePath, varName, identifier);
				TTimeSeries<T>* fullHourlyObsPetTimeSeries = TimeSeriesOperations<SeriesType>::DailyToHourly(*fullDailyObsPetTimeSeries);
				auto result = window.Trim(*fullHourlyObsPetTimeSeries);
				delete fullDailyObsPetTimeSeries;
				delete fullHourlyObsPetTimeSeries;
				return result;
			}
		};

		template <typename T>
		class /*DATATYPES_DLL_LIB*/ SingleNetCdfFileStore
		{

		private:
			string ncVarName;
			string identifier;
			SwiftNetCDFAccess* dataAccess = nullptr;
			string fileName;

		protected:
			SingleNetCdfFileStore() {}
			/**
			* \brief	Constructor to create a new SWIFT netCDF file
			*
			* \param	filename			 	name of the new file to create.
			* \param	nEns				 	Size of the ensembles
			* \param	nLead				 	Length of the lead time for each of the time series in the ensemble forecast for a given time.
			* \param	timeUnits			 	Units of the temporal dimension(s).
			* \param [in]	timeVar		 	The values of the "main" time dimension, consistent with the temporal units given with the previous parameter
			* \param [in]	stationIds   	List of identifiers for the stations.
			* \param [in]	leadTimeUnits	Units of the lead time dimension (if applicable)
			*/
			SingleNetCdfFileStore(const string& fname, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes,
				const string& ncVarName = "", const string& identifier = "", const string& leadTimeUnits = "") :
				ncVarName(ncVarName), identifier(identifier), fileName(fname)
			{
				dataAccess = new SwiftNetCDFAccess(fname, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, globalAttributes, leadTimeUnits);
			}

			SingleNetCdfFileStore(const string& fname, const DimensionsDefinitions& dimDefinitions, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes,
				const string& ncVarName = "", const string& identifier = "") :
				ncVarName(ncVarName), identifier(identifier), fileName(fname)
			{
				dataAccess = new SwiftNetCDFAccess(fname, dimDefinitions, varDefinitions, globalAttributes);
			}
			
			SingleNetCdfFileStore(const string& fname, const string& ncVarName = "", const string& identifier = "", bool writeMode = false) :
				ncVarName(ncVarName), identifier(identifier), fileName(fname)
			{
				if (!writeMode)
					dataAccess = new SwiftNetCDFAccess(fname);
			}

			void Init(const string& filename, const DimensionsDefinitions& dimDefinitions, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes)
			{
				if (dataAccess != nullptr)
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("netCDF single file store already bound to a netCDF file");
				dataAccess = new SwiftNetCDFAccess(filename, dimDefinitions, varDefinitions, globalAttributes);
			}

			void MoveFrom(SingleNetCdfFileStore& src)
			{
				std::swap(dataAccess, src.dataAccess);
				std::swap(fileName, src.fileName);
				std::swap(identifier, src.identifier);
				std::swap(ncVarName, src.ncVarName);
			}

			void CopyFrom(const SingleNetCdfFileStore& src)
			{
				ExceptionUtilities::ThrowNotSupported("Deep copy operations are not supported");
			}

			string GetNcVarName(bool allowDiscovery = true) const
			{
				string varName = ncVarName;
				const bool removeReservedVarnames = true;
				if (varName.empty() && allowDiscovery)
				{
					vector<string> v = this->GetNcAccess()->ReadVariableNames(removeReservedVarnames);
					if (v.size() == 1)
						varName = v[0];
					else if (v.size() > 1)
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation();
					else if (v.empty())
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation();
				}
				return varName;
			}

			string DataSummaryForIdentifier() const
			{
				if (identifier.empty())
					return string(", identifier: <NA>");
				else
					return string(", identifier: ") + this->GetIdentifier();
			}

			string GetDefaultDataSummary() const
			{
				auto start = this->GetStart();
				auto end = this->GetEnd();
				string result =
					string("variable name: ") + this->GetNcVarName() +
					DataSummaryForIdentifier() +
					string(", start: ") + to_iso_extended_string(start) +
					string(", end: ") + to_iso_extended_string(end) +
					string(", time length: ") + boost::lexical_cast<string>(this->GetNcAccess()->GetTimeLength()) +
					string(", time step: ") + this->GetNcAccess()->GetTimeStep().GetName();
				return result;
			}

			size_t IndexForIdentifier(bool strict=true) const
			{
				if (identifier.empty())
				{
					if (strict)
					{
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("This netCDF data provider does not have a unique identifier (station ID)");
					}
					// TODO: reassess how to deal with it, if really acceptable
					if (GetNcAccess()->GetNumIdentifiers() == 1)
						return 0;
				}
				return IndexForIdentifier(this->identifier);
			}

			virtual string GetIdentifier(bool strict = true) const
			{
				if (identifier.empty())
				{
					if (strict)
					{
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("This netCDF data provider does not have a unique identifier (station ID)");
					}
					// TODO: reassess how to deal with it, if really acceptable
					if (GetNcAccess()->GetNumIdentifiers() == 1)
					{
						auto ids = GetIdentifiers();
						if (ids.size() == 1)
							return ids[0];
						datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Inconsistency between netCDF claimed number of identifiers and retrieved list");
					}
				}
				return this->identifier;
			}

			virtual vector<string> GetIdentifiers() const
			{
				if (identifier.empty())
				{
					// TODO: reassess how to deal with it, if really acceptable
					return GetNcAccess()->GetIdentifiers();
				}
				else
					return vector<string>({ this->identifier });
			}

			SwiftNetCDFAccess* GetNcAccess() const
			{
				if (dataAccess == nullptr)
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("The low level swift netCDF data access is a nullptr");
				return dataAccess;
			}

			string& GetFileName() { return fileName; }

		public:
			virtual ~SingleNetCdfFileStore()
			{
				Close();
			}

			virtual void Close()
			{
				if (dataAccess != nullptr)
				{
					delete dataAccess;
					dataAccess = nullptr;
				}
			}

			bool HasNcAccess()
			{
				return (dataAccess != nullptr);
			}

			size_t GetEnsembleSize() const
			{
				auto v = GetNcVarName(false);
				if(v.empty())
					return this->GetNcAccess()->GetEnsembleSize();
				return this->GetNcAccess()->GetEnsembleSize(this->GetNcVarName());
			}

			size_t GetLeadTimeCount() const
			{
				auto v = GetNcVarName(false);
				if (v.empty())
					return this->GetNcAccess()->GetLeadTimeCount();
				return this->GetNcAccess()->GetLeadTimeCount(this->GetNcVarName());
			}

			size_t GetTimeLength() const
			{
				return this->GetNcAccess()->GetTimeLength();
			}

			TimeStep GetTimeStep() const
			{
				return this->GetNcAccess()->GetTimeStep();
			}

			ptime TimeForIndex(size_t timeIndex) const
			{
				return this->GetNcAccess()->TimeForIndex(timeIndex);
			}

			vector<ptime> GetTimeDim() const
			{
				return this->GetNcAccess()->GetTimeDim();
			}

			vector<double> GetLeadTimeDim() const
			{
				return this->GetNcAccess()->GetLeadTimeDim();
			}

			ptime GetStart() const
			{
				return this->GetNcAccess()->GetStart();
			}

			ptime GetEnd() const
			{
				return this->GetNcAccess()->GetEnd();
			}

			size_t IndexForIdentifier(const string& identifier) const
			{
				return this->GetNcAccess()->IndexForIdentifier(identifier);
			}

			//vector<string> GetIdentifiers() const
			//{
			//	return this->GetNcAccess()->GetIdentifiers();
			//}

			VariableAttributes GetVarAttributes()
			{
				std::string ncVarName = GetNcVarName();
				return this->GetNcAccess()->GetAttributes(ncVarName);
			}

			GlobalAttributes GetGlobalAttributes()
			{
				return this->GetNcAccess()->GetGlobalAttributes();
			}
		};


		template <typename T>
		class /*DATATYPES_DLL_LIB*/ NetCdfSingleSeriesStore :
			public SingleTimeSeriesStore<T>,
			public SingleNetCdfFileStore<T>
		{
			//
		public:

			NetCdfSingleSeriesStore(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const string& ncVarName, const string& identifier = "", const string& leadTimeUnits = "") :
				SingleNetCdfFileStore<T>(filename, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, ncVarName, identifier, leadTimeUnits)
			{
			}

			NetCdfSingleSeriesStore(const string& filename, const DimensionsDefinitions& dimDefinitions, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(filename, dimDefinitions, varDefinitions, globalAttributes, ncVarName, identifier)
			{
			}

			NetCdfSingleSeriesStore(const string& fname) :
				SingleNetCdfFileStore<T>(fname)
			{
			}

			NetCdfSingleSeriesStore(const string& fname, const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(fname, ncVarName, identifier)
			{
			}

			NetCdfSingleSeriesStore& operator=(NetCdfSingleSeriesStore&& src)
			{
				// Avoid self assignment
				if (&src == this) {
					return *this;
				}
				SingleNetCdfFileStore<T>::MoveFrom(src);
				return *this;
			}

			NetCdfSingleSeriesStore& operator=(const NetCdfSingleSeriesStore& src)
			{
				if (&src == this) {
					return *this;
				}
				SingleNetCdfFileStore<T>::CopyFrom(src);
				return *this;
			}

			NetCdfSingleSeriesStore(NetCdfSingleSeriesStore&& src)
			{
				*this = src;
			}

			NetCdfSingleSeriesStore(const NetCdfSingleSeriesStore& src)
			{
				*this = src;
			}

			static string Dimensions() { return "2"; }

			string GetDataSummary() const
			{
				return SingleNetCdfFileStore<T>::GetDefaultDataSummary();
			}

			TTimeSeries<T>* Read()
			{
				return Read(this->GetIdentifier());
			}

			TTimeSeries<T>* Read(const string& identifier)
			{
				if (identifier == string(""))
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Cannot get a time series if no identifier is provided.");

				int index = this->IndexForIdentifier(identifier);
				if(index < 0)
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Index not found for data identifier: " + identifier);

				// Need to disambiguate the call to the template method GetSeries for GCC, somehow. Even if this does not look ambiguous.
				// See e.g. http://stackoverflow.com/questions/15572415/expected-primary-expression-before-in-g-but-not-in-microsoft-compiler
				return this->GetNcAccess()->template GetSeries<T>(this->GetNcVarName(), index);
			}

			MultiTimeSeries<TTimeSeries<T>*>* ReadAllCollection()
			{
				MultiTimeSeries<TTimeSeries<T>*>* result = new MultiTimeSeries<TTimeSeries<T>*>();
				// Need to disambiguate the call to the template method GetSeries for GCC, somehow. Even if this does not look ambiguous.
				// See e.g. http://stackoverflow.com/questions/15572415/expected-primary-expression-before-in-g-but-not-in-microsoft-compiler
				return this->GetNcAccess()->template GetSeries<T>(this->GetNcVarName());
			}

			void Write(TTimeSeries<T> * timeSeries)
			{
				vector<T> values = timeSeries->GetValuesVector();
				this->GetNcAccess()->template SetValues<T>(this->GetNcVarName(), this->IndexForIdentifier(), values);
			}

			vector<TTimeSeries<T>*> ReorderPerIdentifier(const std::map<string, TTimeSeries<T>*>& toSave)
			{
				vector<string> ids =this->GetNcAccess()->GetIdentifiers();
				return datatypes::utils::STLHelper::SortValues(toSave, ids);
			}

			vector<T> Serialize(const vector<TTimeSeries<T>*>& series)
			{
				vector<vector<T>> v;
				for (size_t i = 0; i < series.size(); i++)
				{
					v.push_back(series[i]->GetValuesVector());
				}
				return datatypes::utils::STLHelper::Serialize(v);
			}

			void WriteToIdentifiers(const std::map<string, TTimeSeries<T>*>& toSave)
			{
				vector<TTimeSeries<T>*> series = ReorderPerIdentifier(toSave);
				vector<T> values = Serialize(series);
				this->GetNcAccess()->template SetValues<T>(this->GetNcVarName(), values);
			}

			void WriteToNcVariables(const std::map<string, TTimeSeries<T>*>& toSave)
			{
				for (auto& kvp : toSave)
				{
					TTimeSeries<T>* ptr = kvp.second;
					if (ptr != nullptr)
						this->GetNcAccess()->template SetValues<T>(kvp.first, 0, ptr->GetValuesVector());
				}
			}

			vector<string> GetIdentifiers() const
			{
				return SingleNetCdfFileStore<T>::GetIdentifiers();
				vector<string> x = { SingleNetCdfFileStore<T>::GetIdentifier() };
				return x;
			}
		};

		template <typename T>
		class /*DATATYPES_DLL_LIB*/ NetCdfEnsembleTimeSeriesStore : public EnsembleTimeSeriesStore < T >, public SingleNetCdfFileStore<T>
		{
			//
		public:
			NetCdfEnsembleTimeSeriesStore(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& ncVarName, const string& identifier = "", const string& leadTimeUnits = "") :
				SingleNetCdfFileStore<T>(filename, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, globalAttributes, ncVarName, identifier, leadTimeUnits)
			{
			}

			NetCdfEnsembleTimeSeriesStore(const string& filename, const DimensionsDefinitions& dimDefinitions, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes,
				const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(filename, dimDefinitions, varDefinitions, globalAttributes, ncVarName, identifier)
			{
			}

			NetCdfEnsembleTimeSeriesStore(const string& fname, const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(fname, ncVarName, identifier)
			{
			}

			static string Dimensions() { return DATATYPES_THREE_DIMENSIONS_DATA; }

			string GetDataSummary() const
			{
				return SingleNetCdfFileStore<T>::GetDefaultDataSummary();
			}

			vector<string> GetIdentifiers() const 
			{ 
				return SingleNetCdfFileStore<T>::GetIdentifiers();
				vector<string> x = { SingleNetCdfFileStore<T>::GetIdentifier() };
				return x;
			}

			MultiTimeSeries<TTimeSeries<T>*>* Read()
			{
				return this->GetNcAccess()->template GetEnsembleSeries<T>(this->GetNcVarName(), this->IndexForIdentifier());
			}

			void Write(MultiTimeSeries<TTimeSeries<T>*> * ensemble)
			{
				size_t stationIndex = this->IndexForIdentifier();
				vector<T*>* values = ensemble->GetValues();
				this->GetNcAccess()->SetEnsembles(this->GetNcVarName(), stationIndex, *values);
				for (T* data : *values)
					if (data != nullptr) delete[] data;
				delete values;
			}

		};

		template<typename StorageType>
		class EagerWriter
			: public StoragePolicy< StorageType >
		{
		public:
			using EnsemblePtrType = typename TimeSeriesEnsembleTimeSeriesStore<double>::EnsemblePtrType;
			using PtrEnsemblePtrType = StorageType;
			using ElementType = typename EnsemblePtrType::ElementType;
			using TsType = EnsemblePtrType::ItemType;
		private:

			//class MtsRef : public EnsemblePtrType
			//{
			//private:
			//	WritableTimeSeriesEnsembleTimeSeriesStore<T> * store;
			//	size_t index;
			//protected:
			//	void OperatorEqualImpl(const EnsemblePtrType& src) override
			//	{
			//		store->SetItem("", index, src);
			//	}
			//public:
			//	MtsRef(WritableTimeSeriesEnsembleTimeSeriesStore<T> * store, size_t index)
			//	{
			//		this->store = store;
			//		this->index = index;
			//	}

			//};
			vector<PtrEnsemblePtrType> ensemblesProxies;
			WritableTimeSeriesEnsembleTimeSeriesStore<ElementType> * store;

			// bool StoreInitialized() { return store->HasNcAccess(); }

			void resetProxies()
			{
				if(store->IsActive())
					resetProxies(store->GetLength());
			}

			void resetProxies(size_t length)
			{
				for (size_t i = 0; i < ensemblesProxies.size(); i++)
				{
					PtrEnsemblePtrType ptr = ensemblesProxies[i];
					if (ptr != nullptr)
						delete ptr;
				}
				ensemblesProxies.assign(length, nullptr);
			}
		public:
			EagerWriter(WritableTimeSeriesEnsembleTimeSeriesStore<ElementType> * store)
			{
				if (store == nullptr) datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("store must not be nullptr for an EagerWriter");
				this->store = store;
				resetProxies();
			}

			EagerWriter(const EagerWriter& src)
			{
				this->store = src.store;
				resetProxies();
			}

			bool ReadOnly() override { return !store->IsActive(); }

			size_t Size() const
			{
				return store->GetLength();
			}

			void Allocate(size_t length, PtrEnsemblePtrType value)
			{
				store->Allocate(length, value);
				resetProxies();
			}

			void AllocateValues(size_t length, const PtrEnsemblePtrType* values)
			{
				store->AllocateValues(length, values);
				resetProxies();
			}

			void AllocateValues(const vector<PtrEnsemblePtrType>& values)
			{
				store->AllocateValues(values);
				resetProxies();
			}

			void CopyTo(vector<PtrEnsemblePtrType>& dest, size_t from = 0, size_t to = -1) const
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();
				//CheckIntervalBounds(from, to);
				//size_t len = (to - from) + 1;
				//if (dest.size() != len)
				//{
				//	dest.clear();
				//	dest.resize(len);
				//};
				//for (size_t i = 0; i < len; i++)
				//{
				//	dest[i] = (data[i] == nullptr ? store.ReadAt(i) : data[i]);
				//}
			}
		private:

			void CheckDataItemRange(const size_t i) const
			{
				datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(i, 0, store->GetLength(), "index");
			}

			//TimeStep timeStep;
			//ptime start;
			//size_t length = 0;
			//size_t ensembleSize = 0;
			//size_t ensembleLength = 0;
			//TimeStep ensembleTimeStep;

		public:

			PtrEnsemblePtrType& GetProxy(const size_t i) {
				CheckDataItemRange(i);
				auto proxy = ensemblesProxies[i];
				if (proxy == nullptr)
				{
					auto s = new TimeSeriesEnsembleTimeSeriesStoragePolicy<TsType>(store, i);
					proxy = new EnsemblePtrType(s);
					ensemblesProxies[i] = proxy;
				}
				return proxy;
				//const PtrEnsemblePtrType& blah = currentItem;
				//return const_cast<EnsemblePtrType&>(blah);
			}

			PtrEnsemblePtrType& operator[](const size_t i) {
				return GetProxy(i);
			}
			const PtrEnsemblePtrType& operator[](const size_t i) const {
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented("Not sure how to implement this, if at all possible, sorry");
				return new EnsemblePtrType(); // to compile...
			}

			StoragePolicy<PtrEnsemblePtrType>* Clone() const
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("EagerWriter", "Clone");
				return nullptr;
			}

			size_t GetLength() const
			{
				return store->GetLength();
			}

			TimeStep GetTimeStep() const override
			{
				return store->GetTimeStep();
			}

			ptime GetStart() const override
			{
				return store->GetStart();
			}

			void SetTimeStep(const TimeStep& tStep) override
			{
				store->SetTimeStep(tStep);
			}

			void SetStart(const ptime & start) override
			{
				store->SetStart(start);
			}
		};



		template <typename T=double>
		class /*DATATYPES_DLL_LIB*/ NetCdfTimeSeriesEnsembleTimeSeriesStore :
			public WritableTimeSeriesEnsembleTimeSeriesStore < T >,
			public SingleNetCdfFileStore<T>
		{
		public:
			NetCdfTimeSeriesEnsembleTimeSeriesStore(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(filename, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, globalAttributes, ncVarName, identifier)
			{
			}

			NetCdfTimeSeriesEnsembleTimeSeriesStore(const string& filename, const DimensionsDefinitions& dimDefinitions, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes,
				const string& ncVarName, const string& identifier = "") :
				SingleNetCdfFileStore<T>(filename, dimDefinitions, varDefinitions, globalAttributes, ncVarName, identifier)
			{
			}

			NetCdfTimeSeriesEnsembleTimeSeriesStore(const string& fname, const string& ncVarName, const string& varIdentifier, bool writeMode = false) :
				SingleNetCdfFileStore<T>(fname, ncVarName, varIdentifier, writeMode)
			{
			}

			static string Dimensions() { return DATATYPES_FOUR_DIMENSIONS_DATA; }

			using SeriesType = typename CommonTypes<T>::SeriesType;
			using PtrSeriesType = typename CommonTypes<T>::PtrSeriesType;
			using EnsemblePtrType = typename CommonTypes<T>::EnsemblePtrType;
			using PtrEnsemblePtrType = typename CommonTypes<T>::PtrEnsemblePtrType;
			using TSeriesEnsemblePtrType = typename CommonTypes<T>::TSeriesEnsemblePtrType;
			using PtrTSeriesEnsemblePtrType = typename CommonTypes<T>::PtrTSeriesEnsemblePtrType;
			using ElementType = typename EnsemblePtrType::ElementType;

		private:

			EnsembleForecastTimeSeries< TTimeSeries<T> >* ReadSeriesFromFile(const string& dataId)
			{
				EnsembleForecastTimeSeries<TTimeSeries<T>>* result;
				size_t dataLength = this->GetLength();
				ptime start = this->GetStart();
				TimeStep step = this->GetTimeStep();
				result = new EnsembleForecastTimeSeries<TTimeSeries<T>>(dataLength, start, step);
				for (size_t i = 0; i < dataLength; i++)
				{
					auto rawTs = this->GetForecasts(i);
					result->SetValue(i, rawTs);
				}
				return result;
			}

			EnsembleForecastTimeSeries< SeriesType >* CreateWriteProxy(const string& dataId)
			{
				StoragePolicy<PtrEnsemblePtrType> * s = new EagerWriter<PtrEnsemblePtrType>(this);
				return new EnsembleForecastTimeSeries< SeriesType >(s);
			}

		public:
			virtual EnsembleForecastTimeSeries< TTimeSeries<T> >* GetSeries(const string& dataId)
			{
				EnsembleForecastTimeSeries<TTimeSeries<T>>* result;

				if (this->HasNcAccess())
					result = ReadSeriesFromFile(dataId);
				else
					result = CreateWriteProxy(dataId);
				return result;
			}

			vector<string> GetIdentifiers() const
			{
				return SingleNetCdfFileStore<T>::GetIdentifiers();
				vector<string> x = { SingleNetCdfFileStore<T>::GetIdentifier() };
				return x;
			}

			/**
			 * \brief	Gets the ensemble forecast for a given index in the time dimension
			 *
			 * \param	i	Zero-based index of the time step of interest.
			 *
			 * \return	a pointer to a new MultiTimeSeries.
			 */
			PtrEnsemblePtrType GetForecasts(size_t i)
			{
				size_t stationIndex = this->IndexForIdentifier(false);
				auto nc =this->GetNcAccess();
				ptime issueTime = GetTimeForIndex(i);
				std::pair<ptime, TimeStep> fcastGeom = nc->GetLeadTimeGeometry(issueTime);
				vector<ElementType*>* series = nc->template GetForecasts<T>(this->GetNcVarName(), stationIndex, i);
				auto result = new EnsemblePtrType(*series, this->GetLeadTimeCount(), fcastGeom.first, fcastGeom.second);
				for (auto& d : (*series))
				{
					if (d != nullptr) delete[] d;
				}
				delete series;
				return result;
			}

			void SetForecasts(size_t i, MultiTimeSeries<TimeSeries*> * forecasts)
			{
				size_t stationIndex = this->IndexForIdentifier(false);
				auto values = forecasts->GetValues();
				this->GetNcAccess()->SetForecasts(this->GetNcVarName(), stationIndex, i, (*values));
				for (auto& d : (*values))
				{
					if (d != nullptr) delete[] d;
				}
				delete values;
			}

			virtual ~NetCdfTimeSeriesEnsembleTimeSeriesStore()
			{
				if (indexForTime != nullptr)
				{
					delete indexForTime;
					indexForTime = nullptr;
				}

				if (timeForIndex != nullptr)
				{
					delete timeForIndex;
					timeForIndex = nullptr;
				}
			};

			MultiTimeSeries<TTimeSeries<T>*>* Read(const string& ensembleIdentifier) override
			{
				char * end;
				const char* q = ensembleIdentifier.c_str();
				size_t index = -1;
				ptime dateIndex = TimeStep::PtimeFromIsoString(ensembleIdentifier);
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
				return GetForecasts(index);
			}

			size_t GetIndexForTime(const ptime& dateIndex)
			{
				if (indexForTime == nullptr)
				{
					vector<ptime> p = this->GetNcAccess()->GetTimeDim();
					indexForTime = new std::map<ptime, size_t>();
					for (size_t i = 0; i < p.size(); i++)
					{
						(*indexForTime)[p[i]] = i;
					}
				}
				return indexForTime->at(dateIndex);
			}

			ptime GetTimeForIndex(size_t index)
			{
				if (timeForIndex == nullptr)
				{
					timeForIndex = new std::vector<ptime>(this->GetNcAccess()->GetTimeDim());
				}
				return (*timeForIndex)[index];
			}

			//vector<string> GetItemIdentifiers() const
			//{
			//	vector<ptime> times = this->GetNcAccess()->GetTimeDim();
			//	std::function<string(const ptime&)> f = [&](const ptime& p) { return boost::posix_time::to_iso_string(p); };
			//	return SwiftNetCDFAccess::Convert<ptime, string>(times, f);
			//}

			size_t GetLength() const
			{
				return this->GetNcAccess()->GetTimeLength();
			}

			TimeStep GetTimeStep() const
			{
				// TODO: returning this->GetNcAccess()->GetTimeStep() is adequeate for 
				// SWIFT netCDF specs v1, not the most recent 
				return this->GetNcAccess()->GetTimeStep();
			}

			TimeStep GetLeadTimeStep() const
			{
				return this->GetNcAccess()->GetLeadTimeStep();
			}

			ptime GetStart() const
			{
				vector<ptime> times = this->GetNcAccess()->GetTimeDim();
				return times[0];
			}

			ptime GetEnd() const
			{
				vector<ptime> times = this->GetNcAccess()->GetTimeDim();
				return times[times.size()-1];
			}

			string GetDataSummary() const
			{
				return SingleNetCdfFileStore<T>::GetDefaultDataSummary();
			}

			using WritableTimeSeriesEnsembleTimeSeriesStore < T >::GetEnsembleSize;
			using SingleNetCdfFileStore<T>::GetEnsembleSize;

			void Allocate(size_t length, PtrEnsemblePtrType value)
			{
				vector<PtrEnsemblePtrType> v(length);
				v.assign(length, value);
				AllocateValues(v);
			}

			void AllocateValues(const vector<PtrEnsemblePtrType>& values)
			{
				if (IsActive())
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("This netCDF store is already initialised and active");

				DimensionsDefinitions dimDef = DimensionsFromSeries<T>(values);

				string ncVarname(this->GetNcVarName());
				string longName = "<NA>";
				string units = "";
				double fillValue = DEFAULT_MISSING_DATA_VALUE;
				int type = 9;
				string typeDescription = "<NA>";
				string datType = "<NA>";
				string datDescription = "<NA>";

				map<string, VariableDefinition> v;
				v[ncVarname] = VariableDefinition(ncVarname, DATATYPES_DOUBLE_PRECISION_ID, Dimensions(), longName, units, fillValue, type, typeDescription, datType, datDescription, "Point");
				GlobalAttributes globAtts = GlobalAttributes::CreateDefault();
				SingleNetCdfFileStore<T>::Init(this->GetFileName(), dimDef, v, globAtts);
				for (size_t i = 0; i < values.size(); i++)
				{
					SetItem(ncVarname, i, values[i]);
				}
			}

			void SetSeries(const string& dataId, PtrTSeriesEnsemblePtrType value)
			{
				for (size_t i = 0; i < value->GetLength(); i++)
				{
					SetItem(this->GetNcVarName(), i, value->GetValue(i));
				}
			}

			void SetItem(const string& dataId, size_t index, PtrEnsemblePtrType value)
			{
				SetForecasts(index, value);
			}

			void SetItem(const string& dataId, size_t index, const EnsemblePtrType& value)
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();
			}

			//SetItem(const string& dataId, PtrEnsemblePtrType) = 0;
			//EnsemblePtrType Read(const string& ensembleIdentifier) = 0;
			void SetLength(size_t length)
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();
			}

			void SetStart(ptime start)
			{
				// TODO? can it be done after initial allocation?
			}

			//vector<string> GetItemIdentifiers() const = 0;
			void SetTimeStep(const TimeStep&)
			{
				// TODO? can it be done after initial allocation?
			}

			bool IsActive()
			{
				return (this->HasNcAccess());
			}

		private:
			std::vector<ptime> * timeForIndex = nullptr;
			std::map<ptime, size_t> * indexForTime = nullptr;
		};


		template <typename T>
		class MultiFileTimeSeriesEnsembleTimeSeriesStore;

		/**
		* \brief	An implementation of StoragePolicy such that the content of a 
		* 			time series is spread amongst several files.
		*
		* \tparam	T	The type of each data item this can handle.
		*/
		template <typename T>
		class MultiFileTsStorage
			: public StoragePolicy<T>
		{
		public:
			// Implicitely here, we are dealing only with time series of ensemble of time series
			typedef typename std::remove_pointer<T>::type::ElementType ElementType;
		private:
			MultiFileTsStorage(const MultiFileTsStorage& src)
			{
				store = src.store;
				// We do not copy the data vector!
				ReserveVectorData(store.GetLength());
				readOnly = src.readOnly;
			}

			MultiFileTimeSeriesEnsembleTimeSeriesStore<ElementType> store;

			void CheckIntervalBounds(const size_t& from, size_t& to) const
			{
				size_t tsLen = this->GetLength();
				datatypes::exceptions::RangeCheck<size_t>::CheckTimeSeriesInterval(from, to, tsLen);
			}
		public:
			MultiFileTsStorage(const MultiFileTimeSeriesEnsembleTimeSeriesStore<ElementType>& storage, const string& dataIdentifier, bool readOnly = true)
			{
				store = storage;
				size_t length = store.GetLength();
				ReserveVectorData(length);
				this->readOnly = readOnly;
			}

			vector<T> data;
			bool readOnly = true;

			bool ReadOnly() override { return readOnly; }

			size_t Size() const 
			{ 
				return data.size();
			}

			void ReserveVectorData(size_t length)
			{
				if (!data.empty())
				{
					for (size_t i = 0; i < data.size(); i++)
					{
						auto ptr = data[i];
						if (ptr != nullptr)
							delete ptr;
					}
					data.clear();
				}
				if (length > 0)
				{
					data.reserve(length);
					data.assign(length, nullptr);
				}
			}

			void Allocate(size_t length, T value)
			{
				if(ReadOnly())
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("MultiFileTsStorage::Allocate cannot be called if the storage is read-only");
				else
				{
					ReserveVectorData(length);
					data.assign(length, value);
				}
			}
			void AllocateValues(size_t length, const T* values)
			{
				if (ReadOnly())
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("MultiFileTsStorage::AllocateValues cannot be called if the storage is read-only");
				else
				{
					ReserveVectorData(length);
					data.assign(values, values + length);
				}
			}
			void AllocateValues(const vector<T>& values)
			{
				if (ReadOnly())
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("MultiFileTsStorage::AllocateValues cannot be called if the storage is read-only");
				else 
				{
					ReserveVectorData(values.size());
					data.assign(values.begin(), values.end());
				}
			}

			void CopyTo(vector<T>& dest, size_t from = 0, size_t to = -1) const
			{
				CheckIntervalBounds(from, to);
				size_t len = (to - from) + 1;
				if (dest.size() != len)
				{
					dest.clear();
					dest.resize(len);
				};
				for (size_t i = 0; i < len; i++)
				{
					dest[i] = (data[i] == nullptr ? store.ReadAt(i) : data[i]);
				}
			}
		private:

			void CheckDataItemRange(const size_t i) const
			{
				if (data.empty())
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("No data item present in this MultiFileTsStorage");
				datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(i, 0, data.size(), "index");
			}
		public:
			T& operator[](const size_t i) {
				CheckDataItemRange(i);
				if (data[i] == nullptr)
					data[i] = store.ReadAt(i);
				return data[i];
			}
			const T& operator[](const size_t i) const {
				CheckDataItemRange(i);
				return data[i];
			}

			StoragePolicy<T>* Clone() const 
			{ 
				return new MultiFileTsStorage<T>(*this); 
			}

			size_t GetLength() const
			{
				return store.GetLength();
			}

			TimeStep GetTimeStep() const override
			{
				return store.GetTimeStep();
			}

			ptime GetStart() const override
			{
				return store.GetStart();
			}

			void SetTimeStep(const TimeStep& tStep) override
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();
			}

			void SetStart(const ptime & start) override
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();
			}
		};


		/**
		* \brief	An implementation of TimeSeriesEnsembleTimeSeriesStore such that the content of a
		* 			time series is spread amongst several files.
		*
		* \tparam	T	The type of each data item this can handle.
		*/
		template <typename T>
		class /*DATATYPES_DLL_LIB*/ MultiFileTimeSeriesEnsembleTimeSeriesStore : 
			public TimeSeriesEnsembleTimeSeriesStore < T >
		{
		public:

			string DatePattern = "YYYYMMDD";
			string FileNamePattern = datatypes::io::IoHelper::DefaultFilePattern;

			/**
			 * \brief	Default constructor. Needed for class member properties; should otherwise not be used.
			 */
			MultiFileTimeSeriesEnsembleTimeSeriesStore()
			{
			}
			
			MultiFileTimeSeriesEnsembleTimeSeriesStore(const string& forecastDataFiles, const string& varName, const string& varIdentifier, int index,
				const TimeStep& timeStep, const ptime& start, int length,
				int ensembleSize, int ensembleLength, const TimeStep& ensembleTimeStep)
			{
				this->forecastDataFiles = forecastDataFiles;
				this->varName = varName;
				this->varIdentifier = varIdentifier;
				this->index = index;

				this->timeStep = timeStep;
				this->start = start;
				this->length = length;
				this->ensembleSize = ensembleSize;
				this->ensembleLength = ensembleLength;
				this->ensembleTimeStep = ensembleTimeStep;

			}

			virtual ~MultiFileTimeSeriesEnsembleTimeSeriesStore() {/*TODO*/ };

			using SeriesType = typename CommonTypes<T>::SeriesType;

			EnsembleForecastTimeSeries< SeriesType >* GetSeries(const string& dataId)
			{
				// Buckle up...
				// T is of double
				// if T is double, SeriesType is TTimeSeries<T>
				// if T is double, SeriesTypePtr is TTimeSeries<T>*
				// EnsembleForecastTimeSeries< SeriesType > is an EnsembleForecastTimeSeries<TTimeSeries<T>>
				// which is a PointerTypeTimeSeries < MultiTimeSeriesPtr<SeriesType> >
				// which is a PointerTypeTimeSeries < MultiTimeSeriesPtr<TTimeSeries<T> >
				// which is a TTimeSeries < MultiTimeSeriesPtr<TTimeSeries<T>* >
				// which is a TTimeSeries < MultiTimeSeries<TTimeSeries<T*>*>
				// so the type parameters of the storage policy must be a type MultiTimeSeries<TTimeSeries<T*>*
				// 
				// That all said:
				// an EnsembleForecastTimeSeries is a template typename for a TTimeSeries, so a simpler way to specify the storage type is:
				using StorageType = typename EnsembleForecastTimeSeries< SeriesType >::ElementType;
				StoragePolicy<StorageType>* s = new MultiFileTsStorage<StorageType>(*this, dataId);
				return new EnsembleForecastTimeSeries< SeriesType >(s);
			}

		private:
			MultiTimeSeries<SeriesType*>* ReadByFileId(const string& fileIdentifier) const
			{
				using namespace boost::filesystem;
				using datatypes::io::IoHelper;
				string fileName = IoHelper::MakeFileName(forecastDataFiles, fileIdentifier);
				path p(fileName);
				if (exists(p) && is_regular_file(p))
					return TimeSeriesIOHelper<T>::ReadForecastTimeSeries(fileName, varName, varIdentifier, index);
				else
					return nullptr;
			}

			string GetItemIdentifierForIndex(size_t index) const
			{
				ptime t = timeStep.AddSteps(start, index);
				return TimeStep::ToString(t, DatePattern);
			}

			string GetFilenameForItem(size_t index) const
			{
				//"blah/sample_data\UnitTests\netcdf\testpet{0}.nc"
				string fn = ShortFileNamePattern();
				string s = GetItemIdentifierForIndex(index);
				boost::algorithm::replace_first(fn, FileNamePattern, s);
				return fn;
			}

		public:

			MultiTimeSeries<SeriesType*>* Read(const string& fileIdentifier) override
			{
				return ReadByFileId(fileIdentifier);
			}

			MultiTimeSeries<SeriesType*>* ReadAt(size_t index) const
			{
				datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(index, 0, this->length - 1, "index");
				string id = GetItemIdentifierForIndex(index);
				return ReadByFileId(id);
			}

			size_t GetLength() const
			{
				return this->length;
				//return GetMatchingFiles().size();
			}

			TimeStep GetTimeStep() const
			{
				return timeStep;
			}

			ptime GetStart() const
			{
				return start;
			}

			ptime GetEnd() const
			{
				if (length < 0)
					return start;
				size_t n = length;
				return timeStep.AddSteps(start, n);
			}

			string ShortFileNamePattern() const
			{
				namespace fs = boost::filesystem;
				fs::path filesPath(forecastDataFiles);
				auto someDir = filesPath.parent_path();
				string fileNamePattern(filesPath.filename().string());
				return fileNamePattern;
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
			//string ncVarName;
			string varName;
			//string identifier;
			string varIdentifier;
			int index;

			TimeStep timeStep;
			ptime start;
			int length = -1;
			int ensembleSize = -1;
			int ensembleLength = -1;
			TimeStep ensembleTimeStep;

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
				boost::algorithm::replace_first(fileNamePattern, FileNamePattern, ".*");
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
		private:
			static void CreateLibrary(const TimeSeriesLibraryDescription& description, TimeSeriesLibrary& result);
		public:

			using PtrSeriesType = TTimeSeries<double>*;
			using SeriesType = TTimeSeries<double>;

			static TimeSeriesLibrary CreateLibrary(const TimeSeriesLibraryDescription& description);
			static TimeSeriesLibrary* CreateLibraryPtr(const TimeSeriesLibraryDescription& description);

			static TimeSeriesLibrary LoadTimeSeriesLibrary(const string& filepath, const string& dataPath);
			static TimeSeriesLibrary* LoadTimeSeriesLibraryPtr(const string& filepath, const string& dataPath);

			static TimeSeriesLibrary* CreateTimeSeriesLibraryPtr(const string& type);

			static SingleTimeSeriesStore<double>* CreateTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static EnsembleTimeSeriesStore<double>* CreateEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static TimeSeriesEnsembleTimeSeriesStore<double>* CreateTsEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier);

			static EnsembleForecastTimeSeries< SeriesType >* LoadTsEnsTs(const string& ncFilename, const string& ncVarName, const string& ncIdentifier, const string& dataId);

			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier);
			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier, int index,
				const string& timeStep, const string& start, int length, int ensembleSize);
			static TimeSeriesSourceInfo CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& ncFilename, const string& ncVarName, const string& ncIdentifier, int index,
				const string& timeStep, const string& start, int length, int ensembleSize, int ensembleLength,
				const string& ensembleTimeStep);

			static const string kTestRecorderKey;
			static const string kTimeSeriesEnsemblesKey;

			static bool HasTimeSeriesSourceInfoBuilderRegistered();
			static void RegisterTimeSeriesSourceInfoBuilder(TimeSeriesSourceInfoBuilder* srcBuilder);
		private:
			static TimeSeriesSourceInfoBuilder* GetBuilder();
			static TimeSeriesSourceInfoBuilder* infoBuilder;


		};

		class DATATYPES_DLL_LIB SwiftNetcdfStoreFactory :
			public TimeSeriesStoreFactory
		{
		private:
			string dirPath;
			DataGeometryProvider* geometryProvider;
			boost::filesystem::path CreateNcFilename(const string& dataId)
			{
				return boost::filesystem::path(dirPath) / (dataId + ".nc");
			}

		public:
			SwiftNetcdfStoreFactory(const string& path, DataGeometryProvider* dgp);
			~SwiftNetcdfStoreFactory();
			TimeSeriesEnsembleTimeSeriesStore<double>* CreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId);
			bool CanCreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId);
		};


	}
}
