#pragma once

#include "time_series.h"

#include <netcdf.h>
#include <map>

namespace datatypes
{
	namespace timeseries
	{
		class DLL_LIB VariableAttributes
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
			class DLL_LIB SwiftNetCDFAccess
			{
				using string = std::string;

			public:
				SwiftNetCDFAccess(const string& filename);
				SwiftNetCDFAccess(const string& filename, int nEns, int nLead, const string& timeUnits, std::vector<double>& timeVar, std::vector<string>& stationIds, std::vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);
				~SwiftNetCDFAccess();

				int GetEnsembleSize();
				int GetLeadTimeCount();
				int GetTimeLength();
				ptime * GetTimeDim();
				ptime GetStart();
				int IndexForIdentifier(const string& identifier);
				//int IndexForTime(const ptime* time);
				ptime TimeForIndex(int timeIndex);
				std::vector<double*> * GetForecasts(const string& varName, int catchmentNumber, int timeIndex);
				void SetForecasts(const string& varName, int catchmentNumber, int timeIndex, std::vector<double*> &values);

				static string CreateTimeUnitsAttribute(const ptime& utcStart, const string& units);
				static ptime ParseStartDate(const string& unitsAttribute);
				static string ParseTimeUnits(const string& unitsAttribute);

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
				void DefineVariables(std::vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes);
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

				void GetNetcdWindow(int catchmentNumber, int timeIndex, size_t *startp, size_t *countp);
				int GetVarId(const string& varName);
				double * GetForecastDataBuffer(int numStations = 1, int numTimeSteps = 1);
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
				int * variableVarIds;
				int leadTimeLength;
				//int stepMultiplier;
				int ensembleSize;
				string catchmentName;
			};
		}

		using namespace datatypes::timeseries::io;

		template <typename T>
		class DLL_LIB SwiftNetCDFTimeSeries
		{
			using string = std::string;
		public:
			SwiftNetCDFTimeSeries(SwiftNetCDFAccess * dataAccess, const string& varName, const string& identifier);
			MultiTimeSeries<T> * GetForecasts(int i);
			void SetForecasts(int i, MultiTimeSeries<T> * forecasts);
			int GetEnsembleSize();
			int GetLeadTimeCount();
			int GetTimeLength();
			ptime TimeForIndex(int timeIndex);

		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
			string varName;
			string identifier;
			int catchmentNumber;
		};

		template <typename T>
		class DLL_LIB SwiftTimeSeriesStore
		{
			using string = std::string;
		public:
			virtual SwiftNetCDFTimeSeries<T> * Get(const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1) = 0;
			//virtual void Set(SwiftNetCDFTimeSeries<T> * series, const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1) = 0;
			virtual int GetEnsembleSize() = 0;
			virtual int GetLeadTimeCount() = 0;
			virtual ptime * GetTimeDim() = 0;
			virtual ptime GetStart() = 0;
			// ??? GetValues(const string& varName) = 0;
			virtual int IndexForIdentifier(const string& identifier) = 0;
			//virtual int IndexForTime(const ptime* time) = 0;

		};

		template <typename T>
		class DLL_LIB SwiftNetCDFTimeSeriesStore : public SwiftTimeSeriesStore < T >
		{
			using string = std::string;
		public:
			SwiftNetCDFTimeSeriesStore(const string& filename);
			SwiftNetCDFTimeSeriesStore(const string& filename, int nEns, int nLead, const string& timeUnits, std::vector<double>& timeVar, std::vector<string>& stationIds, std::vector<string>& varNames,
				std::map<string, VariableAttributes>& varAttributes = std::map<string, VariableAttributes>());
			~SwiftNetCDFTimeSeriesStore();

			SwiftNetCDFTimeSeries<T> * Get(const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1) override;
			//void Set(SwiftNetCDFTimeSeries<T> * series, const string& varName, const string& identifier, const string& dimIdent = "station_id", const ptime* startTime = nullptr, int leadTimeCount = -1) override;
			int GetEnsembleSize() override;
			int GetLeadTimeCount() override;
			ptime * GetTimeDim() override;
			int GetTimeLength();
			ptime GetStart() override;
			int IndexForIdentifier(const string& identifier) override;
			//int IndexForTime(const ptime* time) override;

		private:
			SwiftNetCDFAccess * dataAccess = nullptr;
		};
	}
}