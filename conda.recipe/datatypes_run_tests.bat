SET ROOTDIR=%~dp0

IF "%SWIFT_TEST_DIR%" == "" (
    SET SWIFT_TEST_DIR=%EHP_SHARED%/STF/SWIFTUnitTestData/UnitTests/
)

%ROOTDIR%\datatypes_tests.exe
