#pragma once

#include "../common.h"
#include "datatypes_test_helpers.h"
#include "../time_series_io.hpp"


namespace datatypes {
	namespace tests {

		using namespace datatypes::timeseries;
		using namespace boost::filesystem;

		path FileSystemHelper::GetTempFile()
		{
			path tempFname = unique_path();
			path tempDir = temp_directory_path();
			path fPath = tempDir / tempFname; // operator/ is overloaded
			return fPath;
		}

		void FileSystemHelper::Remove(const path& p)
		{
			if (exists(p))
			{
#ifdef _WIN32
				remove(p);
#else
			std:string path = p.generic_string();
				const char* p_str = path.c_str();
				std::remove(p_str);
#endif
			}
		}

		string TestDataLocationHelper::ReadEnvironmentVariable(const string& envVar) {

			using namespace std;
			char* ev = STRDUP(envVar.c_str());
			char* libvar = nullptr;
			string result;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) 
			size_t requiredSize;
			getenv_s(&requiredSize, nullptr, 0, ev);
			if (requiredSize == 0)
			{
				free(ev);
				string msg = "No environment variable called '" + envVar + "' found.";
				throw std::invalid_argument(msg);
			}

			libvar = (char*)malloc(requiredSize * sizeof(char));
			getenv_s(&requiredSize, libvar, requiredSize, ev);

			result = string(libvar);
			free(libvar);
#else
			libvar = getenv(ev);
			if (libvar == nullptr)
			{
				string msg = "No environment variable called '" + envVar + "' found.";
				throw std::invalid_argument(msg);
			}
			result = string(libvar);
#endif
			free(ev);
			return result;
		}

		string TestDataLocationHelper::BuildPath(const vector<string>& folders) {
			if (folders.size() == 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("BuildPath: 'folders' argument must have at least one element");
			path p(folders[0]);
			for (size_t i = 1; i < folders.size(); i++)
			{
				p = p / path(folders[i]);
			}
			return p.generic_string();
//			va_list vl;
//			va_start(vl, n_args);
//			string root = string(va_arg(vl, char*));
//			string sep;
//			size_t y = root.find("/");
//			if (root.find("/") >= 0 && root.find("/") < root.length()) {
//				sep = "/";
//			}
//			else if (root.find("\\") >= 0) {
//				sep = "\\";
//			}
//			else {
//#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) 
//				sep = "\\";
//#else
//				sep = "/";
//#endif
//			}
//
//			if (root[root.length() - 1] == sep[0]) {
//				root = root.substr(0, root.length() - 1);
//			}
//			for (int i = 1; i < n_args; i++) {
//				string str = string(va_arg(vl, char*));
//				root = root + sep + str;
//			}
//			va_end(vl);
//			return root;
		}

		TimeSeriesLibrary TestDataLocationHelper::CreateEnsembleTimeSeriesLibrary(string& rainObsId, string& petObsId, string& rainFcastId, string& petFcastId)
		{
			TimeSeriesLibrary dataLibrary;

			rainObsId = string("rain_obs");
			petObsId = string("pet_obs");
			rainFcastId = string("rain_fcast_ens");
			petFcastId = string("pet_fcast_ens");

			string rainNcVarname("rain_der");
			string petNcVarname("pet_der");

			string historicalSeriesNcIdentifier("1");

			string rainFcastNcVarname("rain_fcast_ens");

			string petFcastNcVarname(petNcVarname);

			string testDir = TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR");

			// Historic, observed data
			dataLibrary.AddSource(rainObsId,
				TimeSeriesLibraryFactory::CreateTsSource(TestDataLocationHelper::BuildPath({ testDir, "Obs_data", "Upper_Murray_rain_1hr.nc" }), rainNcVarname, historicalSeriesNcIdentifier));
			dataLibrary.AddSource(petObsId,
				TimeSeriesLibraryFactory::CreateTsSource(TestDataLocationHelper::BuildPath({ testDir, "Obs_data", "Upper_Murray_pet_24h_89_2012.nc" }), petNcVarname, historicalSeriesNcIdentifier));

			// Read Forecast PET Data
			dataLibrary.AddSource(petFcastId,
				TimeSeriesLibraryFactory::CreateTsSource(TestDataLocationHelper::BuildPath({ testDir, "Fct_Data", "Upper_Murray_pet_clim_1990_2010.nc" }), petNcVarname, historicalSeriesNcIdentifier));

			string forecastDataFiles(TestDataLocationHelper::BuildPath({ testDir, "Fct_Data", "Upper_Murray_F1_1_{0}21_shuffle.nc" })); // where {0} is e.g. 20100801
			TimeSeriesEnsembleTimeSeriesStore<double> * etss = new MultiFileTimeSeriesEnsembleTimeSeriesStore<double>(forecastDataFiles, rainFcastNcVarname, string("1"), 0);
			dataLibrary.AddSource(rainFcastId, etss);

			return dataLibrary;
		}

	}
}