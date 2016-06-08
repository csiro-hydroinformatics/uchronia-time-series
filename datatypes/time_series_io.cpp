
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "exception_utilities.h"
#include "time_series_io.hpp"
#include "time_series_store.hpp"

namespace datatypes
{
	namespace timeseries
	{
		namespace io
		{
			SwiftNetCDFAccess::SwiftNetCDFAccess(const string& filename)
			{
				int code = nc_open(filename.c_str(), NC_NOWRITE, &ncid);
				if (NC_NOERR != code)
				{
					ThrowOnFileOpenFail(filename, code);
				}
				else
				{
					ReadGeometry();
				}
			}

			SwiftNetCDFAccess::SwiftNetCDFAccess(const string& filename, int nEns, int nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds, 
				vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes/*, const string& stationIdVarName*/)
			{
				auto fname = filename.c_str();
				int code = nc_create(fname, NC_CLASSIC_MODEL | NC_CLOBBER, &ncid);
				if (NC_NOERR != code)
				{
					ThrowOnFileOpenFail(filename, code);
				}
				else
				{
					WriteGeometry(nEns, nLead, timeUnits, timeVar, stationIds, varNames, varAttributes);
				}
			}

			SwiftNetCDFAccess::~SwiftNetCDFAccess()
			{
				if (stationNames != nullptr) delete stationNames;
				if (variableVarNames != nullptr) delete variableVarNames;
				if (variableVarIds != nullptr) delete[] variableVarIds;
				if (stationIds != nullptr) delete[] stationIds;
				if (timeVec != nullptr) delete[] timeVec;
				if (leadTimeVec != nullptr) delete[] leadTimeVec;
				if (stationLat != nullptr) delete[] stationLat;
				if (stationLon != nullptr) delete[] stationLon;
				if (stationElevation != nullptr) delete[] stationElevation;
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
				return startDate;

			}

			string SwiftNetCDFAccess::CreateTimeUnitsAttribute(const ptime& utcStart, TimeStep& timeStep)
			{
				string timeUnit;

				string name = timeStep.GetName();

				if (name == "daily")
					timeUnit = "days";
				else if (name == "hourly")
					timeUnit = "hours";
				else
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Unsupported timestep " + name);

				return CreateTimeUnitsAttribute(utcStart, timeUnit);
			}

			string SwiftNetCDFAccess::CreateTimeUnitsAttribute(const ptime& utcStart, const string& units)
			{
				string timeUnit(units);
				auto startStr = boost::posix_time::to_iso_extended_string(utcStart);
				return timeUnit + " since " + startStr + " +0000";
			}

			void SwiftNetCDFAccess::ReadGeometry()
			{
				InquireDimIds();
				InquireDimVarIds();
				ReadTimeUnits();

				numTimeSteps = InquireDimLength(timeDimId);
				numStations = InquireDimLength(stationDimId);
				strLen = InquireDimLength(strLenDimId);
				leadTimeLength = InquireDimLength(leadTimeDimId);
				ensembleSize = InquireDimLength(ensMemberDimId);

				stationNames = GetStringVariable(strLen, stationNameVarId, numStations);

				stationIds = GetVariable<int>(stationIdVarId, numStations);
				stationLat = GetVariable<double>(latVarId, numStations);
				stationLon = GetVariable<double>(lonVarId, numStations);
				stationElevation = GetVariable<double>(elevationVarId, numStations);

				catchmentName = HasAttribute(kCatchmentAttName) ? GetStringAttribute(kCatchmentAttName) : "<unspecified>";

				timeVec = GetIntVariable(timeVarId, numTimeSteps);
				leadTimeVec = GetIntVariable(leadTimeVarId, leadTimeLength);
			}

			void SwiftNetCDFAccess::SetTimeVar(vector<double>& timeVar)
			{
				double * vec = timeVar.data();
				nc_put_var(ncid, timeVarId, vec);
				this->timeVec = ConvertToArray<double, int>(timeVar);
			}

			void SwiftNetCDFAccess::WriteGeometry(int nEns, int nLead, const string& timeUnits, vector<double>& timeVar, vector<string>& stationIds, vector<string>& varNames, std::map<string, VariableAttributes>& varAttributes)
			{
				using namespace std;
				DefineMandatoryDimensions(nEns, nLead, stationIds.size());
				DefineDimVariables();
				FillAttributes(timeUnits);

				variableNames = vector<string>(varNames);
				variableAttributes = map<string, VariableAttributes>(varAttributes);

				ncendef(ncid);
				SetTimeVar(timeVar);

				numTimeSteps = timeVar.size();
				numStations = stationIds.size();
				// strLen; keep default value for the time being.

				stationNames = new vector<string>(stationIds); // KLUDGE: needs a change to the interface.

				this->stationIds = ConvertToArray<int>(stationIds);
				// stationLat = 
				// stationLon = 
				// stationElevation = 

				catchmentName = "<unspecified>";

				leadTimeVec = new int[leadTimeLength];
				for (int i = 0; i < leadTimeLength; i++)
				{
					leadTimeVec[i] = i + 1;
				}
				
				this->timeUnits = ParseTimeUnits(string(timeUnits));

				WriteCommonVarData();
			}

			int SwiftNetCDFAccess::AddGlobalAttribute(const string& attName, const string& attValue)
			{
				return AddAttribute(NC_GLOBAL, attName, attValue);
			}

			int SwiftNetCDFAccess::AddAttribute(int varId, const string& attName, const string& attValue)
			{
				return nc_put_att(ncid, varId, attName.c_str(), NC_CHAR, attValue.size() + 1, attValue.c_str());
			}

			int SwiftNetCDFAccess::AddGlobalAttribute(char * attName, char * attValue)
			{
				return AddGlobalAttribute(string(attName), string(attValue));
			}

			void SwiftNetCDFAccess::FillAttributes(const string& timeUnits)
			{
				using namespace boost::gregorian;
				namespace bt = boost::posix_time;

				//tempstring = variableName + " for " + mHead.mCatchmentName;
				//int tempint = tempstring.length() + 1;
				//int code = nc_put_att(ncid, NC_GLOBAL, "title", NC_CHAR, tempint, tempstring.c_str());

				int code;
				code = AddGlobalAttribute("institution", "CSIRO Land and Water");
				code = AddGlobalAttribute("source", "SWIFT (devel version)");
				code = AddGlobalAttribute(string("catchment"), this->catchmentName);
				code = AddGlobalAttribute("STF_convention_version", "2.0");
				code = AddGlobalAttribute("STF_nc_spec", "https://wiki.csiro.au/display/wirada/NetCDF+for+SWIFT");
				code = AddGlobalAttribute("comment", "We do not comment on-water matters");

				auto todo = string("TODO");

				// Not sure whether what is wanted. 
				// See for instance, for formats: http://www.boost.org/doc/libs/1_56_0/doc/html/date_time/date_time_io.html
				auto timeStamp = bt::to_iso_extended_string(bt::second_clock::universal_time());
				code = AddGlobalAttribute(string(kHistoryAttName), timeStamp + string(" UTC - New file created "));

				code = AddAttribute(timeVarId, string(kStandardNameAttName), string(kTimeVarName));
				code = AddAttribute(timeVarId, string(kLongNameAttName), string(kTimeVarName));

				code = AddAttribute(timeVarId, string(kUnitsAttName), string(timeUnits));
				code = AddAttribute(timeVarId, string("time_standard"), todo);
				code = AddAttribute(timeVarId, string(kAxisAttName), todo);

				code = AddAttribute(stationIdVarId, string(kLongNameAttName), todo);
				code = AddAttribute(stationNameVarId, string(kLongNameAttName), todo);

				code = AddAttribute(ensMemberVarId, string(kLongNameAttName), todo);
				code = AddAttribute(ensMemberVarId, string(kStandardNameAttName), todo);
				code = AddAttribute(ensMemberVarId, string(kUnitsAttName), string("member id"));
				code = AddAttribute(ensMemberVarId, string(kAxisAttName), string("u"));

				code = AddAttribute(leadTimeVarId, string(kLongNameAttName), string("forecast lead time"));
				code = AddAttribute(leadTimeVarId, string(kStandardNameAttName), string("lead time"));
				code = AddAttribute(leadTimeVarId, string(kUnitsAttName), todo);
				code = AddAttribute(leadTimeVarId, string(kAxisAttName), string("v"));

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
				if (t2string == string("hour"))
				{
					timeUnits = string("hr");
				}
				else if (t2string == string("days"))
				{
					timeUnits = string("da");
				}
				else
				{
					timeUnits = string("mo");
				}
				return timeUnits;
			}


			int SwiftNetCDFAccess::InquireDimLength(int dimId)
			{
				size_t size;
				int code = nc_inq_dimlen(ncid, dimId, &size);

				return (int)size;
			}

			template<class T>
			T* SwiftNetCDFAccess::GetVariable(int varId, int n)
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

			template<class T, class TStored>
			T* SwiftNetCDFAccess::GetVariable(int varId, int n)
			{
				T* result = new T[n];
				TStored* tmp = GetVariable<TStored>(varId, n);
				for (int i = 0; i < n; i++)
				{
					result[i] = static_cast<T>(tmp[i]);
				}
				delete[] tmp;
				return result;
			}

			vector<string> SwiftNetCDFAccess::ReadVariableNames()
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

			string SwiftNetCDFAccess::ReadStringAttribute(const string& varName, const string& attName)
			{
				int varId = GetVarId(varName);

				nc_type type;
				int code = nc_inq_atttype(ncid, varId, attName.c_str(), &type);

				if (type != NC_CHAR)
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Attribute attName not of type string.");

				size_t size;
				code = nc_inq_attlen(ncid, varId, attName.c_str(), &size);

				char* strAtt = new char[size];
				code = nc_get_att_text(ncid, varId, attName.c_str(), strAtt);
				string result = string(strAtt);

				delete[] strAtt;

				return result;
			}

			double SwiftNetCDFAccess::ReadNumericAttribute(const string& varName, const string& attName)
			{
				int varId = GetVarId(varName);

				nc_type type;
				int code = nc_inq_atttype(ncid, varId, attName.c_str(), &type);

				if (type != NC_DOUBLE || type != NC_FLOAT || type != NC_INT || type != NC_INT64)
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Attribute attName not of numeric type (double, float, int or int64).");

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

			string SwiftNetCDFAccess::GetStringAttribute(const string& attName)
			{
				size_t size;
				int code = nc_inq_attlen(ncid, NC_GLOBAL, attName.c_str(), &size);

				char * tempchars = new char[size + 1];
				code = nc_get_att(ncid, NC_GLOBAL, attName.c_str(), tempchars);

				auto result = string(tempchars, size);
				delete[] tempchars;
				return result;
			}

			bool SwiftNetCDFAccess::HasAttribute(const string& attName)
			{
				size_t size;
				int code = nc_inq_attlen(ncid, NC_GLOBAL, attName.c_str(), &size);
				return (code == NC_NOERR);
			}

			nc_type SwiftNetCDFAccess::GetDataType(int variableId)
			{
				nc_type datatype;
				int code = nc_inq_vartype(ncid, variableId, &datatype);
				return datatype;
			}

			vector<string> * SwiftNetCDFAccess::GetStringVariable(int strLen, int varId, int n)
			{
				char * bfr = new char[strLen];
				auto strVec = new vector<string>();
				size_t starts[2] = { 0, 0 };
				const size_t counts[2] = { 1, strLen };
				for (int i = 0; i < n; i++)
				{
					starts[0] = i;
					int code = nc_get_vara(ncid, varId, starts, counts, bfr);
					strVec->push_back(string(bfr));
				}
				delete bfr;
				return strVec;
			}

			int * SwiftNetCDFAccess::GetIntVariable(int varId, int size)
			{
				nc_type datatype = GetDataType(varId);
				if (datatype == NC_DOUBLE)
				{
					return GetVariable<int, double>(varId, size);
				}
				else if (datatype == NC_INT)
				{
					return GetVariable<int>(varId, size);
				}
				else
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation(string("The netCDF data type code ") + std::to_string(datatype) + " of the variable " + std::to_string(varId) + " is not supported ");
			}

			void SwiftNetCDFAccess::WriteSingleSeriesVarData()
			{
				const int varNumDims = 2;
				int vardimids[varNumDims];
				vardimids[0] = timeDimId;
				vardimids[1] = stationDimId;

				DefineVariables(varNumDims, vardimids);
			}

			void SwiftNetCDFAccess::WriteEnsembleVarData()
			{
				const int varNumDims = 3;
				int vardimids[varNumDims];
				vardimids[0] = timeDimId;
				vardimids[1] = ensMemberDimId;
				vardimids[2] = stationDimId;

				DefineVariables(varNumDims, vardimids);
			}

			void SwiftNetCDFAccess::WriteForecastsVarData()
			{
				const int varNumDims = 4;
				int vardimids[varNumDims];
				vardimids[0] = timeDimId;
				vardimids[1] = ensMemberDimId;
				vardimids[2] = stationDimId;
				vardimids[3] = leadTimeDimId;

				DefineVariables(varNumDims, vardimids);
			}

			void SwiftNetCDFAccess::DefineVariables(const int varNumDims, int* varDimIDs)
			{
				ncredef(ncid);

				int code;
				variableVarIds = new int[variableNames.size()];
				variableVarNames = new vector<string>(variableNames);

				for (int i = 0; i < variableNames.size(); i++)
				{
					string varName = variableNames[i];
					code = nc_def_var(ncid, varName.c_str(), NC_DOUBLE, varNumDims, varDimIDs, variableVarIds + i);
					int varId = variableVarIds[i];
					AddAttributes(varId, varName, variableAttributes);
				}

				ncendef(ncid);
			}

			void SwiftNetCDFAccess::AddVariableAttributes(int varId, const string& longName, const string& units, const string& type, double fillValue)
			{
				int code = nc_put_att_double(ncid, varId, kFillValueAttName.c_str(), NC_DOUBLE, (size_t)1, &fillValue);
				code = AddAttribute(varId, string(kLongNameAttName), longName);
				code = AddAttribute(varId, string(kUnitsAttName), units);
				code = AddAttribute(varId, string(kTypeAttName), type);
			}

			void SwiftNetCDFAccess::AddAttributes(int varId, const string& varName, std::map<string, VariableAttributes>& varAttributes)
			{
				int code;
				if (varAttributes.find(varName) == varAttributes.end())
				{
					AddVariableAttributes(varId, varName, string("unknown"), string("unknown"), VariableAttributes::DefaultFillValue());
				}
				else
				{
					auto att = varAttributes.at(varName);
					AddVariableAttributes(varId, att.LongName, att.Units, att.Type, att.FillValue);
				}
			}

			void SwiftNetCDFAccess::DefineMandatoryDimensions(int nEns, int nLead, int nStations)
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
				int timedimids[1] = { timeDimId };
				int stationnamedimids[2] = { stationDimId, strLenDimId };
				int stationiddimids[1] = { stationDimId };
				int leadtimedimids[1] = { leadTimeDimId };
				int ensmemberdimids[1] = { ensMemberDimId };

				int code = nc_def_var(ncid, kTimeVarName.c_str(), NC_INT, 1, timedimids, &timeVarId);
				code = nc_def_var(ncid, kStationNameVarName.c_str(), NC_CHAR, 2, stationnamedimids, &stationNameVarId);
				code = nc_def_var(ncid, kStationIdVarName.c_str(), NC_INT, 1, stationiddimids, &stationIdVarId);
				code = nc_def_var(ncid, kLeadTimeVarName.c_str(), NC_INT, 1, leadtimedimids, &leadTimeVarId);
				code = nc_def_var(ncid, kEnsMemberVarName.c_str(), NC_INT, 1, ensmemberdimids, &ensMemberVarId);
				code = nc_def_var(ncid, kLatVarName.c_str(), NC_FLOAT, 1, stationiddimids, &latVarId);
				code = nc_def_var(ncid, kLonVarName.c_str(), NC_FLOAT, 1, stationiddimids, &lonVarId);
				code = nc_def_var(ncid, kElevationVarName.c_str(), NC_FLOAT, 1, stationiddimids, &elevationVarId);
			}

			void SwiftNetCDFAccess::WriteCommonVarData()
			{
				int code;
				size_t* startp;
				size_t* countp;

				startp = new size_t[1];
				countp = new size_t[1];

				// Set Station ID's

				startp[0] = 0;
				countp[0] = numStations;

				code = nc_put_vara_int(ncid, stationIdVarId, startp, countp, stationIds);

				// Set Ensemble No's

				int eLen = GetEnsembleSize();

				startp[0] = 0;
				countp[0] = eLen;

				int* ensArr = new int[eLen];
				for (int i = 0; i < eLen; i++)
					ensArr[i] = i;

				code = nc_put_vara_int(ncid, ensMemberVarId, startp, countp, ensArr);

				delete[] ensArr;

				// Set Lead Time No's
				
				int fLen = GetLeadTimeCount();

				startp[0] = 0;
				countp[0] = fLen;

				int* ldtArr = new int[fLen];
				for (int i = 0; i < fLen; i++)
					ldtArr[i] = i + 1;

				code = nc_put_vara_int(ncid, leadTimeVarId, startp, countp, ldtArr);

				delete[] ldtArr;


				delete[] startp;
				delete[] countp;
			}

			int SwiftNetCDFAccess::GetEnsembleSize()
			{
				return this->ensembleSize;
			}

			int SwiftNetCDFAccess::GetEnsembleSize(const string& ncVarName)
			{
				if (GetNumDims(ncVarName)> 2)  
					// double var2_fcast_ens[lead_time,station,ens_member,time]
					//   double var1_ens[station,ens_member,time]  
					return this->ensembleSize;  
				else
					// double var2_obs[station,time]
					return 0;
			}

			int SwiftNetCDFAccess::GetNumDims(const string& ncVarName)
			{
				int ndimsp;
				int code = nc_inq_varndims(ncid, this->GetVarId(ncVarName), &ndimsp);
				if (NC_NOERR != code)
				{
					ThrowOnVarInquiryFail(ncVarName, code);
				}
				return ndimsp;
			}

			int SwiftNetCDFAccess::GetLeadTimeCount(const string& ncVarName)
			{
				if (GetNumDims(ncVarName)> 3) 
					// double var2_fcast_ens[lead_time,station,ens_member,time]
					return leadTimeLength;
				else
					//   double var1_ens[station,ens_member,time]  
					// double var2_obs[station,time]
					return 0; 
			}

			int SwiftNetCDFAccess::GetLeadTimeCount()
			{
				return leadTimeLength;
			}

			vector<ptime> SwiftNetCDFAccess::GetTimeDim()
			{
				int timeLen = this->GetTimeLength();
				auto tstep = GetTimeStep();
				vector<ptime> result(timeLen);
				for (size_t i = 0; i < timeLen; i++)
				{
					result[i] = tstep.AddSteps(startDate, timeVec[i]);
				}
				return result;
			}

			int SwiftNetCDFAccess::GetTimeLength()
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

			int SwiftNetCDFAccess::IndexForIdentifier(const string& identifier) const
			{
				using boost::lexical_cast;

				int intIdentifier = lexical_cast<int>(identifier);

				vector<int> testVector(stationIds, stationIds + numStations);

				auto index = std::find(testVector.begin(), testVector.end(), intIdentifier);
				return (index == testVector.end() ? -1 : index - testVector.begin());
			}

			vector<string> SwiftNetCDFAccess::GetIdentifiers() const
			{
				vector<int> testVector(stationIds, stationIds + numStations);
				std::function<string(const int&)> f = [&](const int& p) { return boost::lexical_cast<string>(p); };
				return SwiftNetCDFAccess::Convert<int, string>(testVector, f);
			}

			ptime SwiftNetCDFAccess::TimeForIndex(int timeIndex)
			{
				int delta = timeVec[timeIndex];
				auto timeStart = GetStart();
				return GetTimeStep().AddSteps(timeStart, timeIndex);
			}

			TimeStep SwiftNetCDFAccess::GetTimeStep()
			{
				// TODO: revise this. See also ParseTimeUnits
				if (timeUnits == string("hr")) return TimeStep::GetHourly();
				else if (timeUnits == string("da")) return TimeStep::GetDaily();
				else if (timeUnits == string("mo"))
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Monthly time step is not supported");
				else
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Unknown time unit identifier " + timeUnits);
			}

			double * SwiftNetCDFAccess::GetForecastDataBuffer(int numStations, int numTimeSteps)
			{
				// double rain_fcast_ens[lead_time,station,ens_member,time] in R; rev conventions for C, but order does not matter here
				return new double[leadTimeLength * numStations * ensembleSize * numTimeSteps];
			}

			double * SwiftNetCDFAccess::GetEnsembleDataBuffer(int numStations, int numTimeSteps)
			{
				// double rain_ens[station,ens_member,time] in R; rev conventions for C, but order does not matter here
				return new double[numStations * ensembleSize * numTimeSteps];
			}

			double * SwiftNetCDFAccess::GetSingleSeriesDataBuffer(int numStations, int numTimeSteps)
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
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("variable name not found " + varName);
				}
				return dataVarId;
			}

			void SwiftNetCDFAccess::GetEnsFcastNetcdfWindow(int catchmentNumber, int timeIndex, size_t *startp, size_t *countp)
			{
				// size_t startp[4]; // double rain_fcast_ens[lead_time,station,ens_member,time] in R; rev conventions for C
				startp[0] = timeIndex;
				startp[1] = 0;
				startp[2] = catchmentNumber;
				startp[3] = 0;

				// size_t countp[4]; // double rain_fcast_ens[lead_time,station,ens_member,time] in R; rev conventions for C
				countp[0] = 1;
				countp[1] = ensembleSize;
				countp[2] = 1;
				countp[3] = leadTimeLength;
			}

			void SwiftNetCDFAccess::GetEnsNetcdfWindow(int catchmentNumber, size_t *startp, size_t *countp)
			{
				// double q_ens[station, ens_member, time]  in R; rev conventions for C
				startp[0] = 0;
				startp[1] = 0;
				startp[2] = catchmentNumber;

				countp[0] = this->GetTimeLength();
				countp[1] = ensembleSize;
				countp[2] = 1;
			}

			void SwiftNetCDFAccess::GetNetcdfWindow(int catchmentNumber, size_t *startp, size_t *countp)
			{
				// double q_der[station, time]  in R; rev conventions for C
				startp[0] = 0;
				startp[1] = catchmentNumber;

				countp[0] = this->GetTimeLength();
				countp[1] = 1;
			}
		}

		TimeSeriesLibrary TimeSeriesLibraryFactory::CreateLibrary(const TimeSeriesLibraryDescription& description)
		{
			TimeSeriesLibrary result;
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

			//for (auto& id : tsEnsIds)
			//{
			//	TimeSeriesEnsembleTimeSeriesStore<double> * etss = new MultiFileTimeSeriesEnsembleTimeSeriesStore<double>(
			//		description.GetFilename(id),
			//		description.GetFileVariableName(id),
			//		description.GetIdentifier(id),
			//		description.GetIndex(id));
			//	result.AddSource(id, etss);
			//}
			return result;
		}

		SingleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			SwiftNetCDFTimeSeriesStore<double>* access = new SwiftNetCDFTimeSeriesStore<double>(ncFilename);
			return new NetCdfSingleSeriesStore<double>(access, ncVarName, ncIdentifier);
		}


		EnsembleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			SwiftNetCDFTimeSeriesStore<double>* access = new SwiftNetCDFTimeSeriesStore<double>(ncFilename);
			return new NetCdfEnsembleTimeSeriesStore<double>(access, ncVarName, ncIdentifier);
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesLibraryFactory::CreateTsEnsTsSource(const string& ncFilename, const string& ncVarName, const string& ncIdentifier)
		{
			SwiftNetCDFTimeSeriesStore<double>* access = new SwiftNetCDFTimeSeriesStore<double>(ncFilename);
			return new NetCdfTimeSeriesEnsembleTimeSeriesStore<double>(access, ncVarName, ncIdentifier);
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
		template class SwiftNetCDFTimeSeriesStore < double >;

		// template class SwiftTimeSeriesStore < double > ;
		template class SwiftNetCDFTimeSeries < double >;

		template class MultiFileTimeSeriesEnsembleTimeSeriesStore < double >;
		template class TimeSeriesIOHelper < double >;

		TimeSeriesSourceInfo TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(const string& dataId, const string& storageType, const string& fileName, const string& ncVarName, const string& identifier, int index,
			const string& timeStep, const string& start, int length, int ensembleSize, int ensembleLength,
			const string& ensembleTimeStep)
		{
			// TODO: extra arguments
			if (storageType == TimeSeriesLibrary::StorageTypeMultipleNetcdfFiles ||
				storageType == TimeSeriesLibrary::StorageTypeSingleNetcdfFile)
				return TimeSeriesSourceInfo(NetCdfSourceInfo(dataId, fileName, ncVarName, identifier, index, storageType, timeStep, start, length, ensembleSize, ensembleLength, ensembleTimeStep));
			else
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented("Unknown storage type: " + storageType);
				return TimeSeriesSourceInfo();
			}
		}

	}
}
