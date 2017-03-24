
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "datatypes/exception_utilities.h"
#include "datatypes/time_series_io.hpp"
#include "datatypes/time_series_store.hpp"

namespace datatypes
{
	namespace timeseries
	{
		GlobalAttributes::GlobalAttributes() {}

		GlobalAttributes::GlobalAttributes(const string& title, const string& institution, const string& source, const string& catchment, double stfConventionVersion, const string& stfNcSpec, const string& comment, const string& history)
		{
			Title = title;
			Institution = institution;
			Source = source;
			Catchment = catchment;
			STFConventionVersion = stfConventionVersion;
			STFNCSpec = stfNcSpec;
			Comment = comment;
			History = history;
		}

		GlobalAttributes::GlobalAttributes(GlobalAttributes&& src) {
			*this = std::move(src);
		}

		GlobalAttributes::GlobalAttributes(const GlobalAttributes& src)
		{
			Title = src.Title;
			Institution = src.Institution;
			Source = src.Source;
			Catchment = src.Catchment;
			STFConventionVersion = src.STFConventionVersion;
			STFNCSpec = src.STFNCSpec;
			Comment = src.Comment;
			History = src.History;
		}

		GlobalAttributes& GlobalAttributes::operator=(const GlobalAttributes& src)
		{
			if (&src == this) {
				return *this;
			}
			Title = src.Title;
			Institution = src.Institution;
			Source = src.Source;
			Catchment = src.Catchment;
			STFConventionVersion = src.STFConventionVersion;
			STFNCSpec = src.STFNCSpec;
			Comment = src.Comment;
			History = src.History;
			return *this;
		}

		GlobalAttributes GlobalAttributes::CreateDefault()
		{
			return CreateDefault("<unspecified>");
		}

		GlobalAttributes GlobalAttributes::CreateDefault(const string& catchment)
		{
			string timeStamp = boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::universal_time());
			string history = timeStamp + " UTC - New file created";

			return GlobalAttributes(
				"",
				"CSIRO Land and Water",
				"SWIFT (devel version)",
				string(catchment),
				2.0,
				"https://confluence.csiro.au/display/wirada/netCDF+for+Water+Forecasting+Specification+v2.0",
				"",
				history);
		}


		const double VariableAttributes::DefaultFillValue() 
		{ 
			return DEFAULT_MISSING_DATA_VALUE; 
		}
		
		VariableAttributes::VariableAttributes() 
		{
			FillValue = DefaultFillValue(); 
		}
		
		VariableAttributes::VariableAttributes(const string& longName, const string& units, int type, const string& typeDescription, const string& datType, const string& datDescription, const string& locationType, double fillValue)
		{
			LongName = longName;
			Units = units;
			Type = type;
			TypeDescription = typeDescription;
			DatType = datType;
			DatDescription = datDescription;
			LocationType = locationType;
			FillValue = fillValue;
		}

		VariableAttributes::VariableAttributes(VariableAttributes&& src) {   // Move constructor.
			*this = std::move(src);
		}

		VariableAttributes::VariableAttributes(const VariableAttributes& src) {
			LongName = src.LongName;
			Units = src.Units;
			Type = src.Type;
			TypeDescription = src.TypeDescription;
			DatType = src.DatType;
			DatDescription = src.DatDescription;
			LocationType = src.LocationType;
			FillValue = src.FillValue;
		}

		VariableAttributes& VariableAttributes::operator=(const VariableAttributes& src)
		{
			if (&src == this) {
				return *this;
			}
			LongName = src.LongName;
			Units = src.Units;
			Type = src.Type;
			TypeDescription = src.TypeDescription;
			DatType = src.DatType;
			DatDescription = src.DatDescription;
			LocationType = src.LocationType;
			FillValue = src.FillValue;
			return *this;
		}

		VariableDefinition::VariableDefinition(const string& name, const string& precision, const string& dimensions, const string& longName, const string& units, double fillValue, int type, const string& typeDescription, const string& datType, const string& datDescription, const string& locationType) :
			attributes(VariableAttributes(longName, units, type, typeDescription, datType, datDescription, locationType, fillValue))
		{
			Name = name;
			Precision = precision;
			Dimensions = dimensions;
		}

		VariableDefinition::VariableDefinition(const string& name, const VariableAttributes& attribs, const string& dimensions, const string& precision) :
			attributes(attribs)
		{
			Name = name;
			Precision = precision;
			Dimensions = dimensions;
		}

		int VariableDefinition::GetPrecision() const
		{
			if (Precision == kSinglePrecision)
				return NC_FLOAT;
			else if (Precision == kDoublePrecision)
				return NC_DOUBLE;
			else
				ExceptionUtilities::ThrowInvalidArgument("Unknown variable precision specifier: " + Precision);
			return NC_NAT;
		}

		void VariableDefinition::Split(const std::map<string, VariableDefinition>& varDefinitions, vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes)
		{
			for (const auto& x : varDefinitions)
			{
				varNames.push_back(x.first);
				varAttributes[x.first] = varDefinitions.at(x.first).attributes;
			}
		}

		VariableDefinition::VariableDefinition() {  
		}

		VariableDefinition::VariableDefinition(VariableDefinition&& src) {   // Move constructor.
			*this = std::move(src);
		}

		VariableDefinition::VariableDefinition(const VariableDefinition& src) {
			Name = src.Name;
			Precision = src.Precision;
			Dimensions = src.Dimensions;
			attributes = src.attributes;
		}

		VariableDefinition& VariableDefinition::operator=(VariableDefinition&& src) {
			// Avoid self assignment
			if (&src == this) {
				return *this;
			}
			std::swap(Name, src.Name);
			std::swap(Precision, src.Precision);
			std::swap(Dimensions, src.Dimensions);
			std::swap(attributes, src.attributes);
			return *this;
		}

		VariableDefinition& VariableDefinition::operator=(const VariableDefinition& src)
		{
			if (&src == this) {
				return *this;
			}
			Name = src.Name;
			Precision = src.Precision;
			Dimensions = src.Dimensions;
			attributes = src.attributes;
			return *this;
		}

		VariableDefinition VariableDefinition::PointTimeSeries(const string& name, const string& units, const string& longName, int type, const string& typeDescription,
			const string& datType, const string& datDescription, const string& precision, double fillValue, const string& locationType)
		{
			return VariableDefinition(name, precision, "2", longName, units, fillValue, type, typeDescription, datType, datDescription, locationType);
		}

		VariableDefinition VariableDefinition::TimeSeriesEnsembleTimeSeries(const string& name, const string& units, const string& longName, int type, const string& typeDescription,
			const string& datType, const string& datDescription, const string& precision, double fillValue, const string& locationType)
		{
			return VariableDefinition(name, precision, DATATYPES_FOUR_DIMENSIONS_DATA, longName, units, fillValue, type, typeDescription, datType, datDescription, locationType);
		}



		//std::map<string, VariableDefinition> VariableDefinition::Join(const vector<string>& varNames, const std::map<string, VariableAttributes>& varAttributes)
		//{
		//	std::map<string, VariableDefinition> result;
		//	for (const string& x : varNames)
		//	{
		//		result[x] = VariableDefinition(x, varAttributes.at(x), "double");
		//	}
		//	return result;
		//}

		DimensionsDefinitions::DimensionsDefinitions(const size_t ensembleSize, const vector<double>& leadTimeVar,
			const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds, const string& leadTimeUnits)
		{
			EnsembleSize = ensembleSize;
			LeadTimeVar = leadTimeVar;
			TimeUnits = timeUnits;
			TimeVar = timeVar;
			StationIds = stationIds;
			LeadTimeUnits = leadTimeUnits;
		}

		DimensionsDefinitions::DimensionsDefinitions(const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds, const string& leadTimeUnits)
		{
			EnsembleSize = 1;// 0;
			LeadTimeVar = vector<double>();
			LeadTimeVar.push_back(0);
			TimeUnits = timeUnits;
			TimeVar = timeVar;
			StationIds = stationIds;
			LeadTimeUnits = leadTimeUnits;
		}

		DimensionsDefinitions::DimensionsDefinitions(ptime tsEnsStart, const TimeStep& mainTimeStep, size_t tsLength, size_t ensembleSize, const TimeStep& fcastTimeStep, size_t leadTimeSize, int fcastOffset, const vector<string>& stationIds, const string& leadTimeUnits)
		{
			TimeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(tsEnsStart, mainTimeStep);
			auto timeVecs = SwiftNetCDFAccess::CreateTimeVectors(tsEnsStart, mainTimeStep, tsLength, fcastTimeStep, leadTimeSize, fcastOffset);
			TimeVar = timeVecs.first;
			LeadTimeVar = timeVecs.second;
			EnsembleSize = ensembleSize;
			StationIds = stationIds;
			LeadTimeUnits = leadTimeUnits;
		}

		DimensionsDefinitions::DimensionsDefinitions() {
		}

		DimensionsDefinitions::DimensionsDefinitions(DimensionsDefinitions&& src) {   // Move constructor.
			*this = std::move(src);
		}

		DimensionsDefinitions::DimensionsDefinitions(const DimensionsDefinitions& src) {
			*this = src;
		}

		DimensionsDefinitions& DimensionsDefinitions::operator=(DimensionsDefinitions&& src) {
			// Avoid self assignment
			if (&src == this) {
				return *this;
			}
			std::swap(EnsembleSize ,  src.EnsembleSize);
			std::swap(LeadTimeVar ,  src.LeadTimeVar);
			std::swap(TimeUnits ,  src.TimeUnits);
			std::swap(TimeVar ,  src.TimeVar);
			std::swap(StationIds ,  src.StationIds);
			return *this;
		}

		DimensionsDefinitions& DimensionsDefinitions::operator=(const DimensionsDefinitions& src)
		{
			if (&src == this) {
				return *this;
			}
			EnsembleSize = src.EnsembleSize;
			LeadTimeVar = src.LeadTimeVar;
			TimeUnits = src.TimeUnits;
			TimeVar = src.TimeVar;
			StationIds = src.StationIds;
			return *this;
		}

		DataGeometryProvider::~DataGeometryProvider() {}

		namespace io
		{
			SwiftNetCDFAccess::SwiftNetCDFAccess(const string& filename, bool lazyLoad)
			{
				ExceptionUtilities::CheckFileExists(filename);
				int code = nc_open(filename.c_str(), NC_NOWRITE, &ncid);
				if (NC_NOERR != code)
				{
					ThrowOnFileOpenFail(filename, code);
				}
				else
				{
					if (lazyLoad)
						ReadGeometryDimensions();
					else
						ReadGeometry();
				}
			}

			SwiftNetCDFAccess::SwiftNetCDFAccess(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, 
				const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits)
			{
				Init(filename, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, globalAttributes, leadTimeUnits);
			}

			SwiftNetCDFAccess::SwiftNetCDFAccess(const string& filename, const DimensionsDefinitions& d,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes)
			{
				Init(filename, d.EnsembleSize, d.LeadTimeVar, d.TimeUnits, d.TimeVar, d.StationIds, varDefinitions, globalAttributes, d.LeadTimeUnits);
			}

			void SwiftNetCDFAccess::Init(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
				const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits)
			{
				if (nEns == 0) ExceptionUtilities::ThrowOutOfRange("SWIFT netCDF ensemble size must be strictly positive");
				if (leadTimeVar.size() == 0) ExceptionUtilities::ThrowOutOfRange("SWIFT netCDF lead time dimension size must be strictly positive");
				if (timeVar.size() == 0) ExceptionUtilities::ThrowOutOfRange("SWIFT netCDF time dimension size must be strictly positive");

				auto fname = filename.c_str();
				int code = nc_create(fname, NC_CLASSIC_MODEL | NC_CLOBBER, &ncid);
				if (NC_NOERR != code)
				{
					ThrowOnFileOpenFail(filename, code);
				}
				else
				{
					WriteGeometry(nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefinitions, globalAttributes, leadTimeUnits);
					DefineVariables();
				}
			}

			void SwiftNetCDFAccess::ThrowOnFileOpenFail(const string& filename, int code)
			{
				throw std::logic_error(string("Failed to open file ") + filename + " with code " + boost::lexical_cast<string>(code));
			}

			void SwiftNetCDFAccess::ThrowOnVarInquiryFail(const string& varName, int code)
			{
				throw std::logic_error(string("Failed to query on variable ") + varName + " with code " + boost::lexical_cast<string>(code));
			}

			SwiftNetCDFAccess::~SwiftNetCDFAccess()
			{
				if (stationNames != nullptr) delete stationNames;
				if (variableVarNames != nullptr) delete variableVarNames;
				//if (variableVarIds != nullptr) delete[] variableVarIds;
				//if (stationIds != nullptr) delete[] stationIds;
				//if (timeVec != nullptr) delete[] timeVec;
				//if (leadTimeVec != nullptr) delete[] leadTimeVec;
				//if (stationLat != nullptr) delete[] stationLat;
				//if (stationLon != nullptr) delete[] stationLon;
				//if (stationElevation != nullptr) delete[] stationElevation;
				if (ncid != -1) nc_close(ncid);
			}

			ptime SwiftNetCDFAccess::ParseStartDate(const string& unitsAttribute)
			{
				using namespace boost::gregorian;
				namespace bt = boost::posix_time;

				auto t2string = unitsAttribute.substr(unitsAttribute.find("since") + 6, unitsAttribute.npos);

				// http://stackoverflow.com/questions/3786201/parsing-of-date-time-from-string-boost
				// http://www.boost.org/doc/libs/1_56_0/doc/html/date_time/date_time_io.html#date_time.io_tutorial
				auto format = std::locale(std::locale::classic(), new bt::time_input_facet(bt::time_input_facet::iso_time_format_extended_specifier));
				std::istringstream is(t2string);
				is.imbue(format);
				ptime startDate;
				is >> startDate;

				// Shift the start time from the timezone specified in the file to that defined in TimeOffsetIn
				int tzp = (int)t2string.find_last_of('+');
				int tzn = (int)t2string.find_last_of('-');
				int ind = (tzp > 0) ? tzp : tzn;
				int direction = (tzp > 0) ? 1 : -1;
				std::string timezone = t2string.substr(ind + 1);
				int hours = boost::lexical_cast<int>(timezone.substr(0, 2)) * direction;
				int minutes = boost::lexical_cast<int>(timezone.substr(2, 2)) * direction;
				time_duration inputOffset = time_duration(hours, minutes, 0);
				time_duration diff = inputOffset - TimeOffsetIn;
				startDate = startDate - diff;

				return startDate;
			}

			string SwiftNetCDFAccess::GetTimeStepName(const TimeStep& timeStep)
			{
				if (!timeStep.IsRegular())
				{
					if (timeStep == timeStep.GetMonthlyQpp())
						return "months";
				}

				const time_duration oneDay(24, 0, 0);
				const time_duration oneHour(1, 0, 0);
				const time_duration oneMinute(0, 1, 0);
				const time_duration oneSecond(0, 0, 1);

				auto duration = timeStep.GetRegularStepDuration();

				if (duration >= oneDay)
					return"days";
				else if (duration >= oneHour)
					return "hours";
				else if (duration >= oneMinute)
					return "minutes";
				else if (duration >= oneSecond)
					return "seconds";
				else // (duration < oneSecond)
					ExceptionUtilities::ThrowNotSupported("SwiftNetCDFAccess::GetTimeStepName: sub-second time step not yet supported");
			}

			string SwiftNetCDFAccess::CreateTimeUnitsAttribute(const ptime& utcStart, const TimeStep& timeStep)
			{
				string timeUnit = GetTimeStepName(timeStep);
				return CreateTimeUnitsAttribute(utcStart, timeUnit);
			}

			string SwiftNetCDFAccess::CreateLeadTimeUnitsAttribute(const TimeStep& timeStep)
			{
				string timeUnit = GetTimeStepName(timeStep);
				return timeUnit + " since time";
			}

			time_duration SwiftNetCDFAccess::CreateTimeUnits(const TimeStep& timeStep)
			{
				if (!timeStep.IsRegular())
					return time_duration();//Irregular time steps will report a zero length duration

				const time_duration oneDay(24, 0, 0);
				const time_duration oneHour(1, 0, 0);
				const time_duration oneMinute(0, 1, 0);
				const time_duration oneSecond(0, 0, 1);
				string timeUnit;

				auto duration = timeStep.GetRegularStepDuration();

				if (duration >= oneDay)
					return oneDay;
				else if (duration >= oneHour)
					return oneHour;
				else if (duration >= oneMinute)
					return oneMinute;
				else
					return oneSecond;
			}

			vector<double> SwiftNetCDFAccess::CreateTimeVector(const ptime& start, const TimeStep& timeStep, const ptime& origin, const time_duration& timeStepAxis, const size_t length)
			{
				if (!timeStep.IsRegular())
				{
					if (timeStepAxis.total_seconds() == 0)
					{
						double by = 1.0;//Irregular time steps cannot work with multiples of themselves like regular time steps can, so this is always 1
						double offset = timeStep.GetOffset(origin, start);
						return datatypes::utils::SeqVec<double>(offset, by, length);
					}
					else
					{
						ExceptionUtilities::ThrowInvalidArgument("Cannot specify a non zero timeStepAxis duration when using an irregular main time step");
					}
				}

				auto duration = timeStep.GetRegularStepDuration();
				auto durationUnit = timeStepAxis;
				double axisUnitsSeconds = timeStepAxis.total_seconds();
				if (axisUnitsSeconds <= 0) ExceptionUtilities::ThrowInvalidArgument("Axis units must be a strictly positive duration, at least a second");
				double by = (double)duration.total_seconds() / axisUnitsSeconds;
				double offsetSeconds = (start - origin).total_seconds();
				double offset = offsetSeconds / axisUnitsSeconds;
				return datatypes::utils::SeqVec<double>(offset, by, length);
			}

			vector<double> SwiftNetCDFAccess::CreateTimeVector(const ptime& start, const TimeStep& timeStep, const ptime& origin, const TimeStep& timeStepAxis, const size_t length)
			{
				auto durationUnit = CreateTimeUnits(timeStepAxis);
				return CreateTimeVector(start, timeStep, origin, durationUnit, length);
			}

			vector<double> SwiftNetCDFAccess::CreateTimeVector(const ptime& start, const TimeStep& timeStep, const size_t length)
			{
				auto durationUnit = CreateTimeUnits(timeStep);
				return CreateTimeVector(start, timeStep, start, durationUnit, length);
			}

			std::pair<vector<double>, vector<double>> SwiftNetCDFAccess::CreateTimeVectors(const ptime& start, const TimeStep& timeStep, size_t tsLength, const TimeStep& leadTimeStep, size_t leadTimeSize, int fcastOffset)
			{
				auto durationUnit = CreateTimeUnits(timeStep);
				auto mainTv = CreateTimeVector(start, timeStep, start, durationUnit, tsLength);
				ptime fcastTime = leadTimeStep.AddSteps(start, +fcastOffset);
				auto leadTv = CreateTimeVector(fcastTime, leadTimeStep, start, durationUnit, leadTimeSize);
				return std::pair<vector<double>, vector<double>>(mainTv, leadTv);
			}

			string SwiftNetCDFAccess::CreateTimeUnitsAttribute(const ptime& utcStart, const string& units)
			{
				string timeUnit(units);

				// Shift the start time from TimeOffsetIn to TimeOffsetOut
				ptime start = ptime(utcStart);
				time_duration diff = TimeOffsetIn - TimeOffsetOut;
				start = start - diff;
				char sign = (TimeOffsetOut >= time_duration()) ? '+' : '-';
				int direction = (TimeOffsetOut >= time_duration()) ? 1 : -1;
				int hours = TimeOffsetOut.hours() * direction;
				int minutes = TimeOffsetOut.minutes() * direction;
				std::ostringstream ss;
				ss << std::setw(2) << std::setfill('0') << hours << std::setw(2) << std::setfill('0') << minutes;
				std::string timezone = sign + ss.str();
				ss.clear();

				// Output date time in NetCDF spec format e.g. 2000-12-31 09:00:00.0 +1000
				using namespace boost::gregorian;
				namespace bt = boost::posix_time;

				auto format = std::locale(std::locale::classic(), new bt::time_facet("%Y-%m-%d %H:%M:%S"));
				std::ostringstream oss;
				oss.imbue(format);
				oss << start;
				std::string startStr = oss.str();
				oss.clear();

				return timeUnit + " since " + startStr + ".0 " + timezone;
			}

			void SwiftNetCDFAccess::ReadGeometryDimensions()
			{
				InquireDimIds();
				InquireDimVarIds();
				ReadTimeUnits();

				numTimeSteps = InquireDimLength(timeDimId);
				numStations = InquireDimLength(stationDimId);
				strLen = InquireDimLength(strLenDimId);
				leadTimeLength = InquireDimLength(leadTimeDimId);
				ensembleSize = InquireDimLength(ensMemberDimId);
				geometryRead = true;
			}

			void SwiftNetCDFAccess::ReadGeometryVariables()
			{
				stationNames = GetStringVariable(strLen, stationNameVarId, numStations);

				stationIds = GetVariableDimOne<int>(stationIdVarId, numStations);
				if (latVarId >= 0)
					stationLat = GetVariableDimOne<double>(latVarId, numStations);
				if (lonVarId >= 0)
					stationLon = GetVariableDimOne<double>(lonVarId, numStations);
				if (elevationVarId >= 0)
					stationElevation = GetVariableDimOne<double>(elevationVarId, numStations);

				catchmentName = ReadStringAttribute(NC_GLOBAL, kCatchmentAttName, false, "<unspecified>");//v1

				// time dimensions used to be specified as integers in SWIFT v1 specs (TBC). 
				// Also, some files including in the unit test files have NC_DOUBLE precision for time dimensions. 
				// For backward compat, given that the precision is "unlikely" to be an issue in time dims:
				timeVec = ReadAsFloat(timeVarId, numTimeSteps, false);
				leadTimeVec = ReadAsFloat(leadTimeVarId, leadTimeLength, false);
			}

			void SwiftNetCDFAccess::ReadGeometry()
			{
				ReadGeometryDimensions();
				ReadGeometryVariables();
			}

			void SwiftNetCDFAccess::SetTimeVar(const vector<double>& timeVar)
			{
				// TODO: check whether we need to do something special for unlimited dimension
				this->numTimeSteps = timeVar.size();
				this->timeVec = Convert<double, float>(timeVar);
				SetVariableDimOne<float>(timeVarId, timeVec);
			}

			void SwiftNetCDFAccess::SetLeadTimeVar(const vector<double>& leadTimeVar)
			{
				if (this->leadTimeLength != leadTimeVar.size())
					ExceptionUtilities::ThrowInvalidArgument("Unexpected size for lead time vector: expected " + std::to_string(leadTimeLength) + ", got " + std::to_string(leadTimeVar.size()));
				this->leadTimeVec = Convert<double, float>(leadTimeVar);
				SetVariableDimOne<float>(leadTimeVarId, leadTimeVec);
			}

			void SwiftNetCDFAccess::WriteGeometry(size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds, const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& leadTimeUnits)
			{
				using namespace std;
				DefineMandatoryDimensions(nEns, leadTimeVar.size(), stationIds.size());
				DefineDimVariables();
				FillAttributes(globalAttributes, timeUnits, leadTimeUnits);

				this->variableDefinitions = varDefinitions;

				ncendef(ncid);
				SetTimeVar(timeVar);
				SetLeadTimeVar(leadTimeVar);

				numTimeSteps = timeVar.size();
				leadTimeLength = leadTimeVar.size();
				numStations = stationIds.size();
				// strLen; keep default value for the time being.

				stationNames = new vector<string>(stationIds); // KLUDGE: needs a change to the interface.

				this->stationIds = Convert<int>(stationIds);
				// stationLat = 
				// stationLon = 
				// stationElevation = 

				catchmentName = "<unspecified>";

				this->timeUnits = ParseTimeUnits(string(timeUnits));

				WriteCommonVarData();
			}

			int SwiftNetCDFAccess::AddGlobalAttribute(const string& attName, const string& attValue)
			{
				return AddAttribute(NC_GLOBAL, attName, attValue);
			}

			int SwiftNetCDFAccess::AddGlobalAttribute(const string& attName, double attValue)
			{
				return AddAttribute(NC_GLOBAL, attName, attValue);
			}

			int SwiftNetCDFAccess::AddAttribute(int varId, const string& attName, const string& attValue)
			{
				return nc_put_att(ncid, varId, attName.c_str(), NC_CHAR, attValue.size() + 1, attValue.c_str());
			}

			int SwiftNetCDFAccess::AddAttribute(int varId, const string& attName, int attValue)
			{
				return nc_put_att_int(ncid, varId, attName.c_str(), NC_INT, (size_t)1, &attValue);
			}

			int SwiftNetCDFAccess::AddAttribute(int varId, const string& attName, double attValue)
			{
				return nc_put_att_double(ncid, varId, attName.c_str(), NC_DOUBLE, (size_t)1, &attValue);
			}

			int SwiftNetCDFAccess::AddGlobalAttribute(char * attName, char * attValue)
			{
				return AddGlobalAttribute(string(attName), string(attValue));
			}

			void SwiftNetCDFAccess::FillAttributes(const GlobalAttributes& globalAttributes, const string& timeUnits, const string& leadTimeUnits)
			{
				int code;
				code = AddGlobalAttribute(kGlobalAttNameTitle, globalAttributes.Title);
				code = AddGlobalAttribute(kGlobalAttNameInstitution, globalAttributes.Institution);
				code = AddGlobalAttribute(kGlobalAttNameSource, globalAttributes.Source);
				code = AddGlobalAttribute(kGlobalAttNameCatchment, globalAttributes.Catchment);
				code = AddGlobalAttribute(kGlobalAttNameSTF_convention_version, globalAttributes.STFConventionVersion);
				code = AddGlobalAttribute(kGlobalAttNameSTF_nc_spec, globalAttributes.STFNCSpec);
				code = AddGlobalAttribute(kGlobalAttNameComment, globalAttributes.Comment);
				code = AddGlobalAttribute(kGlobalAttNameHistory, globalAttributes.History);

				code = AddAttribute(timeVarId, string(kStandardNameAttName), string(kTimeVarName));
				code = AddAttribute(timeVarId, string(kLongNameAttName), string(kTimeVarName));

				code = AddAttribute(timeVarId, string(kUnitsAttName), string(timeUnits));
				code = AddAttribute(timeVarId, string(kAttNameTimeStandard), string("UTC"));
				code = AddAttribute(timeVarId, string(kAxisAttName), string("t"));
				
				code = AddAttribute(stationIdVarId, string(kLongNameAttName), string("station or node identification code"));
				code = AddAttribute(stationNameVarId, string(kLongNameAttName), string("station or node name"));

				code = AddAttribute(ensMemberVarId, string(kLongNameAttName), string("ensemble member"));
				code = AddAttribute(ensMemberVarId, string(kStandardNameAttName), string("ens_member"));
				code = AddAttribute(ensMemberVarId, string(kUnitsAttName), string("member id"));
				code = AddAttribute(ensMemberVarId, string(kAxisAttName), string("u"));

				code = AddAttribute(leadTimeVarId, string(kLongNameAttName), string("forecast lead time"));
				code = AddAttribute(leadTimeVarId, string(kStandardNameAttName), string("lead time"));
				code = AddAttribute(leadTimeVarId, string(kUnitsAttName), string(leadTimeUnits));
				code = AddAttribute(leadTimeVarId, string(kAxisAttName), string("v"));

				code = AddAttribute(latVarId, string(kLongNameAttName), string("latitude"));
				code = AddAttribute(latVarId, string(kUnitsAttName), string("degrees_north"));
				code = AddAttribute(latVarId, string(kAxisAttName), string("y"));

				code = AddAttribute(lonVarId, string(kLongNameAttName), string("longitude"));
				code = AddAttribute(lonVarId, string(kUnitsAttName), string("degrees_east"));
				code = AddAttribute(lonVarId, string(kAxisAttName), string("x"));

				code = AddAttribute(elevationVarId, string(kStandardNameAttName), string("elevation"));
				code = AddAttribute(elevationVarId, string(kLongNameAttName), string("station elevation above sea level"));
				code = AddAttribute(elevationVarId, string(kUnitsAttName), string("m"));
				
				this->startDate = ParseStartDate(string(timeUnits));

			}

			void SwiftNetCDFAccess::InquireDimIds()
			{
				int code;
				code = nc_inq_dimid(ncid, kTimeDimName.c_str(), &timeDimId);
				code = nc_inq_dimid(ncid, kStationDimName.c_str(), &stationDimId);
				code = nc_inq_dimid(ncid, kStrLenDimName.c_str(), &strLenDimId);
				code = nc_inq_dimid(ncid, kLeadTimeDimName.c_str(), &leadTimeDimId);
				code = nc_inq_dimid(ncid, kEnsMemberDimName.c_str(), &ensMemberDimId);

			}

			void SwiftNetCDFAccess::InquireDimVarIds()
			{
				int code;
				code = nc_inq_varid(ncid, kTimeVarName.c_str(), &timeVarId);
				code = nc_inq_varid(ncid, kStationNameVarName.c_str(), &stationNameVarId);
				code = nc_inq_varid(ncid, kStationIdVarName.c_str(), &stationIdVarId);
				code = nc_inq_varid(ncid, kLeadTimeVarName.c_str(), &leadTimeVarId);
				code = nc_inq_varid(ncid, kEnsMemberVarName.c_str(), &ensMemberVarId);
				code = nc_inq_varid(ncid, kLatVarName.c_str(), &latVarId);
				code = nc_inq_varid(ncid, kLonVarName.c_str(), &lonVarId);
				code = nc_inq_varid(ncid, kElevationVarName.c_str(), &elevationVarId);
			}

			void SwiftNetCDFAccess::ReadTimeUnits()
			{
				size_t size;
				// TODO: consider using udunit2, however this is not up to scratch for windows (32 bits only). See https://github.com/Unidata/UDUNITS-2/issues/16.

				int code = nc_inq_attlen(ncid, timeVarId, kUnitsAttName.c_str(), &size);
				char * tempchars;
				tempchars = new char[size + 1];
				code = nc_get_att_text(ncid, timeVarId, kUnitsAttName.c_str(), tempchars);

				string units(tempchars);
				delete[] tempchars;

				ptime parsedDate = ParseStartDate(units);
				startDate = parsedDate;

				timeUnits = ParseTimeUnits(units);
			}

			string SwiftNetCDFAccess::ParseTimeUnits(const string& unitsAttribute)
			{
				auto t2string = unitsAttribute.substr(unitsAttribute.find("since") + 6, unitsAttribute.npos);

				t2string = unitsAttribute.substr(0, 4);
				string timeUnits;
				// TODO: suggest to use udunits markers for time units.
				// This looks too ad-hoc. Propose clearer scheme. Why would month be a default??
				if (t2string == "hour")
				{
					timeUnits = SWIFT_TIME_UNIT_HOUR;
				}
				else if (t2string == string("days"))
				{
					timeUnits = SWIFT_TIME_UNIT_DAY;
				}
				else if (t2string == string("minu"))
				{
					timeUnits = SWIFT_TIME_UNIT_MINUTE;
				}
				else if (t2string == string("mont"))
				{
					timeUnits = SWIFT_TIME_UNIT_MONTH;
				}
				else
				{
					ExceptionUtilities::ThrowInvalidArgument("SwiftNetCDFAccess::ParseTimeUnits: Unrecognized unit: " + unitsAttribute);
					// legacy that should disappear.
					//timeUnits = string("mo");
				}
				return timeUnits;
			}


			size_t SwiftNetCDFAccess::InquireDimLength(int dimId)
			{
				size_t size;
				int code = nc_inq_dimlen(ncid, dimId, &size);

				return (int)size;
			}

			vector<string> SwiftNetCDFAccess::ReservedVariableNames() const
			{
				return vector<string>({
				kTimeVarName          ,
				kLatVarName           ,
				kLonVarName           ,
				kElevationVarName     ,
				kStationNameVarName   ,
				kStationIdVarName     ,
				kLeadTimeVarName      ,
				kEnsMemberVarName,
				kStrLenVarName ,
				kStationVarName
				});
			}

			vector<string> SwiftNetCDFAccess::ReadVariableNames(bool removeDimVars)
			{
				vector<string> variableNames = vector<string>();

				int nVars;
				nc_inq_nvars(ncid, &nVars);
				
				int* varIds = new int[nVars];
				nc_inq_varids(ncid, &nVars, varIds);

				char* varName = new char[kDefaultStrLength];
				for (int i = 0; i < nVars; i++)
				{
					nc_inq_varname(ncid, varIds[i], varName);
					variableNames.push_back(string(varName));
				}
				delete[] varName;

				delete[] varIds;

				if (removeDimVars)
					variableNames = datatypes::utils::StringProcessing::SetDiff(variableNames, ReservedVariableNames());
				return variableNames;
			}

			vector<string> SwiftNetCDFAccess::ReadAttributeNames(const string& varName)
			{
				int varId = GetVarId(varName);

				int nAtts;
				nc_inq_natts(ncid, &nAtts);

				vector<string> attNames = vector<string>();

				char* attName = new char[kDefaultStrLength];
				for (int i = 0; i < nAtts; i++)
				{
					int code = nc_inq_attname(ncid, varId, i, attName);
					if (code == 0)
						attNames.push_back(string(attName));
				}
				delete[] attName;

				return attNames;
			}

			string SwiftNetCDFAccess::ReadStringAttribute(const string& varName, const string& attName, bool throwIfNotFound, string defaultValue)
			{
				int varId = GetVarId(varName);
				return ReadStringAttribute(varId, attName, throwIfNotFound, defaultValue);
			}

			string SwiftNetCDFAccess::ReadStringAttribute(int varId, const string& attName, bool throwIfNotFound, string defaultValue)
			{
				nc_type type;
				int code = nc_inq_atttype(ncid, varId, attName.c_str(), &type);

				if (code == NC_ENOTATT)
				{
					if (throwIfNotFound)
						ExceptionUtilities::ThrowInvalidArgument("Attribute attName does not exist.");
					else
						return defaultValue;
				}

				if (type != NC_CHAR)
					ExceptionUtilities::ThrowInvalidArgument("Attribute attName not of type string.");

				size_t size;
				code = nc_inq_attlen(ncid, varId, attName.c_str(), &size);

				char* strAtt = new char[size];
				code = nc_get_att_text(ncid, varId, attName.c_str(), strAtt);
				string result = (strAtt[size - 1] == '\0') ? string(strAtt, size - 1) : string(strAtt, size);

				delete[] strAtt;

				return result;
			}

			double SwiftNetCDFAccess::ReadNumericAttribute(const string& varName, const string& attName, bool throwIfNotFound, double defaultValue)
			{
				int varId = GetVarId(varName);
				return ReadNumericAttribute(varId, attName, throwIfNotFound, defaultValue);
			}

			double SwiftNetCDFAccess::ReadNumericAttribute(int varId, const string& attName, bool throwIfNotFound, double defaultValue)
			{
				nc_type type;
				int code = nc_inq_atttype(ncid, varId, attName.c_str(), &type);

				if (code == NC_ENOTATT)
				{
					if (throwIfNotFound)
						ExceptionUtilities::ThrowInvalidArgument("Attribute attName does not exist.");
					else
						return defaultValue;
				}

				if (type != NC_DOUBLE && type != NC_FLOAT && type != NC_INT && type != NC_INT64)
					ExceptionUtilities::ThrowInvalidArgument("Attribute attName not of numeric type (double, float, int or int64).");

				if (type == NC_DOUBLE)
				{
					double d;
					code = nc_get_att_double(ncid, varId, attName.c_str(), &d);
					return d;
				}
				if (type == NC_FLOAT)
				{
					float f;
					code = nc_get_att_float(ncid, varId, attName.c_str(), &f);
					return (double)f;
				}
				if (type == NC_INT)
				{
					int i;
					code = nc_get_att_int(ncid, varId, attName.c_str(), &i);
					return (double)i;
				}
				if (type == NC_INT64)
				{
					long l;
					code = nc_get_att_long(ncid, varId, attName.c_str(), &l);
					return (double)l;
				}
			}

			nc_type SwiftNetCDFAccess::GetDataType(int variableId)
			{
				nc_type datatype;
				int code = nc_inq_vartype(ncid, variableId, &datatype);
				return datatype;
			}

			vector<string> * SwiftNetCDFAccess::GetStringVariable(size_t strLen, int varId, size_t n)
			{
				char * bfr = new char[strLen];
				auto strVec = new vector<string>();
				size_t starts[2] = { 0, 0 };
				const size_t counts[2] = { 1, (size_t)strLen };
				for (int i = 0; i < n; i++)
				{
					starts[0] = i;
					int code = nc_get_vara(ncid, varId, starts, counts, bfr);
					strVec->push_back(string(bfr));
				}
				delete bfr;
				return strVec;
			}

			void SwiftNetCDFAccess::ErrorLossPrecision(int varId)
			{
				ExceptionUtilities::ThrowNotSupported("Read operation '" + std::to_string(varId) + "' with loss of precision not allowed");
			}

			vector<int> SwiftNetCDFAccess::ReadAsInt(int varId, size_t size)
			{
				nc_type datatype = GetDataType(varId);
				if (datatype == NC_DOUBLE)
				{
					ErrorLossPrecision(varId);
					//return GetVariableDimOne<int, double>(varId, size);
				}
				else if (datatype == NC_FLOAT)
				{
					ErrorLossPrecision(varId);
					//return GetVariableDimOne<int, float>(varId, size);
				}
				else if (datatype == NC_INT)
				{
					return GetVariableDimOne<int>(varId, size);
				}
				else
					ExceptionUtilities::ThrowInvalidOperation(string("The netCDF data type code ") + std::to_string(datatype) + " of the variable " + std::to_string(varId) + " is not supported ");
			}

			vector<float> SwiftNetCDFAccess::ReadAsFloat(int varId, size_t size, bool strict)
			{
				nc_type datatype = GetDataType(varId);
				if (datatype == NC_DOUBLE)
				{
					if(strict) // need to be optionally "lax" for some cases, as some SWIFT netCDF files around have double not float for time dims
						ErrorLossPrecision(varId);
					else
						return GetVariableDimOne<float, double>(varId, size);
				}
				else if (datatype == NC_FLOAT)
				{
					return GetVariableDimOne<float>(varId, size);
				}
				else if (datatype == NC_INT)
				{
					return GetVariableDimOne<float, int>(varId, size);
				}
				else
					ExceptionUtilities::ThrowInvalidOperation(string("The netCDF data type code ") + std::to_string(datatype) + " of the variable " + std::to_string(varId) + " is not supported ");
			}

			vector<double> SwiftNetCDFAccess::ReadAsDouble(int varId, size_t size)
			{
				nc_type datatype = GetDataType(varId);
				if (datatype == NC_DOUBLE)
				{
					return GetVariableDimOne<double>(varId, size);
				}
				else if (datatype == NC_FLOAT)
				{
					return GetVariableDimOne<double, float>(varId, size);
				}
				else if (datatype == NC_INT)
				{
					return GetVariableDimOne<double, int>(varId, size);
				}
				else
					ExceptionUtilities::ThrowInvalidOperation(string("The netCDF data type code ") + std::to_string(datatype) + " of the variable " + std::to_string(varId) + " is not supported ");
			}

			vector<int> SwiftNetCDFAccess::GetVarDims(const string& varName)
			{
				int ndimsp;
				int varid = GetVarId(varName);
				int code = nc_inq_varndims(ncid, varid, &ndimsp);
				if (code != NC_NOERR)
					ExceptionUtilities::ThrowInvalidOperation("SwiftNetCDFAccess::GetVarDims nc_inq_varndims failed for variable " + varName);

				vector<int> vardims(ndimsp);
				code = nc_inq_vardimid(ncid, varid, &(vardims[0]));
				if (code != NC_NOERR)
					ExceptionUtilities::ThrowInvalidOperation("SwiftNetCDFAccess::GetVarDims nc_inq_vardimid failed for variable " + varName);
				return vardims;
			}

			vector<int> SwiftNetCDFAccess::GetVarDims(int varNumDims)
			{
				vector<int> result;
				if (varNumDims == 2)
					result = { timeDimId, stationDimId };
				else if (varNumDims == 3)
					result = { timeDimId, ensMemberDimId, stationDimId };
				else if (varNumDims == 4)
					result = { timeDimId, ensMemberDimId, stationDimId, leadTimeDimId };
				else
					ExceptionUtilities::ThrowInvalidArgument("number of variable dimensions not supported: " + std::to_string(varNumDims));
				return result;
			}

			void SwiftNetCDFAccess::DefineVariable(const VariableDefinition& d, int& code)
			{
				string varName = d.Name;
				int varNumDims = boost::lexical_cast<int>(d.Dimensions);
				int precision = d.GetPrecision();
				vector<int> vardimids = GetVarDims(varNumDims);
				int variableVarId;
				code = nc_def_var(ncid, varName.c_str(), precision, varNumDims, &(vardimids[0]), &variableVarId);
				if (NC_NOERR == code)
					AddVariableAttributes(variableVarId, d.attributes);
			}

			void SwiftNetCDFAccess::DefineVariables()
			{
				ncredef(ncid);

				int code;
				for (const auto& x : variableDefinitions)
				{
					DefineVariable(x.second, code);
					if (NC_NOERR != code)
					{
						ncendef(ncid);
						ExceptionUtilities::ThrowInvalidOperation("Failed to create netCDF variable: " + x.second.Name);
					}
				}
				ncendef(ncid);
			}

			void SwiftNetCDFAccess::AddVariableAttributes(int varId, const VariableAttributes& varAttributes)
			{
				AddVariableAttributes(varId, varAttributes.LongName, varAttributes.Units, varAttributes.Type, varAttributes.TypeDescription, varAttributes.DatType, varAttributes.DatDescription, varAttributes.FillValue, varAttributes.LocationType);
			}

			void SwiftNetCDFAccess::AddVariableAttributes(int varId, const string& longName, const string& units, int type, const string& typeDescription, const string& datType, const string& datDescription, double fillValue, const string& locationType)
			{
				int code = AddAttribute(varId, string(kFillValueAttName), fillValue);
				code = AddAttribute(varId, string(kLongNameAttName), longName);
				code = AddAttribute(varId, string(kUnitsAttName), units);
				code = AddAttribute(varId, string(kTypeAttName), type);
				code = AddAttribute(varId, string(kTypeDescriptionAttName), typeDescription);
				code = AddAttribute(varId, string(kDatTypeAttName), datType);
				code = AddAttribute(varId, string(kDatDescriptionAttName), datDescription);
				code = AddAttribute(varId, string(kLocationTypeAttName), locationType);
			}

			void SwiftNetCDFAccess::AddAttributes(int varId, const string& varName, std::map<string, VariableAttributes>& varAttributes)
			{
				if (varAttributes.find(varName) == varAttributes.end())
				{
					AddVariableAttributes(varId, varName, string("unknown"), 0, string("unknown"), string("unknown"), string("unknown"), VariableAttributes::DefaultFillValue(), string("unknown"));
				}
				else
				{
					auto att = varAttributes.at(varName);
					AddVariableAttributes(varId, att.LongName, att.Units, att.Type, att.TypeDescription, att.DatType, att.DatDescription, att.FillValue, att.LocationType);
				}
			}

			VariableAttributes SwiftNetCDFAccess::ReadAttributes(const string& varName)
			{
				VariableAttributes varAttrib = VariableAttributes();
				varAttrib.FillValue = ReadNumericAttribute(varName, kFillValueAttName);
				varAttrib.LongName = ReadStringAttribute(varName, kLongNameAttName);
				varAttrib.Units = ReadStringAttribute(varName, kUnitsAttName);
				varAttrib.Type = (int)ReadNumericAttribute(varName, kTypeAttName);
				varAttrib.TypeDescription = ReadStringAttribute(varName, kTypeDescriptionAttName);
				varAttrib.DatType = ReadStringAttribute(varName, kDatTypeAttName);
				varAttrib.DatDescription = ReadStringAttribute(varName, kDatDescriptionAttName);
				varAttrib.LocationType = ReadStringAttribute(varName, kLocationTypeAttName);
				return varAttrib;
			}

			VariableAttributes SwiftNetCDFAccess::GetAttributes(const string& varName)
			{
				auto iter = variableDefinitions.find(varName);
				if (iter != variableDefinitions.end())
					return variableDefinitions[varName].attributes;
				return ReadAttributes(varName);
			}

			GlobalAttributes SwiftNetCDFAccess::GetGlobalAttributes()
			{
				GlobalAttributes globAttrib = GlobalAttributes();
				globAttrib.Title = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameTitle));
				globAttrib.Institution = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameInstitution));
				globAttrib.Source = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameSource));
				globAttrib.Catchment = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameCatchment));
				globAttrib.STFConventionVersion = ReadNumericAttribute(NC_GLOBAL, string(kGlobalAttNameSTF_convention_version));
				globAttrib.STFNCSpec = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameSTF_nc_spec));
				globAttrib.Comment = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameComment));
				globAttrib.History = ReadStringAttribute(NC_GLOBAL, string(kGlobalAttNameHistory));
				return globAttrib;
			}

			const string SwiftNetCDFAccess::kTimeDimName = "time";
			const string SwiftNetCDFAccess::kStationDimName = "station";
			const string SwiftNetCDFAccess::kStrLenDimName = "strLen";
			const string SwiftNetCDFAccess::kLeadTimeDimName = "lead_time";
			const string SwiftNetCDFAccess::kEnsMemberDimName = "ens_member";

			const string SwiftNetCDFAccess::kTimeVarName = "time";
			const string SwiftNetCDFAccess::kLatVarName = "lat";
			const string SwiftNetCDFAccess::kLonVarName = "lon";
			const string SwiftNetCDFAccess::kElevationVarName = "elevation";
			const string SwiftNetCDFAccess::kStationNameVarName = "station_name";
			const string SwiftNetCDFAccess::kStationIdVarName = "station_id";
			const string SwiftNetCDFAccess::kLeadTimeVarName = "lead_time";
			const string SwiftNetCDFAccess::kEnsMemberVarName = "ens_member";
			const string SwiftNetCDFAccess::kStrLenVarName = "strLen";
			const string SwiftNetCDFAccess::kStationVarName = "station";

			const string SwiftNetCDFAccess::kUnitsAttName = "units";
			const string SwiftNetCDFAccess::kStandardNameAttName = "standard_name";
			const string SwiftNetCDFAccess::kLongNameAttName = "long_name";
			const string SwiftNetCDFAccess::kAxisAttName = "axis";
			const string SwiftNetCDFAccess::kTypeAttName = "type";
			const string SwiftNetCDFAccess::kTypeDescriptionAttName = "type_description";
			const string SwiftNetCDFAccess::kDatTypeAttName = "dat_type";
			const string SwiftNetCDFAccess::kDatDescriptionAttName = "dat_type_description";
			const string SwiftNetCDFAccess::kLocationTypeAttName = "location_type";

			const string SwiftNetCDFAccess::kCatchmentAttName = "Catchment";
			const string SwiftNetCDFAccess::kFillValueAttName = "_FillValue";

			const string SwiftNetCDFAccess::kGlobalAttNameTitle = "title";
			const string SwiftNetCDFAccess::kGlobalAttNameInstitution = "institution";
			const string SwiftNetCDFAccess::kGlobalAttNameSource = "source";
			const string SwiftNetCDFAccess::kGlobalAttNameCatchment = "catchment";
			const string SwiftNetCDFAccess::kGlobalAttNameSTF_convention_version = "STF_convention_version";
			const string SwiftNetCDFAccess::kGlobalAttNameSTF_nc_spec = "STF_nc_spec";
			const string SwiftNetCDFAccess::kGlobalAttNameComment = "comment";
			const string SwiftNetCDFAccess::kGlobalAttNameHistory = "history";
			const string SwiftNetCDFAccess::kAttNameTimeStandard = "time_standard";

			void SwiftNetCDFAccess::CheckCompliance(const string& filename, int majorVersion, int minorVersion, vector<string>& warnings, vector<string>& errors)
			{

				// Added to address https://jira.csiro.au/browse/WIRADA-87
				int ncid = -1;
				int timeDimId, stationDimId, leadTimeDimId, ensMemberDimId, strLenDimId;
				int timeVarId = -1, stationNameVarId = -1, stationIdVarId = -1, leadTimeVarId = -1, ensMemberVarId = -1, latVarId = -1, lonVarId = -1, elevationVarId = -1;

				auto checkDimIdInq = [&errors](int ncid, const string& dimName, int* idp) {
					int code = nc_inq_dimid(ncid, dimName.c_str(), idp);
					if (NC_NOERR != code)
						errors.push_back("Failed to find required dimension " + dimName);
					return code;
				};

				auto checkVarIdInq = [&errors](int ncid, const string& varName, int* idp) {
					int code = nc_inq_varid(ncid, varName.c_str(), idp);
					if (NC_NOERR != code)
						errors.push_back("Failed to find required variable " + varName);
					return code;
				};

				auto checkVarType = [&errors](int ncid, int  varid, const string& varName, nc_type expectedType)
				{
					char* name = nullptr;
					nc_type xtype;
					int ndims;
					int dimids[NC_MAX_VAR_DIMS];
					int natts;

					int code = nc_inq_var(ncid, varid,
						name,
						&xtype,
						&ndims,
						dimids,
						&natts
						);
					if (NC_NOERR != code)
						errors.push_back("Failed to inquire variable " + varName);
					if (xtype != expectedType)
						errors.push_back("Variable " + varName + " is of nc_type " + std::to_string(xtype) + "; expected " + std::to_string(expectedType));
					return code;
				};

				auto checkHasAttribute = [&errors](int ncid, const string& attName) 
				{
					size_t size;
					int code = nc_inq_attlen(ncid, NC_GLOBAL, attName.c_str(), &size);
					if (NC_NOERR != code)
						errors.push_back("Missing global attribute: " + attName);
					return code;
				};

				ExceptionUtilities::CheckFileExists(filename);
				try {
					int code = nc_open(filename.c_str(), NC_NOWRITE, &ncid);
					if (NC_NOERR != code)
					{
						ThrowOnFileOpenFail(filename, code);
					}
					else
					{
						code = checkDimIdInq(ncid, kTimeDimName, &timeDimId);
						code = checkDimIdInq(ncid, kStationDimName, &stationDimId);
						code = checkDimIdInq(ncid, kStrLenDimName, &strLenDimId);
						code = checkDimIdInq(ncid, kLeadTimeDimName, &leadTimeDimId);
						code = checkDimIdInq(ncid, kEnsMemberDimName, &ensMemberDimId);

						code = checkVarIdInq(ncid, kTimeVarName, &timeVarId);
						code = checkVarIdInq(ncid, kStationIdVarName, &stationIdVarId);
						code = checkVarIdInq(ncid, kLeadTimeVarName, &leadTimeVarId);
						code = checkVarIdInq(ncid, kEnsMemberVarName, &ensMemberVarId);
						code = checkVarIdInq(ncid, kStationNameVarName, &stationNameVarId);
						code = checkVarIdInq(ncid, kLatVarName, &latVarId);
						code = checkVarIdInq(ncid, kLonVarName, &lonVarId);
						code = checkVarIdInq(ncid, kElevationVarName, &elevationVarId);

						code = checkVarType(ncid, timeVarId, kTimeVarName, NC_FLOAT);
						code = checkVarType(ncid, stationIdVarId, kStationIdVarName, NC_INT);
						code = checkVarType(ncid, leadTimeVarId, kLeadTimeVarName, NC_FLOAT);
						code = checkVarType(ncid, ensMemberVarId, kEnsMemberVarName, NC_INT);
						code = checkVarType(ncid, stationNameVarId, kStationNameVarName, NC_CHAR);
						code = checkVarType(ncid, latVarId, kLatVarName, NC_FLOAT);
						code = checkVarType(ncid, lonVarId, kLonVarName, NC_FLOAT);
						code = checkVarType(ncid, elevationVarId, kElevationVarName, NC_FLOAT);

						// Global attributes
						code = checkHasAttribute(ncid, kGlobalAttNameTitle);
						code = checkHasAttribute(ncid, kGlobalAttNameInstitution);
						code = checkHasAttribute(ncid, kGlobalAttNameSource);
						code = checkHasAttribute(ncid, kGlobalAttNameCatchment);
						code = checkHasAttribute(ncid, kGlobalAttNameSTF_convention_version);
						code = checkHasAttribute(ncid, kGlobalAttNameSTF_nc_spec);
						code = checkHasAttribute(ncid, kGlobalAttNameComment);
						code = checkHasAttribute(ncid, kGlobalAttNameHistory);

						// variable dimensions

						// variable names

						nc_close(ncid);
					}
				}
				catch(...)
				{
					if (ncid > 0)
						nc_close(ncid);
					throw;
				}
			}

			void SwiftNetCDFAccess::DefineMandatoryDimensions(size_t nEns, size_t nLead, size_t nStations)
			{
				int code = nc_def_dim(ncid, kTimeDimName.c_str(), NC_UNLIMITED, &timeDimId);
				code = nc_def_dim(ncid, kStationDimName.c_str(), nStations, &stationDimId);
				code = nc_def_dim(ncid, kStrLenDimName.c_str(), kDefaultStrLength, &strLenDimId);
				code = nc_def_dim(ncid, kLeadTimeDimName.c_str(), nLead, &leadTimeDimId);
				code = nc_def_dim(ncid, kEnsMemberDimName.c_str(), nEns, &ensMemberDimId);

				ensembleSize = nEns;
				leadTimeLength = nLead;
				numStations = nStations;
			}

			void SwiftNetCDFAccess::DefineDimVariables()
			{
				// https://confluence.csiro.au/display/wirada/netCDF+for+Water+Forecasting+Specification+v2.0
				int timedimids[1] = { timeDimId };
				int stationnamedimids[2] = { stationDimId, strLenDimId };
				int stationiddimids[1] = { stationDimId };
				int leadtimedimids[1] = { leadTimeDimId };
				int ensmemberdimids[1] = { ensMemberDimId };

				int code = nc_def_var(ncid, kTimeVarName.c_str(), NC_FLOAT, 1, timedimids, &timeVarId);
				code = nc_def_var(ncid, kStationNameVarName.c_str(), NC_CHAR, 2, stationnamedimids, &stationNameVarId);
				code = nc_def_var(ncid, kStationIdVarName.c_str(), NC_INT, 1, stationiddimids, &stationIdVarId);
				code = nc_def_var(ncid, kLeadTimeVarName.c_str(), NC_FLOAT, 1, leadtimedimids, &leadTimeVarId);
				code = nc_def_var(ncid, kEnsMemberVarName.c_str(), NC_INT, 1, ensmemberdimids, &ensMemberVarId);
				code = nc_def_var(ncid, kLatVarName.c_str(), NC_FLOAT, 1, stationiddimids, &latVarId);
				code = nc_def_var(ncid, kLonVarName.c_str(), NC_FLOAT, 1, stationiddimids, &lonVarId);
				code = nc_def_var(ncid, kElevationVarName.c_str(), NC_FLOAT, 1, stationiddimids, &elevationVarId);
			}

			//void SwiftNetCDFAccess::DefineDimVariables_V1()
			//{
			//	int timedimids[1] = { timeDimId };
			//	int stationnamedimids[2] = { stationDimId, strLenDimId };
			//	int stationiddimids[1] = { stationDimId };
			//	int leadtimedimids[1] = { leadTimeDimId };
			//	int ensmemberdimids[1] = { ensMemberDimId };

			//	int code = nc_def_var(ncid, kTimeVarName.c_str(), NC_INT, 1, timedimids, &timeVarId);
			//	code = nc_def_var(ncid, kStationNameVarName.c_str(), NC_CHAR, 2, stationnamedimids, &stationNameVarId);
			//	code = nc_def_var(ncid, kStationIdVarName.c_str(), NC_INT, 1, stationiddimids, &stationIdVarId);
			//	code = nc_def_var(ncid, kLeadTimeVarName.c_str(), NC_INT, 1, leadtimedimids, &leadTimeVarId);
			//	code = nc_def_var(ncid, kEnsMemberVarName.c_str(), NC_INT, 1, ensmemberdimids, &ensMemberVarId);
			//	code = nc_def_var(ncid, kLatVarName.c_str(), NC_FLOAT, 1, stationiddimids, &latVarId);
			//	code = nc_def_var(ncid, kLonVarName.c_str(), NC_FLOAT, 1, stationiddimids, &lonVarId);
			//	code = nc_def_var(ncid, kElevationVarName.c_str(), NC_FLOAT, 1, stationiddimids, &elevationVarId);
			//}

			void SwiftNetCDFAccess::WriteCommonVarData()
			{
				// Set Station ID's
				SetVariableDimOne<int>(stationIdVarId, stationIds);
				// Set Ensemble No's
				size_t eLen = GetEnsembleSize();
				vector<int> ensembleIds = datatypes::utils::SeqVec<int>(0, 1, eLen);
				SetVariableDimOne<int>(ensMemberVarId, ensembleIds);
			}

			size_t SwiftNetCDFAccess::GetEnsembleSize()
			{
				return this->ensembleSize;
			}

			size_t SwiftNetCDFAccess::GetEnsembleSize(const string& ncVarName)
			{
				if (ncVarName.empty())
					return this->ensembleSize;
				else if (GetNumDims(ncVarName)> 2)
					// double var2_fcast_ens[lead_time,station,ens_member,time]
					//   double var1_ens[station,ens_member,time]  
					return this->ensembleSize;
				else
					// double var2_obs[station,time]
					return 0;
			}

			size_t SwiftNetCDFAccess::GetNumDims(const string& ncVarName)
			{
				int ndimsp;
				int code = nc_inq_varndims(ncid, this->GetVarId(ncVarName), &ndimsp);
				if (NC_NOERR != code)
				{
					ThrowOnVarInquiryFail(ncVarName, code);
				}
				return ndimsp;
			}

			size_t SwiftNetCDFAccess::GetLeadTimeCount(const string& ncVarName)
			{
				if (ncVarName.empty())
					return leadTimeLength;
				if (GetNumDims(ncVarName)> 3)
					// double var2_fcast_ens[lead_time,station,ens_member,time]
					return leadTimeLength;
				else
					//   double var1_ens[station,ens_member,time]  
					// double var2_obs[station,time]
					return 0; 
			}

			size_t SwiftNetCDFAccess::GetLeadTimeCount()
			{
				return leadTimeLength;
			}

			vector<ptime> SwiftNetCDFAccess::GetTimeDim()
			{
				size_t timeLen = this->GetTimeLength();
				auto tstep = GetTimeUnitTimeStep();
				vector<ptime> result(timeLen);
				for (size_t i = 0; i < timeLen; i++)
				{
					result[i] = tstep.AddSteps(startDate, timeVec[i]);
				}
				return result;
			}

			size_t SwiftNetCDFAccess::GetTimeLength() const
			{
				return this->numTimeSteps;
			}

			ptime SwiftNetCDFAccess::GetStart()
			{
				return ptime(startDate);
			}

			ptime SwiftNetCDFAccess::GetEnd()
			{
				return GetTimeStep().AddSteps(GetStart(), GetTimeLength()-1);
			}

			size_t SwiftNetCDFAccess::IndexForIdentifier(const string& identifier) const
			{
				using boost::lexical_cast;

				if (identifier == string(""))
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Identifier provided cannot be an empty string.");

				int intIdentifier = lexical_cast<int>(identifier);

				vector<int> testVector(stationIds);

				auto index = std::find(testVector.begin(), testVector.end(), intIdentifier);

				//return (index == testVector.end() ? -1 : index - testVector.begin());
				// see issue 409 for consideration of the following, alternate behavior:
				if (index == testVector.end())
					ExceptionUtilities::ThrowNotSupported("Index not found for series identifier " + identifier);
				return(index - testVector.begin());
			}

			size_t SwiftNetCDFAccess::GetNumIdentifiers() const
			{
				return numStations;
			}

			vector<string> SwiftNetCDFAccess::GetIdentifiers() const
			{
				vector<int> testVector(stationIds);
				std::function<string(const int&)> f = [&](const int& p) { return boost::lexical_cast<string>(p); };
				return SwiftNetCDFAccess::Convert<int, string>(testVector, f);
			}

			ptime SwiftNetCDFAccess::TimeForIndex(size_t timeIndex)
			{
				ExceptionUtilities::CheckInRange<size_t>(timeIndex, 0, numTimeSteps, "timeIndex");
				if (cachedTimeVector)
					return this->ptimeVec[timeIndex];
				else
				{
					this->ptimeVec = vector<ptime>(numTimeSteps);
					auto timeStart = GetStart();
					auto tst = GetTimeStep();
					if (timeVec.size() == numTimeSteps)
					{
						for (size_t i = 0; i < numTimeSteps; i++)
							ptimeVec[i] = tst.AddSteps(timeStart, timeVec[i]);
					}
					else // default fallback
					{
						for (size_t i = 0; i < numTimeSteps; i++)
							ptimeVec[i] = tst.AddSteps(timeStart, i);
					}
					cachedTimeVector = true;
					return this->ptimeVec[timeIndex];
				}
			}

			TimeStep SwiftNetCDFAccess::GetTimeStep()
			{
				return FindTimeStep<float>(cachedTimeStep, this->timeStep, timeVec);
			}

			TimeStep SwiftNetCDFAccess::GetLeadTimeStep()
			{
				return FindTimeStep<float>(cachedLeadTimeStep, this->leadTimeStep, leadTimeVec);
			}

			TimeStep SwiftNetCDFAccess::GetTimeUnitTimeStep() const
			{
				return GetTimeUnitTimeStep(this->timeUnits);
			}

			TimeStep SwiftNetCDFAccess::GetTimeUnitTimeStep(const string& timeUnits)
			{
				if (timeUnits == SWIFT_TIME_UNIT_DAY) return TimeStep::GetDaily();
				else if (timeUnits == SWIFT_TIME_UNIT_HOUR) return TimeStep::GetHourly();
				else if (timeUnits == SWIFT_TIME_UNIT_MINUTE) return TimeStep::GetHourly() / 60;
				else if (timeUnits == SWIFT_TIME_UNIT_MONTH) return TimeStep::GetMonthlyQpp();
				// Below is proabaly only legacy
				else if (timeUnits == string("hr")) return TimeStep::GetHourly();
				else if (timeUnits == string("da")) return TimeStep::GetDaily();
				else if (timeUnits == string("mo"))
					ExceptionUtilities::ThrowInvalidOperation("Monthly time step is not supported");
				else
					ExceptionUtilities::ThrowInvalidOperation("Unknown time unit identifier " + timeUnits);
			}

			std::pair<ptime, TimeStep> SwiftNetCDFAccess::CreateTimeGeometry(const string& axisDefinition, const vector<double>& timeCoords)
			{
				ptime origin = ParseStartDate(axisDefinition);
				auto utstep = GetTimeUnitTimeStep(ParseTimeUnits(axisDefinition));
				ptime start = StartCoordinate<double>(origin, utstep, timeCoords);
				TimeStep timeStep = FindTimeStep<double>(utstep, timeCoords);
				return std::pair<ptime, TimeStep>(start, timeStep);
			}

			std::pair<ptime, TimeStep> SwiftNetCDFAccess::GetLeadTimeGeometry(const ptime& issueTime)
			{
				auto utstep = GetTimeUnitTimeStep();
				ptime start = StartCoordinate(issueTime, utstep, leadTimeVec);
				auto tStep = GetLeadTimeStep();
				return std::pair<ptime, TimeStep>(start, tStep);
			}

			vector<double> SwiftNetCDFAccess::GetLeadTimeDim()
			{
				return Convert<float, double>(leadTimeVec);
			}

			double * SwiftNetCDFAccess::GetForecastDataBuffer(size_t numStations, size_t numTimeSteps)
			{
				// double rain_fcast_ens[lead_time,station,ens_member,time] in R; rev conventions for C, but order does not matter here
				return new double[leadTimeLength * numStations * ensembleSize * numTimeSteps];
			}

			double * SwiftNetCDFAccess::GetEnsembleDataBuffer(size_t numStations, size_t numTimeSteps)
			{
				// double rain_ens[station,ens_member,time] in R; rev conventions for C, but order does not matter here
				return new double[numStations * ensembleSize * numTimeSteps];
			}

			double * SwiftNetCDFAccess::GetSingleSeriesDataBuffer(size_t numStations, size_t numTimeSteps)
			{
				// double q_der[station, time]  in R; rev conventions for C
				// but order does not matter here
				return new double[numStations * numTimeSteps];
			}

			int SwiftNetCDFAccess::GetVarId(const string& varName)
			{
				int dataVarId;
				int code = nc_inq_varid(ncid, varName.c_str(), &dataVarId);
				if (NC_NOERR != code)
				{
					ExceptionUtilities::ThrowInvalidArgument("variable name not found " + varName);
				}
				return dataVarId;
			}

			void SwiftNetCDFAccess::GetEnsFcastNetcdfWindow(const string& varName, size_t stationIndex, size_t timeIndex, vector<size_t>& startp, vector<size_t>& countp)
			{
				GetNetcdfWindow(varName, stationIndex, startp, countp);
				// size_t startp[4]; // double rain_fcast_ens[lead_time,station,ens_member,time] in R; rev conventions for C
				vector<int> varDims = GetVarDims(varName);
				size_t n = varDims.size();
				for (size_t i = 0; i < n; i++)
				{
					int dimId = varDims[i];
					if (dimId == this->timeDimId)
					{
						startp[i] = timeIndex;
						countp[i] = 1;
					}
				}
			}

			//void SwiftNetCDFAccess::GetEnsNetcdfWindow(size_t stationIndex, size_t *startp, size_t *countp)
			//{
			//	// double q_ens[station, ens_member, time]  in R; rev conventions for C
			//	startp[0] = 0;
			//	startp[1] = 0;
			//	startp[2] = catchmentNumber;

			//	countp[0] = this->GetTimeLength();
			//	countp[1] = ensembleSize;
			//	countp[2] = 1;
			//}

			void SwiftNetCDFAccess::GetNetcdfWindow(const string& varName, vector<size_t>& startp, vector<size_t>& countp)
			{
				vector<int> varDims = GetVarDims(varName);
				size_t n = varDims.size();
				startp = vector<size_t>(n);
				countp = vector<size_t>(n);

				for (size_t i = 0; i < n; i++)
				{
					int dimId = varDims[i];
					if (dimId == this->stationDimId)
					{
						startp[i] = 0;
						countp[i] = GetNumIdentifiers();
					}
					else if (dimId == this->timeDimId)
					{
						startp[i] = 0;
						countp[i] = GetTimeLength();
					}
					else if (dimId == this->ensMemberDimId)
					{
						startp[i] = 0;
						countp[i] = GetEnsembleSize();
					}
					else if (dimId == this->leadTimeDimId)
					{
						startp[i] = 0;
						countp[i] = GetLeadTimeCount();
					}
					else
					{
						ExceptionUtilities::ThrowNotSupported("Variable " + varName + " has a dimension other than time, station, ensemble or lead time");
					}
				}
			}

			void SwiftNetCDFAccess::GetNetcdfWindow(const string& varName, size_t stationIndex, vector<size_t>& startp, vector<size_t>& countp)
			{
				// TODO: see issue 409:
				if (stationIndex < 0)
					stationIndex = 0;

				GetNetcdfWindow(varName, startp, countp);
				vector<int> varDims = GetVarDims(varName);
				size_t n = varDims.size();
				for (size_t i = 0; i < n; i++)
				{
					int dimId = varDims[i];
					if (dimId == this->stationDimId)
					{
						startp[i] = stationIndex;
						countp[i] = 1;
					}
				}
			}
		}

		time_duration SwiftNetCDFAccess::TimeOffsetIn;
		time_duration SwiftNetCDFAccess::TimeOffsetOut;

		void SwiftNetCDFAccess::SetTimeOffsetIn(const time_duration& td)
		{
			TimeOffsetIn = time_duration(td);
		}

		void SwiftNetCDFAccess::SetTimeOffsetOut(const time_duration& td)
		{
			TimeOffsetOut = time_duration(td);
		}

		TimeSeriesLibrary* TimeSeriesLibraryFactory::CreateLibraryPtr(const TimeSeriesLibraryDescription& description)
		{
			TimeSeriesLibrary* result = new TimeSeriesLibrary();
			CreateLibrary(description, *result);
			return result;
		}

		TimeSeriesLibrary TimeSeriesLibraryFactory::CreateLibrary(const TimeSeriesLibraryDescription& description)
		{
			TimeSeriesLibrary result;
			CreateLibrary(description, result);
			return result;
		}

		void TimeSeriesLibraryFactory::CreateLibrary(const TimeSeriesLibraryDescription& description, TimeSeriesLibrary& result)
		{
			auto singleIds = description.GetDataIdSingle();
			for (auto& id : singleIds)
			{
				result.AddSource(id, description.CreateSingleTimeSeriesStore(id));
			}
			auto ensIds = description.GetDataIdEnsembleTs();
			for (auto& id : ensIds)
			{
				result.AddSource(id, description.CreateEnsembleTimeSeriesStore(id));
			}
			auto tsEnsIds = description.GetDataIdTsEnsembleTs();
			for (auto& id : tsEnsIds)
			{
				result.AddSource(id, description.CreateTimeSeriesEnsembleTimeSeriesStore(id));
			}
		}

		using namespace datatypes::timeseries::io;

		SingleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			return new NetCdfSingleSeriesStore<double>(ncFilename, ncVarName, ncIdentifier);
		}

		EnsembleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			return new NetCdfEnsembleTimeSeriesStore<double>(ncFilename, ncVarName, ncIdentifier);
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateTsEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			return new NetCdfTimeSeriesEnsembleTimeSeriesStore<double>(ncFilename, ncVarName, ncIdentifier);
		}

		EnsembleForecastTimeSeries< TimeSeriesLibraryFactory::SeriesType >* TimeSeriesLibraryFactory::LoadTsEnsTs(const string& ncFilename, const string& ncVarName, const string& ncIdentifier, const string& dataId)
		{
			return TimeSeriesIOHelper<double>::ReadForecastTimeSeries(ncFilename, ncVarName, ncIdentifier);
			//TimeSeriesEnsembleTimeSeriesStore<double>* store;
			//auto result = store->GetSeries(dataId);
			//delete store;
			//return result;
		}

		TimeSeriesSourceInfo TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& fileName, const string& ncVarName, const string& identifier)
		{
			int index = -1;
			return CreateNetcdfSourceInfo(dataId, storageType, fileName, ncVarName, identifier, index, string(""), string(""), -1, -1);
		}

		TimeSeriesSourceInfo TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& fileName, const string& ncVarName, const string& identifier, int index,
			const string& timeStep, const string& start, int length, int ensembleSize)
		{
			return CreateNetcdfSourceInfo(dataId, storageType, fileName, ncVarName, identifier, index, string(""), string(""), -1, -1, -1, string(""));
		}

		// explicit instantiations. Without these, code using this DL library would fail at link time.
		// see http://stackoverflow.com/a/495056/2752565
		//template class SwiftNetCDFTimeSeriesStore < double >;

		// template class SwiftTimeSeriesStore < double > ;
		//template class SwiftNetCDFTimeSeries < double >;

		template class MultiFileTimeSeriesEnsembleTimeSeriesStore < double >;
		template class TimeSeriesIOHelper < double >;

		TimeSeriesSourceInfo TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& fileName, const string& ncVarName, const string& identifier, int index,
			const string& timeStep, const string& start, int length, int ensembleSize, int ensembleLength,
			const string& ensembleTimeStep)
		{
			// TODO: extra arguments
			if (storageType == NetCdfSourceInfo::StorageTypeMultipleNetcdfFiles ||
				storageType == NetCdfSourceInfo::StorageTypeSingleNetcdfFile)
				return TimeSeriesSourceInfo(NetCdfSourceInfo(dataId, fileName, ncVarName, identifier, index, storageType, timeStep, start, length, ensembleSize, ensembleLength, ensembleTimeStep));
			else
			{
				ExceptionUtilities::ThrowNotImplemented("Unknown storage type: " + storageType);
				return TimeSeriesSourceInfo();
			}
		}

		SwiftNetcdfStoreFactory::SwiftNetcdfStoreFactory(const string& path, DataGeometryProvider* dgp) :
			dirPath(path) , geometryProvider(dgp)
		{
		}
		SwiftNetcdfStoreFactory::~SwiftNetcdfStoreFactory() {}
		TimeSeriesEnsembleTimeSeriesStore<double>* SwiftNetcdfStoreFactory::CreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId)
		{
			using namespace boost::filesystem;
			auto file = CreateNcFilename(dataId);
			ExceptionUtilities::CheckFileDoesNotExist(file);
			// DimensionsDefinitions dimDef = geometryProvider->GetDimensions();
			string units = "";
			auto v = VariableDefinition::TimeSeriesEnsembleTimeSeries(dataId, units, dataId);
			map<string, VariableDefinition> varDef;
			varDef[dataId] = v;
			string ncVarName = dataId;
			return new NetCdfTimeSeriesEnsembleTimeSeriesStore<double>(file.generic_string(), ncVarName, "", /*writeMode*/ true);
			//return new NetCdfTimeSeriesEnsembleTimeSeriesStore<double>(file.generic_string(), dimDef, varDef, dataId);
		}

		bool SwiftNetcdfStoreFactory::CanCreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId)
		{
			using namespace boost::filesystem;
			return exists(dirPath) && !exists(CreateNcFilename(dataId));
		}

		template <typename T>
		typename TimeSeriesIOHelper<T>::SeriesType* TimeSeriesIOHelper<T>::Read(const string& netCdfFilePath, const string& varName, const string& identifier)
		{
			NetCdfSingleSeriesStore<T> netCdfTimeSeries(netCdfFilePath, varName, identifier);
			auto result = netCdfTimeSeries.Read();
			return result;
		}

		template <typename T>
		void TimeSeriesIOHelper<T>::Write(const string& varName, std::map<string, TTimeSeries<T>*>& recordedTimeSeries,
			const std::map<string, string>& idMap, const string& filePath)
		{
			using datatypes::utils::STLHelper;
			if (recordedTimeSeries.size() == 0) return;
			auto keys = STLHelper::GetKeys<string, TTimeSeries<T>*>(recordedTimeSeries);
			TTimeSeries<T>* aRecordedTimeSeries = recordedTimeSeries[keys[0]];
			std::vector<std::string> stationIds = STLHelper::GetValues(idMap);
			DimensionsDefinitions dimDefinitions = DimensionsFromPointTimeSeries<T>(aRecordedTimeSeries);
			dimDefinitions.StationIds = stationIds;
			std::map<string, VariableDefinition> varDefinitions;
			varDefinitions[varName] = VariableDefinition::PointTimeSeries(varName, "<NA>", varName);
			GlobalAttributes globAtts = GlobalAttributes::CreateDefault();
			NetCdfSingleSeriesStore<T> store(filePath, dimDefinitions, varDefinitions, globAtts, varName);
			std::map<string, TTimeSeries<T>*> toSave = STLHelper::Remap<string, TTimeSeries<T>*>(recordedTimeSeries, idMap);
			store.WriteToIdentifiers(toSave);
		}

		template void TimeSeriesIOHelper<double>::Write(const string& varName, std::map<string, TTimeSeries<double>*>& recordedTimeSeries, const std::map<string, string>& idMap, const string& filePath);

		template <typename T>
		void TimeSeriesIOHelper<T>::Write(DimensionsDefinitions& dimDefinitions, const map<std::string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes,
			std::map<string, TTimeSeries<T>*>& recordedTimeSeries, const string& filePath)
		{
			using datatypes::utils::STLHelper;
			if (recordedTimeSeries.size() == 0) return;
			NetCdfSingleSeriesStore<T> store(filePath, dimDefinitions, varDefinitions, globalAttributes, "");
			store.WriteToNcVariables(recordedTimeSeries);
		}

		template void TimeSeriesIOHelper<double>::Write(DimensionsDefinitions& dimDefinitions, const map<std::string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, std::map<string, TTimeSeries<double>*>& recordedTimeSeries, const string& filePath);

		template <typename T>
		typename TimeSeriesIOHelper<T>::PtrEnsemblePtrType TimeSeriesIOHelper<T>::ReadForecastTimeSeries(const string& netCdfFilepath, const string& varName, const string& identifier, int index)
		{
			NetCdfTimeSeriesEnsembleTimeSeriesStore<T> forecastRainEnsemble(netCdfFilepath, varName, identifier);
			PtrEnsemblePtrType forecastRainMultiTimeSeries = forecastRainEnsemble.GetForecasts(index);
			return forecastRainMultiTimeSeries;
		}

		template <typename T>
		typename TimeSeriesIOHelper<T>::PtrTSeriesEnsemblePtrType TimeSeriesIOHelper<T>::ReadForecastTimeSeries(const string& netCdfFilepath, const string& varName, const string& identifier)
		{
			NetCdfTimeSeriesEnsembleTimeSeriesStore<T> forecastRainEnsemble(netCdfFilepath, varName, identifier);
			return forecastRainEnsemble.GetSeries("");
		}

	}
}
