#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <vector>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "catch/catch.hpp"
#include "catch/catch_macros.hpp"

#include "cinterop/c_cpp_interop.hpp"

// We first include the extern_c_api as a user of the external library via opaque pointers:
#define DATATYPES_USE_OPAQUE_POINTERS
#define USING_DATATYPES
#include "datatypes/extern_c_api.h"

// Then we also include the macros for transparent pointers.
#define FORCE_OPAQUE_PTR_TYPECAST

#undef DATATYPES_USE_OPAQUE_POINTERS
#define DATATYPES_USE_CPP_POINTERS

#include "datatypes/internals_c_api.hpp"
#undef FORCE_OPAQUE_PTR_TYPECAST
#undef  DATATYPES_USE_CPP_POINTERS
#define DATATYPES_USE_OPAQUE_POINTERS


#include "datatypes/datatypes_test_helpers.h"

using namespace std;


void delete_ansi_string_array(char** values, int arrayLength)
{
	if (values == nullptr)
		throw std::logic_error("delete_ansi_string_array: values cannot be a nullptr");
	for (int i = 0; i < arrayLength; i++)
		delete[] values[i];
	delete[] values;
}

TEST_CASE("Retrieving", "[uchronia]")
{

	using datatypes::tests::TestDataLocationHelper;
	using datatypes::utils::StringProcessing;
	using namespace cinterop::utils;
	TimeSeriesLibrary* dataLibrary = TestDataLocationHelper::CreateTestTimeSeriesLibrary();

	auto ds = WRAP_ENSEMBLE_DATA_SET_PTR(dataLibrary);

	int size;
	char** cids = GetEnsembleDatasetDataIdentifiers(ds, &size);
	vector<string> ids = to_cpp_string_vector(cids, size, false);
	DeleteAnsiStringArray(cids, size);

	REQUIRE(StringProcessing::SetEquals(ids, TestDataLocationHelper::TestTsLibraryIdentifiers()));

	auto streq = [&](char* a, char* b) 
	{
		REQUIRE(string(a) == string(b));		
	};

	time_series_dimensions_description* tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1Obs.c_str());
	REQUIRE(tsdim->num_dimensions == 1);
	streq(tsdim->dimensions[0].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1Ens.c_str());
	REQUIRE(tsdim->num_dimensions == 2);
	streq(tsdim->dimensions[0].dimension_type, ENSEMBLE_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[1].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1FcastEns.c_str());
	REQUIRE(tsdim->num_dimensions == 3);
	streq(tsdim->dimensions[0].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[1].dimension_type, ENSEMBLE_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[2].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	delete ds;
}
