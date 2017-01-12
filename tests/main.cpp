#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "common_datatypes.h"

void TestTimeStepAddSteps(const std::string& start_iso_str, const std::string& expected_iso_str, int nSteps)
{
	TimeStep ts = TimeStep::GetMonthlyQpp();

	ptime start = from_iso_string(start_iso_str);
	ptime actual = ts.AddSteps(start, nSteps);
	ptime expected = from_iso_string(expected_iso_str);
	REQUIRE_EQUAL(expected, actual);
}

TEST_CASE("Monthly QPP TimeStep AddSteps Test")
{
	// Positive

	// [dd < 24]
	TestTimeStepAddSteps("20000123T101112", "20000123T101112", 0);
	TestTimeStepAddSteps("20000123T101112", "20000223T101112", 1);
	TestTimeStepAddSteps("20000123T101112", "20010623T101112", 17);

	// [dd >= 24] w/seven day offset
	TestTimeStepAddSteps("20000124T101112", "20000124T101112", 0);
	TestTimeStepAddSteps("20000124T101112", "20000222T101112", 1);
	TestTimeStepAddSteps("20000124T101112", "20010623T101112", 17);

	// Negative

	// [dd < 24]
	TestTimeStepAddSteps("20000123T101112", "20000123T101112", -0);
	TestTimeStepAddSteps("20000123T101112", "19991223T101112", -1);
	TestTimeStepAddSteps("20000123T101112", "19980823T101112", -17);

	// [dd >= 24] w/seven day offset
	TestTimeStepAddSteps("20000124T101112", "20000124T101112", -0);
	TestTimeStepAddSteps("20000124T101112", "19991224T101112", -1);
	TestTimeStepAddSteps("20000124T101112", "19980824T101112", -17);
}

void TestTimeStepNumSteps(const std::string& start_iso_str, const std::string& end_iso_str, ptrdiff_t exp_upper, ptrdiff_t exp_steps, ptrdiff_t exp_offset)
{
	TimeStep ts = TimeStep::GetMonthlyQpp();

	ptime s = from_iso_string(start_iso_str);
	ptime e = from_iso_string(end_iso_str);
	ptrdiff_t act_upper = ts.GetUpperNumSteps(s, e);
	REQUIRE_EQUAL(exp_upper, act_upper);
	ptrdiff_t act_steps = ts.GetNumSteps(s, e);
	REQUIRE_EQUAL(exp_steps, act_steps);
	ptrdiff_t act_offset = ts.GetOffset(s, e);
	REQUIRE_EQUAL(exp_offset, act_offset);
}

TEST_CASE("Monthly QPP TimeStep NumSteps Test")
{
	// [dd < 24]

	// Difference between start and end is a fraction (one second) less than one month
	TestTimeStepNumSteps("20000123T000000", "20000222T235959", 2, 1, 0);
	
	// Difference between start and end exactly one month
	TestTimeStepNumSteps("20000123T000000", "20000223T000000", 2, 2, 1);

	// Difference between start and end is a fraction (one second) more than one month
	TestTimeStepNumSteps("20000123T000000", "20000223T000001", 3, 2, 1);

	// [dd >= 24] w/seven day offset

	// Difference between start and end is a fraction (one second) less than one month
	TestTimeStepNumSteps("20000124T000000", "20000221T235959", 2, 1, 0);

	// Difference between start and end exactly one month
	TestTimeStepNumSteps("20000124T000000", "20000222T000000", 2, 2, 1);

	// Difference between start and end is a fraction (one second) more than one month
	TestTimeStepNumSteps("20000124T000000", "20000222T000001", 3, 2, 1);
}
