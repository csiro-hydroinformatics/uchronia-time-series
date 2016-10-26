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

	}
}
