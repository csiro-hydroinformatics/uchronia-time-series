#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>    

#include "common.h"

namespace datatypes
{
	namespace utils
	{
		using namespace std;
		bad_lexical_cast::bad_lexical_cast(const string& msg) 
			: invalid_argument(msg.c_str())
		{
		}
		
		vector<string> StringProcessing::Split(const string& s, const string& separators)
		{
			vector<string> tokens;
			boost::split(tokens, s, boost::is_any_of(separators));
			return tokens;
		}

		string StringProcessing::TrimAny(const string& s, const string& charactersToTrim)
		{
			string result(s);
			boost::trim_if(result, boost::is_any_of(charactersToTrim));
			return result;
		}

		vector<string> StringProcessing::SplitOnSpaces(const string& s, bool removeEmptyEntries)
		{
			vector<string> split = Split(s, " \t");
			if (removeEmptyEntries)
				split = RemoveEmpty(split);
			return split;
		}

		vector<string> StringProcessing::Concatenate(const vector<vector<string>>& vars)
		{
			vector<string> res;
			for (const auto& v : vars)
			{
				Concatenate(res, v);
			}
			return res;
		}
		void StringProcessing::Concatenate(vector<string>& a, const vector<string>& b)
		{
			size_t offset = a.size();
			a.resize(a.size() + b.size());
			std::copy(b.begin(), b.end(), a.begin() + offset);
		}

		vector<string> StringProcessing::RemoveEmpty(const vector<string>& s)
		{
			vector<string> res;
			string empty = string("");
			for (int i = 0; i < s.size(); i++)
				if (s[i] != empty)
					res.push_back(s[i]);
			return res;
		}

		string StringProcessing::Paste(const vector<string>& items, const string& sep)
		{
			string res;
			for (size_t i = 0; i < items.size() - 1; i++)
			{
				res += items[i];
				res += sep;
			}
			res += items[items.size() - 1];
			return res;
		}

		vector<string> StringProcessing::Paste(const string& prefix, const vector<string>& postfixes)
		{
			vector<string> result(postfixes.size());
			for (size_t i = 0; i < postfixes.size(); i++)
				result[i] = prefix + postfixes[i];
			return result;
		}

		bool StringProcessing::StringPredicate(const string& toTest, const string& toMatch, boost::function<bool(const string&, const string&)> predicate, bool caseSensitive)
		{
			using namespace boost::algorithm;
			string t(toTest);
			string m(toMatch);

			if (!caseSensitive)
			{
				to_lower(t);
				to_lower(m);
			} return (predicate(t, m));
		}

		bool StringProcessing::ContainsAll(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive)
		{
			for (auto& t : toTest)
				if (!EqualsAny(t, toMatch, caseSensitive))
					return false;
			return true;
		}

		vector<string> StringProcessing::Unique(const vector<string>& set)
		{
			vector<string> res;
			for (auto& t : set)
				if (!EqualsAny(t, res))
					res.push_back(t);
			return set;
		}

		bool StringProcessing::SetEquals(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive)
		{
			for (auto& t : toTest)
				if (!EqualsAny(t, toMatch, caseSensitive))
					return false;
			for (auto& t : toMatch)
				if (!EqualsAny(t, toTest, caseSensitive))
					return false;
			return true;
		}

		vector<string> StringProcessing::SetDiff(const vector<string>& toTest, const vector<string>& toRemove, bool caseSensitive)
		{
			vector<string> result;
			for (auto& t : toTest)
				if (!EqualsAny(t, toRemove, caseSensitive))
					result.push_back(t);
			return result;
		}

		bool StringProcessing::Contains(const string& toTest, const string& toMatch, bool caseSensitive)
		{
			return StringPredicate(toTest, toMatch, funContains, caseSensitive);
		}

		bool StringProcessing::EqualsAny(const string& toTest, const vector<string>& toMatch, bool caseSensitive)
		{
			for (auto& m : toMatch)
				if (Equals(toTest, m, caseSensitive))
					return true;
			return false;
		}

		bool StringProcessing::StartsWith(const string& toTest, const string& toMatch, bool caseSensitive)
		{
			return StringPredicate(toTest, toMatch, funStartsWith, caseSensitive);
		}

		bool StringProcessing::Equals(const string& toTest, const string& toMatch, bool caseSensitive)
		{
			return StringPredicate(toTest, toMatch, funEquals, caseSensitive);
		}

		bool StringProcessing::funContains(const string& toTest, const string& toMatch) { return boost::algorithm::contains(toTest, toMatch); }
		bool StringProcessing::funStartsWith(const string& toTest, const string& toMatch) { return boost::algorithm::starts_with(toTest, toMatch); }
		bool StringProcessing::funEquals(const string& toTest, const string& toMatch) { return boost::algorithm::equals(toTest, toMatch); }

		string StringProcessing::BuildIdentifier(vector<string> &tokens, int fromIndex, int toIndex, const string& sep)
		{
			if (toIndex < 0)
				toIndex = tokens.size();
			vector<string>::const_iterator first = tokens.begin() + fromIndex;
			vector<string>::const_iterator last = tokens.begin() + toIndex;
			vector<string> newVec(first, last);
			string result = boost::join(newVec, sep);
			return result;
		}

		string StringProcessing::BuildIdentifier(const string &a, const string &b, const string& sep)
		{
			return a + sep + b;
		}

		const string StringProcessing::kElementSeparatorToken = ".";



	}

	namespace interop
	{
		std::atomic<double > MissingValueHandling::TimeSeriesMissingValueValue(DEFAULT_MISSING_DATA_VALUE);
	}

}

// Provide an implementation for c_cpp_interop.cpp
void delete_ansi_string_array(char** values, int arrayLength)
{
	if (values == nullptr)
		throw std::logic_error("delete_ansi_string_array: values cannot be a nullptr");
	for (int i = 0; i < arrayLength; i++)
		delete[] values[i];
	delete[] values;
}

void delete_array(double* values) {
	if (values == nullptr)
		throw std::logic_error("delete_array: value cannot be a nullptr");
	delete[] values;
}

