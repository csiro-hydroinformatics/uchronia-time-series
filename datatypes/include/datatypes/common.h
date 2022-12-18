#pragma once
#include <stdio.h>
#include <atomic>
#include <string> 
#include <vector>
#include <map> 
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <datatypes/setup_exports.h>

#ifdef _MSC_VER
#  pragma warning(push)
// class blah needs to have dll-interface to be used by clients of class blah
#  pragma warning(disable : 4251 4231 4660 4275)
#endif

#define DEFAULT_MISSING_DATA_VALUE -9999.0
#define DEFAULT_STATION_IDENTIFIER { "0" }

// You can define the following to activate the Visual Leak Detector tool for visual C++
// https://vld.codeplex.com
//#define DATATYPES_USE_VLD

#ifdef DATATYPES_USE_VLD
  #include <vld.h>
#endif

#ifdef _WIN32
  #define STRDUP _strdup
#else
  #define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

using std::string;
using std::vector;
using std::map;
using std::pair;

using namespace boost::posix_time;
using namespace boost::gregorian;

#define DATATYPES_DOUBLE_PRECISION_ID "double"
#define DATATYPES_SINGLE_PRECISION_ID "single"

#define DATATYPES_FOUR_DIMENSIONS_DATA "4"
#define DATATYPES_THREE_DIMENSIONS_DATA "3"
#define DATATYPES_TWO_DIMENSIONS_DATA "2"

#define SWIFT_TIME_UNIT_MINUTE string("minute")
#define SWIFT_TIME_UNIT_HOUR string("hour")
#define SWIFT_TIME_UNIT_DAY string("day")
#define SWIFT_TIME_UNIT_MONTH string("month")

// macros for strings used as structuring identifiers or keys in std::map

#define FILE_KEY       "File"
#define ID_DATA_KEY       "Id"
#define VAR_KEY        "Var"
#define IDENTIFIER_KEY "Identifier"
#define INDEX_KEY      "Index"
#define TYPE_KEY       "Type"

/**
* \brief	A string identifier for a multivariate time series (structurally an ensemble, but not semantically, e.g. TS for multiple sites)
*/
#define SINGLE_SERIES_COLLECTION_TYPE_ID       "single_collection"

/**
* \brief	A string identifier for a single univariate time series
*/
#define SINGLE_SERIES_TYPE_ID                  "single"

/**
* \brief	A string identifier for ensemble of time series (e.g. ensemble forecasts for one lead time)
*/
#define ENSEMBLE_SERIES_TYPE_ID                "ensemble_ts"

/**
* \brief	A string identifier for time series of ensemble of time series (e.g. ensemble forecasts time series)
*/
#define TIME_SERIES_ENSEMBLE_SERIES_TYPE_ID    "ts_ensemble_ts"

/**
 * \brief	A string identifier for time series of collection of time series (e.g. forecasts for multiple sites)
 */
#define TIME_SERIES_SERIES_COLLECTION_TYPE_ID  "ts_ts_collection"

/**
 * \brief	A string identifier for time series stored in a single netcdf file
 */
#define STORAGE_TYPE_SINGLE_NETCDF_FILE        "single_nc_file"

/**
 * \brief	A string identifier for time series stored in a multiple netcdf files with a date pattern
 */
#define STORAGE_TYPE_MULTIPLE_NETCDF_FILES     "multiple_nc_files_filename_date_pattern"


namespace datatypes
{
	namespace utils
	{

		void datatypes_delete_ansi_string_array(char** values, int arrayLength);

		/**
		 * \brief Helper functions with features found in other languages but not found in the C++ standard template library. 
		 * Many of these features are not used in this library (uchronia) as such, but are here as a place of 
		 * convenience for dependent modelling libraries.
		 */
		class STLHelper
		{
		private:
			STLHelper();

			template <typename U>
			class Comparer
			{
				std::function<bool(const U&, const U&)>& valueCompare;
			public:
				Comparer(std::function<bool(const U&, const U&)>& valueCompare) : valueCompare(valueCompare)
				{
				}
				typedef typename vector<U>::const_iterator const_iterator;
				bool operator()(
					const std::pair<size_t, const_iterator>& a,
					const std::pair<size_t, const_iterator>& b)
				{
					return valueCompare(*(a.second), *(b.second));
				}
			};


		public:
			/**
			 * \brief is a key present in a dictionary (map)
			 */
			template<typename K = string, typename V = string>
			static bool HasKey(const map<K, V>& dict, const string& key)
			{
				return(dict.find(key) != dict.end());
			}
			/**
			 * \brief Gets the keys present in a dictionary (map)
			 */
			template<typename K = string, typename V = string>
			static vector<K> GetKeys(const map<K, V>& dict)
			{
				vector<K> res;
				res.reserve(dict.size());
				for (auto& kvpair : dict)
					res.push_back(kvpair.first);
				return res;
			}

			/**
			 * \brief Change the keys of a dictionary with new keys, remaping
			 * 
			 * \tparam K key type
			 * \tparam V value type
			 * \param dict 
			 * \param newKeys 
			 * \return map<K, V> 
			 */
			template<typename K = string, typename V = string>
			static map<K, V> Remap(const map<K, V>& dict, const map<K, K>& newKeys)
			{
				map<K, V> res;
				for (const auto& kvpair : dict)
				{
					K newKey = newKeys.at(kvpair.first);
					res[newKey] = kvpair.second;
				}
				return res;
			}

			template<typename K = string, typename V = string>
			static map<K, V> Zip(const vector<K>& key, const vector<V>& values)
			{
				map<K, V> res;
				if (key.size() > values.size()) throw std::logic_error("Zip function must have less keys than values");
				for (size_t i = 0; i < key.size(); i++)
					res[key[i]] = values[i];
				return res;
			}

			/**
			 * \brief Gets the values of a dictionary (map)
			 */
			template<typename K = string, typename V = string>
			static vector<V> GetValues(const map<K, V>& dict)
			{
				vector<V> res;
				res.reserve(dict.size());
				for (auto& kvpair : dict)
					res.push_back(kvpair.second);
				return res;
			}

			/**
			 * \brief Gets the values of a dictionary (map), for a set of keys
			 */
			template<typename K = string, typename V = string>
			static vector<V> GetValues(const map<K, V>& dict, const vector<K>& keys)
			{
				vector<V> res;
				res.reserve(keys.size());
				for (auto& k : keys)
					res.push_back(dict.at(k));
				return res;
			}

			template <typename U>
			static bool LessThan(const U& first, const U& second)
			{
				return first < second;
			}

			template <typename U>
			static bool MoreThan(const U& first, const U& second)
			{
				return first > second;
			}

			/**
			 * \brief Gets the values of a dictionary (map), in the order specified by a set of keys.
			 */
			template <typename K, typename V>
			static vector<V> SortValues(
				const std::map<K,V>& in,
				const vector<K>& order)
			{
				vector<V> result;
				for(auto& k : order)
				{ 
					if (!HasKey(in, k))
						throw std::logic_error("No entry in std::map for key " + k);
					result.push_back(in.at(k));
				}
				return result;
			}

			/**
			 * \brief Flatten jagged vectors to a one dimension vector.
			 */
			template <typename T>
			static vector<T> Serialize(const vector<vector<T>>& series)
			{
				vector<T> result;
				for (size_t i = 0; i < series.size(); i++)
				{
					auto& v = series[i];
					auto n = result.size();
					auto m = v.size();
					result.resize(n + m);
					std::copy(v.begin(), v.end(), result.begin() + n);
				}
				return result;
			}

			/**
			 * \brief Reorder the values in one vector, based on the sorting of a second vector, using a specified comparer.
			 * Acknowledgements: derived from http://stackoverflow.com/a/236199/2752565, Konrad Rudolph
			 */
			template <typename T, typename U>
			static vector<T> SortFromRef(
				const vector<T>& in,
				const vector<U>& reference,
				std::function<bool(const U&, const U&)> comparer = STLHelper::LessThan<U>)
			{
				// Attribution: derived from http://stackoverflow.com/a/236199/2752565, Konrad Rudolph

				typedef typename vector<U>::const_iterator myiter;
				vector<std::pair<size_t, myiter>> order(reference.size());

				size_t n = 0;
				for (myiter it = reference.begin(); it != reference.end(); ++it, ++n)
					order[n] = std::make_pair(n, it);


				Comparer<U> ordering(comparer);

				sort(order.begin(), order.end(), ordering);

				vector<T> ret(in.size());

				size_t const size = in.size();
				for (size_t i = 0; i < size; ++i)
					ret[i] = in[order[i].first];

				return ret;
			}
		};

		template<typename T = double>
		vector<T>  SeqVec(T from, T by, size_t num)
		{
			vector<T> data;
			data.resize(num);
			for (size_t i = 0; i < num; i++)
				data[i] = from + ((T)i)*by;
			return data;
		}

		///////////////////
		// Template metaprogramming
		///////////////////
		

		/**
		 * \brief Yield second or third argument depending on first argument.
		 * Could not find an easy to use if_then_else in the STL or Boost. IfThenElse will probably be replaced.
		 * Primary template: yield second or third argument depending on first argument
		 * 
		 * Credits:
		 * C++ templates : the complete guide / David Vandevoorde, Nicolai M. Josuttis. ISBN 0-201-73484-2 (alk.paper)
		 * 
		 * \tparam C first parameter, boolean
		 * \tparam Ta second parameter
		 * \tparam Tb third parameter
		 */
		template<bool C, typename Ta, typename Tb>
		class IfThenElse;

		/**
		 * \brief  partial specialization: true yields second argument
		 */ 
		template<typename Ta, typename Tb>
		class IfThenElse < true, Ta, Tb > {
		public:
			typedef Ta ResultT;
		};

		/**
		 * \brief  partial specialization: true yields third argument
		 */ 
		template<typename Ta, typename Tb>
		class IfThenElse < false, Ta, Tb > {
		public:
			typedef Tb ResultT;
		};

		template <typename T>
		class ValueTypeVectorDispose
		{
		public:
			static void Dispose(vector<T>& a) { a.clear(); }
		};

		template <typename T>
		class PointerTypeVectorDispose
		{
		public:
			static void Dispose(vector<T>& a)
			{
				for (size_t i = 0; i < a.size(); i++)
				{
					auto p = a[i];
					if (p != nullptr)
						delete p;
				}
				a.clear();
			}
		};

		/**
		 * \brief Template program; Type type is a class suitable to dispose of object T, 
		 * whether it is a vector of value types, or a 
		 * vector where items are pointers requiring the delete operator.
		 * Used to dispose of items in a templated time series, with items of either value or pointer types.
		 * 
		 * \tparam T type of items in the vector to clear/dispose of.
		 */
		template <typename T>
		struct DisposeVectorTypeFactory {
			typedef typename IfThenElse<
				std::is_pointer<T>::value,
				ValueTypeVectorDispose<T>,
				PointerTypeVectorDispose<T>
			>::ResultT type;
		};


		template <typename T>
		void DisposeVector(vector<T>& v)
		{
			using U = typename datatypes::utils::DisposeVectorTypeFactory<T>::type;
			U::Dispose(v);
		}

		/**
		 * \class	bad_lexical_cast
		 *
		 * \brief	A bad_lexical_cast that inherits from std::exception, unlike Boost's. Needed for graceful C API interop.
		 */
		class DATATYPES_DLL_LIB bad_lexical_cast : public std::invalid_argument
		{
		public:
			bad_lexical_cast(const string& msg);
		};

		/**
		 * \brief	Wraps boost::lexical_cast with a try/catch; 
		 *          rethrows an exception that inherits from std::exception and a more useful error message.
		 */
		template <typename Target>
		static Target Parse(const string& strId)
		{
			try {
				return boost::lexical_cast<Target>(strId);
			}
			catch (boost::bad_lexical_cast&)
			{
				throw datatypes::utils::bad_lexical_cast(string("Failed to parse '") + strId + string("' as a ") + string(typeid(Target).name()));
			}
		}

		/**
		 * \brief	Wraps boost::lexical_cast with a try/catch; 
		 *          rethrows an exception that inherits from std::exception and a more useful error message.
		 */
		template <typename Source>
		static string ToString(const Source& value)
		{
			try {
				return boost::lexical_cast<string>(value);
			}
			catch (boost::bad_lexical_cast&)
			{
				throw datatypes::utils::bad_lexical_cast(string("Failed to convert a value of type '") + string(typeid(Source).name()) + string("' to a string"));
			}
		}

		/**
		 * \brief	Converts string items to an array, casting strings. Wraps calls to boost::lexical_cast with a try/catch; 
		 *          rethrows an exception that inherits from std::exception and a more useful error message.
		 */
		template<class TTo>
		static TTo* ConvertToArray(const vector<string>& src)
		{
			auto tmp = std::string("");
			string& strId = tmp; // clang
			TTo* result = new TTo[src.size()];
			try {
				for (int i = 0; i < src.size(); i++)
				{
					strId = src[i];
					result[i] = boost::lexical_cast<TTo>(strId);
				}
				return result;
			}
			catch (boost::bad_lexical_cast & c)
			{
				delete[] result;
				throw datatypes::utils::bad_lexical_cast(string("Failed to parse '") + strId + string("' in an vector as a ") + string(typeid(TTo).name()));
			}
		}

		/**
		 * \brief	Converts string items to an array, casting strings (C casts for each item). 
		 *          rethrows an exception that inherits from std::exception and a more useful error message for the end user (better C API support).
		 */
		template<class TFrom, class TTo>
		static TTo* ConvertToArray(const vector<TFrom>& src)
		{
			TTo* result = new TTo[src.size()];
			try {
				for (int i = 0; i < src.size(); i++)
				{
					result[i] = (TTo)src[i];
				}
				return result;
			}
			catch (std::exception& c)
			{
				delete[] result;
				throw datatypes::utils::bad_lexical_cast(string("Failed to convert vector of ") +
					string(typeid(TFrom).name()) +
					string(" to an array of ") +
					string(typeid(TTo).name()));
			}
		}

		/**
		 * \brief Converts each vector items from one type to another. 
		 *          rethrows an exception that inherits from std::exception and a more useful error message for the end user (better C API support).
		 */
		template<class TFrom, class TTo>
		static vector<TTo> Convert(const vector<TFrom>& src)
		{
			vector<TTo> result(src.size());
			try {
				for (int i = 0; i < src.size(); i++)
					result[i] = (TTo)src[i];
				return result;
			}
			catch (std::exception&)
			{
				string msg = (string("Failed to convert vector of ") +
					string(typeid(TFrom).name()) +
					string(" to a vector of ") +
					string(typeid(TTo).name()));

				throw datatypes::utils::bad_lexical_cast(msg);
			}
		}

		/**
		 * \brief	Converts string items to an array, casting strings. Wraps calls to boost::lexical_cast with a try/catch; 
		 *          rethrows an exception that inherits from std::exception and a more useful error message.
		 */
		template<class TTo>
		vector<TTo> Convert(const vector<string> & src)
		{
			string strId;
			vector<TTo> result(src.size());
			try {
				for (int i = 0; i < src.size(); i++)
				{
					strId = src[i];
					result[i] = boost::lexical_cast<TTo>(strId);
				}
				return result;
			}
			catch (boost::bad_lexical_cast&)
			{
				throw datatypes::utils::bad_lexical_cast(string("Failed to parse '") + strId + string("' in an vector as a ") + string(typeid(TTo).name()));
			}
		}

		/**
		 * \brief Helper class with string processing related functions. These emulate methods found in other languages such as C#, R, etc. 
		 * 
		 */
		class DATATYPES_DLL_LIB StringProcessing
		{
		public:
			static vector<string> Split(const string& s, const string& separators);
			static string TrimAny(const string& s, const string& charactersToTrim);
			static vector<string> SplitOnSpaces(const string& s, bool removeEmptyEntries);
			static vector<string> RemoveEmpty(const vector<string>& s);

			static vector<string> Concatenate(const vector<vector<string>>& vars);
			static void Concatenate(vector<string>& a, const vector<string>& b);


			//			static bool Contains(const string& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool Contains(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool StartsWith(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool Equals(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool EqualsAny(const string& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool StringPredicate(const string& toTest, const string& toMatch, boost::function<bool(const string&, const string&)> predicate, bool caseSensitive = true);
			static string Paste(const vector<string>& items, const string& sep = "");
			static vector<string> VPaste(const vector<string>& prefixes, const string& postfix, const string& sep = "");
			static vector<string> VPaste(const string& prefix, const vector<string>& postfixes, const string& sep = "");
			static bool ContainsAll(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool SetEquals(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static vector<string> SetDiff(const vector<string>& toTest, const vector<string>& toRemove, bool caseSensitive = true);
			static vector<string> Unique(const vector<string>& set);

			static const string kElementSeparatorToken;
			static string BuildIdentifier(vector<string> &tokens, int fromIndex, int toIndex = -1, const string& sep=StringProcessing::kElementSeparatorToken);
			static string BuildIdentifier(const string &a, const string &b, const string& sep = StringProcessing::kElementSeparatorToken);


		private:
			static bool funContains(const string& toTest, const string& toMatch);
			static bool funStartsWith(const string& toTest, const string& toMatch);
			static bool funEquals(const string& toTest, const string& toMatch);
		};

		template<typename T=boost::posix_time::ptime>
		T CreateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0)
		{
			using namespace boost::gregorian;
			return T(date(year, month, day), hours(hour) + minutes(minute) + seconds(second));
		}
	}

	namespace interop
	{
		/**
		 * \brief Ways for wrappers to specify to this API what special numeric value to use 
		 * as 'missing value' code in time series interop.
		 */
		class DATATYPES_DLL_LIB MissingValueHandling
		{
			// https://jira.csiro.au/browse/WIRADA-416
		public:
			/** \brief	The time series missing value. */
			static std::atomic<double> TimeSeriesMissingValueValue;
		};
	}

}