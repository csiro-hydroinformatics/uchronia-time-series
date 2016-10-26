#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>    
#include "io_helper.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

namespace datatypes
{
	namespace io
	{
		
		bool IoHelper::FileExists(const boost::filesystem::path& p)
		{
			return (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p));
		}
		bool IoHelper::PathExists(const boost::filesystem::path& p)
		{
			return (boost::filesystem::exists(p));
		}
		bool IoHelper::DirExists(const boost::filesystem::path& p)
		{
			return (boost::filesystem::exists(p) && boost::filesystem::is_directory(p));
		}

		string IoHelper::MakeFileName(const string& fileNamePattern, const string& id, const string& pattern)
		{
			string fileName(fileNamePattern);
			boost::algorithm::replace_first(fileName, pattern, id);
			return fileName;
		}

		const string IoHelper::DefaultFilePattern = "{0}";

		bool IoHelper::IsFileNamePattern(const string& s)
		{
			return boost::algorithm::contains(s, DefaultFilePattern);
		}
	}
}
