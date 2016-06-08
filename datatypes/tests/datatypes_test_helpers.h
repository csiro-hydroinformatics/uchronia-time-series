#pragma once

#include "../common.h"
#include "../time_series_store.hpp"

using namespace datatypes::timeseries;

namespace datatypes {
	namespace tests {

		class DATATYPES_DLL_LIB FileSystemHelper {
		public:
			static boost::filesystem::path GetTempFile();
			static void Remove(const boost::filesystem::path& p);
		};

		class DATATYPES_DLL_LIB TestDataLocationHelper
		{
		public:
			static string ReadEnvironmentVariable(const string& envVar);
			static string BuildPath(const vector<string>& folders);
			static TimeSeriesLibrary CreateEnsembleTimeSeriesLibrary(string& rainObsId, string& petObsId, string& rainFcastId, string& petFcastId);
		};


	}
}