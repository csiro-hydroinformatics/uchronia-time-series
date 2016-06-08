#include <string>
#include <map>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "exception_utilities.h"
#include "time_series_store.hpp"
#include "time_series_io.hpp"

namespace datatypes
{
	namespace timeseries
	{


		
		void IdentifiersProvider::CheckNotEmpty(const string& longId)
		{
			if (longId.empty())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("A data identifier cannot be an empty string");
		}

		vector<string> IdentifiersProvider::SplitHierarchicalIdentifier(const string& longId)
		{
			vector<string> tokens;
			boost::split(tokens, longId, boost::is_any_of(string(".")));
			return tokens;
		}

		string IdentifiersProvider::GetTopmostIdentifier(const string& longId)
		{
			if (longId == "") return "";
			vector<string> tokens = SplitHierarchicalIdentifier(longId);
			if (tokens.size() == 0) return "";
			else return tokens[0];
		}

		vector<string> TimeSeriesLibraryDescription::GetDataIdSingle() const
		{
			vector<string> ids;
			for (auto& x : singleProviders) {
				ids.push_back(string(x.first));
			}
			return ids;
		}

		vector<string> TimeSeriesLibraryDescription::GetDataIdEnsembleTs() const
		{
			vector<string> ids;
			for (auto& x : ensProviders) {
				ids.push_back(string(x.first));
			}
			return ids;
		}

		vector<string> TimeSeriesLibraryDescription::GetDataIdTsEnsembleTs() const
		{
			vector<string> ids;
			for (auto& x : tsEnsProviders) {
				ids.push_back(string(x.first));
			}
			return ids;
		}

		TimeSeriesSourceInfo TimeSeriesLibraryDescription::GetInfo(const string& dataId, bool applyRootDir) const
		{
			string id = IdentifiersProvider::GetTopmostIdentifier(dataId);
			TimeSeriesSourceInfo result;
			if (singleProviders.find(id) != singleProviders.end())
				result = singleProviders.at(string(id));
			else if (ensProviders.find(id) != ensProviders.end())
				result = ensProviders.at(string(id));
			else if (tsEnsProviders.find(id) != tsEnsProviders.end())
				result = tsEnsProviders.at(string(id));
			else
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("Data Id not found: " + id));
			if (applyRootDir)
				result.ApplyRootDir(this->GetRootDirPath().generic_string());
			return result;
		}

		void TimeSeriesLibraryDescription::SetLoadedFileName(const string& fileName)
		{
			filenameLoadedFrom = fileName;
		}

		std::map<string, string> TimeSeriesLibraryDescription::GetSerializableConfiguration(const string& dataId) const
		{
			return this->GetInfo(dataId).GetSerializableConfiguration();
		}

		boost::filesystem::path TimeSeriesLibraryDescription::GetRootDirPath() const
		{
			boost::filesystem::path p(filenameLoadedFrom);
			return p.parent_path();
		}

		SingleTimeSeriesStore<double>* TimeSeriesLibraryDescription::CreateSingleTimeSeriesStore(const string& dataId) const
		{
			if (singleProviders.find(dataId) == singleProviders.end())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("Data Id not found for single time series store: " + dataId));
			auto tsi = GetInfo(dataId, true);
			return tsi.CreateSingleTimeSeriesStore();
		}

		EnsembleTimeSeriesStore<double>* TimeSeriesLibraryDescription::CreateEnsembleTimeSeriesStore(const string& dataId) const
		{
			if (ensProviders.find(dataId) == ensProviders.end())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("Data Id not found for ensemble store: " + dataId));
			auto tsi = GetInfo(dataId, true);
			return tsi.CreateEnsembleTimeSeriesStore();
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesLibraryDescription::CreateTimeSeriesEnsembleTimeSeriesStore(const string& dataId) const
		{
			if (tsEnsProviders.find(dataId) == tsEnsProviders.end())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("Data Id not found for time series of ensembles store: " + dataId));
			auto tsi = GetInfo(dataId, true);
			return tsi.CreateTimeSeriesEnsembleTimeSeriesStore();
		}

		SingleTimeSeriesStore<double>* TimeSeriesSourceInfo::CreateSingleTimeSeriesStore() const
		{
			return impl->CreateSingleTimeSeriesStore();
		}

		EnsembleTimeSeriesStore<double>* TimeSeriesSourceInfo::CreateEnsembleTimeSeriesStore() const
		{
			return impl->CreateEnsembleTimeSeriesStore();
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesSourceInfo::CreateTimeSeriesEnsembleTimeSeriesStore() const
		{
			return impl->CreateTimeSeriesEnsembleTimeSeriesStore();
		}

		std::map<string, string> TimeSeriesSourceInfo::GetSerializableConfiguration() const
		{
			return impl->GetSerializableConfiguration();
		}


		SingleTimeSeriesStore<double>* TimeSeriesSourceInfoImpl::CreateSingleTimeSeriesStore() const
		{
			ExceptionUtilities::ThrowNotSupported(typeid(this).name(), "CreateSingleTimeSeriesStore");
			return nullptr;
		}

		EnsembleTimeSeriesStore<double>* TimeSeriesSourceInfoImpl::CreateEnsembleTimeSeriesStore() const
		{
			ExceptionUtilities::ThrowNotSupported(typeid(this).name(), "CreateEnsembleTimeSeriesStore");
			return nullptr;
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesSourceInfoImpl::CreateTimeSeriesEnsembleTimeSeriesStore() const
		{
			ExceptionUtilities::ThrowNotSupported(typeid(this).name(), "CreateTimeSeriesEnsembleTimeSeriesStore");
			return nullptr;
		}

		std::map<string, string> TimeSeriesSourceInfoImpl::GetSerializableConfiguration() const
		{
			ExceptionUtilities::ThrowNotSupported(typeid(this).name(), "GetSerializableConfiguration");
			return std::map<string,string>();
		}

		SingleTimeSeriesStore<double>* NetCdfSourceInfo::CreateSingleTimeSeriesStore() const
		{
			return TimeSeriesLibraryFactory::CreateTsSource(this->fileName, ncVarName, identifier);
		}

		EnsembleTimeSeriesStore<double>* NetCdfSourceInfo::CreateEnsembleTimeSeriesStore() const
		{
			return TimeSeriesLibraryFactory::CreateEnsTsSource(this->fileName, ncVarName, identifier);
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* NetCdfSourceInfo::CreateTimeSeriesEnsembleTimeSeriesStore() const
		{
			if (storageType == TimeSeriesLibrary::StorageTypeSingleNetcdfFile)
				return TimeSeriesLibraryFactory::CreateTsEnsTsSource(this->fileName, ncVarName, identifier);
			else if (storageType == TimeSeriesLibrary::StorageTypeMultipleNetcdfFiles)
				return new MultiFileTimeSeriesEnsembleTimeSeriesStore<double>(fileName, ncVarName, identifier, index);
			else
			{
				datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented("Unknown storage type: " + storageType);
				return nullptr;
			}
		}

		std::map<string, string> NetCdfSourceInfo::GetSerializableConfiguration() const
		{
		}
		

		//string TimeSeriesLibraryDescription::GetFilename(const string& dataId) const
		//{
		//	using boost::filesystem::path;
		//	string fName = GetInfo(dataId, false).FileName();
		//	path p(fName);
		//	if (p.is_absolute())
		//		return fName;
		//	else
		//	{
		//		path fullPath = GetRootDirPath() / p;
		//		return fullPath.generic_string();
		//	}
		//}

		//string TimeSeriesLibraryDescription::GetFileVariableName(const string& dataId) const
		//{
		//	return GetInfo(dataId).NcVarName();
		//}

		//string TimeSeriesLibraryDescription::GetIdentifier(const string& dataId) const
		//{
		//	return GetInfo(dataId).Identifier();
		//}

		//int TimeSeriesLibraryDescription::GetIndex(const string& dataId) const
		//{
		//	return GetInfo(dataId).Index();
		//}

		NetCdfSourceInfo::NetCdfSourceInfo(const string& dataId, const string& fileName, const string& ncVarName, const string& identifier,
			int index, const string& storageType, const string& timeStep, const string& start, int length,
			int ensembleSize, int ensembleLength, const string& ensembleTimeStep/*, const string& collectionDimensionId*/)
		{
			this->dataId = dataId;
			this->fileName = fileName;
			this->ncVarName = ncVarName;
			this->index = index;
			this->storageType = storageType;
			this->identifier = identifier;
			//this->collectionDimensionId = collectionDimensionId;
		}


		void TimeSeriesLibraryDescription::AddSingle(const string& dataId, const TimeSeriesSourceInfo& t)
		{
			singleProviders[dataId] = t;
		}

		void TimeSeriesLibraryDescription::AddEnsembleTs(const string& dataId, const TimeSeriesSourceInfo& t)
		{
			ensProviders[dataId] = t;
		}

		void TimeSeriesLibraryDescription::AddTsEnsembleTs(const string& dataId, const TimeSeriesSourceInfo& t)
		{
			tsEnsProviders[dataId] = t;
		}

		TimeSeriesLibrary::TimeSeriesLibrary() {}

		TimeSeriesLibrary::TimeSeriesLibrary(const TimeSeriesLibraryDescription& description)
		{
			auto singleTs = description.GetDataIdSingle();
			auto ensTs = description.GetDataIdEnsembleTs();
			auto tsEnsTs = description.GetDataIdTsEnsembleTs();
			for (auto& id : singleTs)
				AddSource(id, (description.GetInfo(id, true).CreateSingleTimeSeriesStore()));
			for (auto& id : ensTs)
				AddSource(id, (description.GetInfo(id, true).CreateEnsembleTimeSeriesStore()));
			for (auto& id : tsEnsTs)
				AddSource(id, (description.GetInfo(id, true).CreateTimeSeriesEnsembleTimeSeriesStore()));
		}

		TimeSeriesLibrary::~TimeSeriesLibrary()
		{
			for (auto& store : tsEnsTimeSeriesProviders)
				delete store.second;
			for (auto& store : ensTimeSeriesProviders)
				delete store.second;
			for (auto& store : timeSeriesProviders)
				delete store.second;

			tsEnsTimeSeriesProviders.clear();
			ensTimeSeriesProviders.clear();
			timeSeriesProviders.clear();
		}

		TimeSeriesLibrary& TimeSeriesLibrary::operator=(TimeSeriesLibrary&& src)
		{
			// Avoid self assignment
			if (&src == this) {
				return *this;
			}
			std::swap(tsEnsTimeSeriesProviders, src.tsEnsTimeSeriesProviders);
			std::swap(ensTimeSeriesProviders, src.ensTimeSeriesProviders);
			std::swap(timeSeriesProviders, src.timeSeriesProviders);
			return *this;
		}

		TimeSeriesLibrary::TimeSeriesLibrary(TimeSeriesLibrary&& src)
		{
			*this = std::move(src);
		}

		const string TimeSeriesLibrary::StorageTypeSingleNetcdfFile = "single_nc_file";
		const string TimeSeriesLibrary::StorageTypeMultipleNetcdfFiles = "multiple_nc_files_filename_date_pattern";

		TTimeSeries<double>* TimeSeriesLibrary::GetSingle(const string& dataId, boost::function<TTimeSeries<double>*(TTimeSeries<double>*)>& tsTransform)
		{
			auto rawTs = GetSingle(dataId);
			return tsTransform(rawTs);
		}

		vector<string> TimeSeriesLibrary::GetIdentifiers() const
		{
			vector<string> result;
			for (auto& x : timeSeriesProviders)
				result.push_back(x.first);
			for (auto& x : ensTimeSeriesProviders)
				result.push_back(x.first);
			for (auto& x : tsEnsTimeSeriesProviders)
				result.push_back(x.first);
			return result;
		}

		vector<string> TimeSeriesLibrary::GetIdentifiers(const string& dataId) const
		{
			auto p = const_cast<TimeSeriesLibrary*>(this);
			if (hasKey(timeSeriesProviders, dataId))
				return p->GetSeriesInformation(dataId)->GetIdentifiers();
			if (hasKey(ensTimeSeriesProviders, dataId))
				return p->GetEnsembleSeriesInformation(dataId)->GetIdentifiers();
			else if (hasKey(tsEnsTimeSeriesProviders, dataId))
				return p->GetTimeSeriesEnsembleSeriesInformation(dataId)->GetIdentifiers();
			else
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data identifier ") + dataId + "not found");
		}

		string TimeSeriesLibrary::GetDataSummary(const string& dataId)
		{
			if (hasKey(timeSeriesProviders, dataId))
				return GetSeriesInformation(dataId)->GetDataSummary();
			if (hasKey(ensTimeSeriesProviders, dataId))
				return GetEnsembleSeriesInformation(dataId)->GetDataSummary();
			else if (hasKey(tsEnsTimeSeriesProviders, dataId))
				return GetTimeSeriesEnsembleSeriesInformation(dataId)->GetDataSummary();
			else
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data identifier ") + dataId + "not found");
		}

		TTimeSeries<double>* TimeSeriesLibrary::GetSingle(const string& dataId)
		{
			IdentifiersProvider::CheckNotEmpty(dataId);
			vector<string> ids = IdentifiersProvider::SplitHierarchicalIdentifier(dataId);
			if (ids.size() == 1)
				return GetSeriesInformation(dataId)->Read();
			else
				return GetSingle(ids[0], ids[1]);
		}

		//TimeSeriesProvider<double>* TimeSeriesLibrary::GetProvider(const string& dataId)
		//{
		//	this->GetSeriesInformation(dataId)->
		//	ExceptionUtilities::ThrowNotImplemented();
		//	return nullptr;
		//}

		TTimeSeries<double>* TimeSeriesLibrary::GetSingle(const string& dataId, const string& collectionIdentifier)
		{
			return GetSeriesInformation(dataId)->Read(collectionIdentifier);
		}

		MultiTimeSeries<TTimeSeries<double>*>* TimeSeriesLibrary::GetEnsemble(const string& dataId, const string& dataItemIdentifier)
		{
			TimeSeriesEnsembleTimeSeriesStore<double>* dataSource = this->tsEnsTimeSeriesProviders[dataId];
			if (dataSource == nullptr)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data source for data identifier ") + dataId + "not found");
			auto rawTs = dataSource->Read(dataItemIdentifier);
			return rawTs;
		}

		MultiTimeSeries<TTimeSeries<double>*>* TimeSeriesLibrary::GetEnsembleTimeSeries(const string& dataId)
		{
			EnsembleTimeSeriesStore<double>* dataSource = GetEnsembleSeriesInformation(dataId);
			return dataSource->Read();
		}

		EnsembleForecastTimeSeries<TTimeSeries<double>>* TimeSeriesLibrary::GetTimeSeriesEnsembleTimeSeries(const string& dataId)
		{
			TimeSeriesEnsembleTimeSeriesStore<double>* dataSource = this->GetTimeSeriesEnsembleSeriesInformation(dataId);

			size_t dataLength = dataSource->GetLength();
			ptime start = dataSource->GetStart();
			TimeStep step = dataSource->GetTimeStep();
			EnsembleForecastTimeSeries<TimeSeries>* result = new EnsembleForecastTimeSeries<TimeSeries>(dataLength, start, step);

			vector<string> dataItemIdentifiers = dataSource->GetItemIdentifiers();
			int i = 0;
			for (auto& dataItemIdentifier : dataItemIdentifiers)
			{
				auto rawTs = dataSource->Read(dataItemIdentifier);
				result->SetValue(i, rawTs);
				i++;
			}

			return result;
		}

		void TimeSeriesLibrary::AddSource(const string& dataId, SingleTimeSeriesStore<double>* store)
		{
			timeSeriesProviders[dataId] = store;
		}

		void TimeSeriesLibrary::AddSource(const string& dataId, EnsembleTimeSeriesStore<double>* store)
		{
			ensTimeSeriesProviders[dataId] = store;
		}

		void TimeSeriesLibrary::AddSource(const string& dataId, TimeSeriesEnsembleTimeSeriesStore<double>* dataAccess)
		{
			tsEnsTimeSeriesProviders[dataId] = dataAccess;
		}

		SingleTimeSeriesStore<double> * TimeSeriesLibrary::GetSeriesInformation(const string& dataId)
		{
			string id = IdentifiersProvider::GetTopmostIdentifier(dataId);
			if (!hasKey(timeSeriesProviders, id))
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data information not found for id ") + id);
			SingleTimeSeriesStore<double> * dataAccess = timeSeriesProviders[id];
			return dataAccess;
		}

		EnsembleTimeSeriesStore<double> * TimeSeriesLibrary::GetEnsembleSeriesInformation(const string& dataId)
		{
			string id = IdentifiersProvider::GetTopmostIdentifier(dataId);
			if (!hasKey(ensTimeSeriesProviders, id))
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data information not found for id ") + id);
			EnsembleTimeSeriesStore<double> * dataAccess = ensTimeSeriesProviders[id];
			return dataAccess;
		}

		TimeSeriesEnsembleTimeSeriesStore<double> * TimeSeriesLibrary::GetTimeSeriesEnsembleSeriesInformation(const string& dataId)
		{
			string id = IdentifiersProvider::GetTopmostIdentifier(dataId);
			if (!hasKey(tsEnsTimeSeriesProviders, id))
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("data information not found for id ") + id);
			TimeSeriesEnsembleTimeSeriesStore<double> * dataAccess = tsEnsTimeSeriesProviders[id];
			return dataAccess;
		}

		SingleTimeSeriesStore<double>* TimeSeriesLibrary::CreateTsSource(const TimeSeriesSourceInfo& desc)
		{
			return desc.CreateSingleTimeSeriesStore();
			//SwiftNetCDFTimeSeriesStore<>* access = new SwiftNetCDFTimeSeriesStore<>(desc.fileName);
			//// TODO: check the consistency of access and desc
			//return new NetCdfSingleSeriesStore<double>(access, desc.ncVarName, desc.identifier);
		}

		EnsembleTimeSeriesStore<double>* TimeSeriesLibrary::CreateEnsTsSource(const TimeSeriesSourceInfo& desc)
		{
			return desc.CreateEnsembleTimeSeriesStore();
			//SwiftNetCDFTimeSeriesStore<>* access = new SwiftNetCDFTimeSeriesStore<>(desc.fileName);
			//// TODO: check the consistency of access and desc
			//return new NetCdfEnsembleTimeSeriesStore<double>(access, desc.ncVarName, desc.identifier);
		}

		TimeSeriesEnsembleTimeSeriesStore<double>* TimeSeriesLibrary::CreateTsEnsTsSource(const TimeSeriesSourceInfo& desc)
		{
			return desc.CreateTimeSeriesEnsembleTimeSeriesStore();
			//if (desc.storageType == StorageTypeSingleNetcdfFile)
			//{
			//	SwiftNetCDFTimeSeriesStore<>* access = new SwiftNetCDFTimeSeriesStore<>(desc.fileName);
			//	// TODO: check the consistency of access and desc
			//	return new NetCdfTimeSeriesEnsembleTimeSeriesStore<double>(access, desc.ncVarName, desc.identifier);
			//}
			//else if (desc.storageType == StorageTypeMultipleNetcdfFiles)
			//{
			//	TimeSeriesEnsembleTimeSeriesStore<double> * etss = new MultiFileTimeSeriesEnsembleTimeSeriesStore<double>(
			//		desc.fileName,
			//		desc.ncVarName,
			//		desc.identifier,
			//		desc.index);
			//	return etss;
			//}
			//else
			//	datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented("unknown storage type: " + desc.storageType);
		}


		TimeSeriesSourceInfoImpl::TimeSeriesSourceInfoImpl() {}
		TimeSeriesSourceInfoImpl::TimeSeriesSourceInfoImpl(const TimeSeriesSourceInfoImpl& src) {}
		TimeSeriesSourceInfoImpl::~TimeSeriesSourceInfoImpl() {}

		using ExceptionUtilities = datatypes::exceptions::ExceptionUtilities;
		TimeSeriesSourceInfo::TimeSeriesSourceInfo(const TimeSeriesSourceInfoImpl& t)
		{
			impl = t.Clone();
		}
		TimeSeriesSourceInfo::TimeSeriesSourceInfo(const TimeSeriesSourceInfo& src) {
			impl = src.impl->Clone();
		}
		TimeSeriesSourceInfo::TimeSeriesSourceInfo() {}
		TimeSeriesSourceInfo::~TimeSeriesSourceInfo()
		{
			if (impl != nullptr)
				delete impl;
		}

		TimeSeriesSourceInfo::TimeSeriesSourceInfo(TimeSeriesSourceInfo&& src)
		{
			if (impl != nullptr)
				delete impl;
			impl = src.impl;
			src.impl = nullptr;
		}

		TimeSeriesSourceInfo& TimeSeriesSourceInfo::operator= (const TimeSeriesSourceInfo& src)
		{
			if (&src == this) {
				return *this;
			}
			impl = src.impl->Clone();
			return *this;
		}

		TimeSeriesSourceInfo& TimeSeriesSourceInfo::operator= (TimeSeriesSourceInfo&& src)
		{
			if (&src == this) {
				return *this;
			}
			if (impl != nullptr)
				delete impl;
			impl = src.impl;
			src.impl = nullptr;
			return *this;
		}

		//void TimeSeriesSourceInfo::ApplyRootDir(const string& rootDir);
		//string TimeSeriesSourceInfo::DataId() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//string TimeSeriesSourceInfo::Identifier() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//string TimeSeriesSourceInfo::StorageType() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//int TimeSeriesSourceInfo::Index() { ExceptionUtilities::ThrowNotImplemented(); return -1; }

		//string TimeSeriesSourceInfo::TimeStep() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//string TimeSeriesSourceInfo::Start() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//int TimeSeriesSourceInfo::Length() { ExceptionUtilities::ThrowNotImplemented(); return -1; }
		//int TimeSeriesSourceInfo::EnsembleSize() { ExceptionUtilities::ThrowNotImplemented(); return -1; }
		//int TimeSeriesSourceInfo::EnsembleLength() { ExceptionUtilities::ThrowNotImplemented(); return -1; }
		//string TimeSeriesSourceInfo::EnsembleTimeStep() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//string TimeSeriesSourceInfo::FileName() { ExceptionUtilities::ThrowNotImplemented(); return ""; }
		//string TimeSeriesSourceInfo::NcVarName() { ExceptionUtilities::ThrowNotImplemented(); return ""; }

		TimeSeriesSourceInfoImpl * NetCdfSourceInfo::Clone() const
		{
			return new NetCdfSourceInfo(*this);
		}

		void TimeSeriesSourceInfo::ApplyRootDir(const string& rootDir)
		{
			impl->ApplyRootDir(rootDir);
		}

		void TimeSeriesSourceInfoImpl::ApplyRootDir(const string& rootDir)
		{
			ExceptionUtilities::ThrowNotImplemented();
		}

		void NetCdfSourceInfo::ApplyRootDir(const string& rootDir)
		{
			boost::filesystem::path r(rootDir);
			boost::filesystem::path f(fileName);
			fileName = (r / f).generic_string();
		}

	}
}
