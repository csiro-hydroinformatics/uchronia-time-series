#pragma once
#include <string> 
#include <stdexcept> 
#include <boost/filesystem.hpp>
#include "common.h"

using std::string;

namespace datatypes
{
	namespace io
	{
		class DATATYPES_DLL_LIB IoHelper
		{
		public:
			static const string DefaultFilePattern;
			static bool FileExists(const boost::filesystem::path& p);
			static bool PathExists(const boost::filesystem::path& p);
			static bool DirExists(const boost::filesystem::path& p);
			static string MakeFileName(const string& fileNamePattern, const string& id, const string& pattern = "{0}");
			static bool IsFileNamePattern(const string& s);
		};
	}
}
