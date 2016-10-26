#include "../common.h"
#include "datatypes_test_helpers.h"
#include "../time_series_io.hpp"


namespace datatypes {
	namespace tests {

		using namespace datatypes::timeseries;
		using namespace boost::filesystem;

		path FileSystemHelper::GetTempFile(const string& format)
		{
			path tempFname = unique_path(format);
			path tempDir = temp_directory_path();
			path fPath = tempDir / tempFname; // operator/ is overloaded
			return fPath;
		}

		path FileSystemHelper::GetTempDir(const string& format)
		{
			return GetTempFile(format);
		}

		void FileSystemHelper::Remove(const path& p)
		{
			if (exists(p))
			{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
				remove(p);
#else
				std::string path = p.generic_string();
				const char* p_str = path.c_str();
				std::remove(p_str);
#endif
			}
		}

		void FileSystemHelper::Remove(const string& p)
		{
			Remove(path(p));
		}

		bool FileSystemHelper::Exists(const string& p)
		{
			return Exists(path(p));
		}

		bool FileSystemHelper::Exists(const path& p)
		{
			return boost::filesystem::exists(p);
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

			/*
			> a <- sncOpen('f:/STSF/Data/UnitTests/Fct_Data/Upper_Murray_F1_1_2010080121_shuffle.nc')
> a
File f:/STSF/Data/UnitTests/Fct_Data/Upper_Murray_F1_1_2010080121_shuffle.nc (NC_FORMAT_CLASSIC):

	 3 variables (excluding dimension variables):
		int station_id[station]
			long_name: station or node identification code
		char station_name[strLen,station]
			long_name: station or node name
		double rain_fcast_ens[lead_time,station,ens_member,time]
			standard_name: rain_fcast_ens
			long_name: Post processed ACCESS-G Rainfall Forecasts with Schaake Shuffle (Multiple Sites) Disaggregated from 3h
			units: mm
			_FillValue: -9999
			type: 2
			type_description: accumulated over the preceding interval
			Location_type: Area

	 5 dimensions:
		time  Size:1   *** is unlimited ***
			standard_name: time
			long_name: time
			time_standard: UTC
			axis: t
			units: hours since 2010-08-01 21:00:00 +0000
		lead_time  Size:231
			standard_name: lead time
			long_name: forecast lead time
			axis: v
			units: hours since time of forecast
		station  Size:3
		strLen  Size:30
		ens_member  Size:1000
			standard_name: ens_member
			long_name: ensemble member
			units: member id
			axis: u

	8 global attributes:
		title: Post Processed ACCESS-G (APS1) Rainfall Forecasts
		institution: CSIRO Land & Water
		source:
		catchment: Upper_Murray
		STF_convention_version: 1
		STF_nc_spec: https://wiki.csiro.au/display/wirada/NetCDF+for+SWIFT
		comment: Data extracted from:
'X:\Projects\WIRADA_4_1\Work\Activity_2\Testbed\Upper_Murray\Subareas\BJP_Calibration\F1\1h\Upper_Murray_F1_1_2010080121_shuffle.nc'
station_ID 7 in source file changed to station_ID 3 to compensate for catchment delineation error
		history: 2013-08-16 12:55:01 +10.0 - File created
 */

			TimeSeriesEnsembleTimeSeriesStore<double> * etss = new MultiFileTimeSeriesEnsembleTimeSeriesStore<double>(forecastDataFiles, rainFcastNcVarname, string("1"), 0,
				TimeStep::GetDaily(),
				// Upper_Murray_F1_1_2010080121_shuffle
				TimeStep::CreatePtime(2010, 8, 1),
				5, // to "Upper_Murray_F1_1_2010080521_shuffle.nc"
				1000, // int ensembleSize, 
				231, // int ensembleLength, 
				// const TimeStep& ensembleTimeStep
				TimeStep::GetHourly());
			dataLibrary.AddSource(rainFcastId, etss);

			return dataLibrary;
		}


		TestSingleTimeSeriesStore::TestSingleTimeSeriesStore(const vector<double>& values,
			const ptime& startDate, const TimeStep& timeStep)
		{
			innerTs = TimeSeries(values, startDate, timeStep);
		}

		TestSingleTimeSeriesStore::TestSingleTimeSeriesStore(const TimeSeries& series)
		{
			innerTs = series;
		}

		string TestSingleTimeSeriesStore::GetDataSummary() const
		{
			auto start = innerTs.GetStartDate();
			auto end = innerTs.GetEndDate();
			string result =
				string(", start: ") + to_iso_extended_string(start) +
				string(", end: ") + to_iso_extended_string(end) +
				string(", time step: ") + innerTs.GetTimeStep().GetName();
			return result;
		}

		TTimeSeries<double>* TestSingleTimeSeriesStore::Read()
		{
			return new TTimeSeries<double>(innerTs);
		}

		TTimeSeries<double>* TestSingleTimeSeriesStore::Read(const string& blah)
		{
			return new TTimeSeries<double>(innerTs);
		}
		std::vector<std::string> TestSingleTimeSeriesStore::GetIdentifiers() const { datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();  std::vector<std::string> x; return x; }


		TestTimeSeriesEnsembleTimeSeriesStore::~TestTimeSeriesEnsembleTimeSeriesStore() { delete ensFts; }
		TestTimeSeriesEnsembleTimeSeriesStore::TestTimeSeriesEnsembleTimeSeriesStore(const TSeriesEnsemblePtrType& ensFts, const string& id) {
			this->ensFts = new TSeriesEnsemblePtrType(ensFts, new SharedVectorStorage<EnsembleForecastTimeSeries<>::ElementType>());
			this->id = id;
		};

		TestTimeSeriesEnsembleTimeSeriesStore::TestTimeSeriesEnsembleTimeSeriesStore(const string& id) {
			this->ensFts = new EnsembleForecastTimeSeries<>(new SharedVectorStorage<EnsembleForecastTimeSeries<>::ElementType>());
			this->id = id;
		};

		TestTimeSeriesEnsembleTimeSeriesStore::PtrTSeriesEnsemblePtrType TestTimeSeriesEnsembleTimeSeriesStore::GetSeries(const string& dataId)
		{
			return new EnsembleForecastTimeSeries<>(this->ensFts);
		}

		TestTimeSeriesEnsembleTimeSeriesStore::PtrTSeriesEnsemblePtrType TestTimeSeriesEnsembleTimeSeriesStore::GetBackendSeries(const string& dataId)
		{
			return ensFts;
		}

		bool TestTimeSeriesEnsembleTimeSeriesStore::IsActive()
		{
			return (ensFts != nullptr);
		}

		TestTimeSeriesEnsembleTimeSeriesStore::PtrEnsemblePtrType TestTimeSeriesEnsembleTimeSeriesStore::Read(const std::string& ensembleIdentifier)
		{
			int i = boost::lexical_cast<int>(ensembleIdentifier);
			MultiTimeSeries<TTimeSeries<double>*>* blah = ensFts->GetValue(i);
			return blah;
		}
		size_t TestTimeSeriesEnsembleTimeSeriesStore::GetLength() const
		{
			return ensFts->GetLength();
		}
		ptime TestTimeSeriesEnsembleTimeSeriesStore::GetStart() const
		{
			return ensFts->GetStartDate();
		}
		//vector<string> TestTimeSeriesEnsembleTimeSeriesStore::GetItemIdentifiers() const
		//{
		//	vector<string> result(ensFts->GetLength());
		//	for (size_t i = 0; i < result.size(); i++)
		//	{
		//		result[i] = boost::lexical_cast<string>(i);
		//	}
		//	return result;
		//}
		string TestTimeSeriesEnsembleTimeSeriesStore::GetDataSummary() const
		{
			return "";
		}
		TimeStep TestTimeSeriesEnsembleTimeSeriesStore::GetTimeStep() const
		{
			return ensFts->GetTimeStep();
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::Allocate(size_t length, PtrEnsemblePtrType value)
		{
			vector<PtrEnsemblePtrType> v (length, value);
			ensFts->Reset(v, ensFts->GetStartDate());
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::AllocateValues(const vector<PtrEnsemblePtrType>& values)
		{
			ensFts->Reset(values, ensFts->GetStartDate());
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::SetSeries(const string& dataId, PtrTSeriesEnsemblePtrType value)
		{
			*ensFts = value;
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::SetItem(const string& dataId, size_t index, PtrEnsemblePtrType value)
		{
			ensFts->SetValue(index, value);
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::SetItem(const string& dataId, size_t index, const EnsemblePtrType& value)
		{
			EnsemblePtrType* nval = new EnsemblePtrType(value);
			ensFts->SetValue(index, nval);
		}

		//EnsemblePtrType Read(const string& ensembleIdentifier) = 0;
		void TestTimeSeriesEnsembleTimeSeriesStore::SetLength(size_t length)
		{
			datatypes::exceptions::ExceptionUtilities::ThrowNotSupported(" TestTimeSeriesEnsembleTimeSeriesStore", "SetLength(size_t length)");
		}

		void TestTimeSeriesEnsembleTimeSeriesStore::SetStart(ptime start)
		{
			ensFts->SetStartDate(start);
		}

		//vector<string> GetItemIdentifiers() const = 0;
		void TestTimeSeriesEnsembleTimeSeriesStore::SetTimeStep(const TimeStep& timeStep)
		{
			ensFts->SetTimeStep(timeStep);
		}

		TestTimeSeriesEnsembleTimeSeriesStore::PtrEnsemblePtrType TestTimeSeriesEnsembleTimeSeriesStore::GetItem(const string& dataId, size_t fcastIndex)
		{
			return ensFts->GetValue(fcastIndex);
		}

		TestTimeSeriesEnsembleTimeSeriesStore::PtrSeriesType TestTimeSeriesEnsembleTimeSeriesStore::GetItem(const string& dataId, size_t fcastIndex, size_t ensIndex)
		{
			return new SeriesType(ensFts->GetValue(fcastIndex)->Get(ensIndex));
		}

		size_t TestTimeSeriesEnsembleTimeSeriesStore::GetEnsembleSize(const string& dataId, size_t fcastIndex) const
		{
			return ensFts->GetValue(fcastIndex)->Size();
		}

		TestEnsembleTimeSeriesStore::TestEnsembleTimeSeriesStore(const MultiTimeSeries<>& data)
		{
			this->data = data;
		}

		MultiTimeSeries<TTimeSeries<double>*>* TestEnsembleTimeSeriesStore::Read()
		{
			return data.AsPointerSeries();
		}
		string TestEnsembleTimeSeriesStore::GetDataSummary() const
		{
			return "";
		}

		TestTimeSeriesStoreFactory::TestTimeSeriesStoreFactory() {}

		TestTimeSeriesStoreFactory::~TestTimeSeriesStoreFactory() {}

		TimeSeriesEnsembleTimeSeriesStore<double>* TestTimeSeriesStoreFactory::CreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId)
		{
			return new datatypes::tests::TestTimeSeriesEnsembleTimeSeriesStore(dataId);
		}

		bool TestTimeSeriesStoreFactory::CanCreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId)
		{
			return true;
		}

	}
}