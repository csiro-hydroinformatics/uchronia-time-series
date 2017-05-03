cd %~dp0
python -c "import tarfile; tarfile.open('UnitTestData.tar.gz', 'r:*').extractall(path='')"
SET SWIFT_TEST_DIR=%CD%\UnitTests

call %PREFIX%\Library\tests\swift\datatypes_run_tests.bat
