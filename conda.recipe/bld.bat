REM Build datatypes.

REM Set up destinations dirs for install
mkdir "%LIBRARY_PREFIX%\tests\swift"
mkdir build

REM test runner script
copy "%RECIPE_DIR%\datatypes_run_tests.bat" "%LIBRARY_PREFIX%\tests\swift\"

REM build everything
cd build

set PATH="%PREFIX%\bin";%PATH%

cmake -G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ..\datatypes
if %errorlevel% neq 0 exit 1
msbuild /p:Configuration=Release /v:q /clp:/v:q "INSTALL.vcxproj"
if %errorlevel% neq 0 exit 1
xcopy /S /Y install\lib\*.* %LIBRARY_LIB%\
xcopy /S /Y install\include\*.* %LIBRARY_INC%\
if %errorlevel% neq 0 exit 1
del *.*

cmake -G "%CMAKE_GENERATOR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install ..\tests
if %errorlevel% neq 0 exit 1
msbuild /p:Configuration=Release /v:q /clp:/v:q "INSTALL.vcxproj"
if %errorlevel% neq 0 exit 1

REM Install the build stuff
move install\bin\datatypes_tests.exe %LIBRARY_PREFIX%\tests\swift\
