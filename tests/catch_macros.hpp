#pragma once

#include "catch.hpp"

// Additional macros that were handy when migrating unit tests written with xUnit++ originally

#define REQUIRE_EQUAL( expected, actual ) REQUIRE( expected == actual )
#define REQUIRE_EQUAL_COMPARER( expected, actual, comparer) REQUIRE( comparer( expected, actual ) )
#define REQUIRE_NOT_EQUAL( expected, actual ) REQUIRE( expected != actual )
#define REQUIRE_NULL( actual ) REQUIRE( nullptr == actual )
#define REQUIRE_NOT_NULL( actual ) REQUIRE( nullptr != actual )
#define REQUIRE_THROWS_EXCEPTION_TYPE(exceptionType, expr) REQUIRE_THROWS_AS( expr, exceptionType)

