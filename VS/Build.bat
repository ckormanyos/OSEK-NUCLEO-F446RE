@echo off

:: Build from Visual Studio (SOLUTION_DIR)

:: Get the build start time

for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

::set currentDir=%CD%

set SOLUTION_PATH=..
cd %SOLUTION_PATH%

:: Environment Variables
set SOLUTION_PATH=%CD%
set MAKE_CMD=%SOLUTION_PATH%\Build\Utilities\make\make.exe
set MAKE_GMK_DIR=%SOLUTION_PATH%\Build\Makefiles
set MAKE_GMK_FILE=%MAKE_GMK_DIR%\Make_000_Build.gmk
set MAKEFILES_PATH=%MAKE_GMK_DIR%
set BUILD_TYPE=%1
set BUILD_NAME=%2
set MAKE_PARAM=BUILD_TYPE=%BUILD_TYPE% BUILD_NAME=%BUILD_NAME% SOLUTION_PATH=%SOLUTION_PATH%
set SCONSOLE=%SOLUTION_PATH%\Build\Utilities\sconsole.exe
set XECHO=%SOLUTION_PATH%\Build\Utilities\xecho.exe
set IsOk=TRUE

:: Configure the console
::%SCONSOLE% -fontSize=12 -screenBuffer={300,5000} -windowSize={200,40} -title=%BUILD_TYPE%

:: Display the paths
echo SOLUTION_PATH  : %SOLUTION_PATH%
echo MAKE_CMD       : %MAKE_CMD%
echo MAKEFILES_PATH : %MAKEFILES_PATH%
echo BUILD_NAME     : %BUILD_NAME%
echo BUILD_TYPE     : %BUILD_TYPE%
echo.


:: Build rule
if "%BUILD_TYPE%"=="Build" (
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% PreBuild
%MAKE_CMD% %MAKE_PARAM% --jobs=%NUMBER_OF_PROCESSORS% --output-sync=none -k -f %MAKE_GMK_FILE% Build
if ERRORLEVEL 1 (goto End)
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% PostBuild
)

:: Rebuild rule
if "%BUILD_TYPE%"=="Rebuild" (
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% PreRebuild
%MAKE_CMD% %MAKE_PARAM% --jobs=%NUMBER_OF_PROCESSORS% --output-sync=none -k -f %MAKE_GMK_FILE% Rebuild
if ERRORLEVEL 1 (goto End)
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% PostRebuild
)

:: Clean rule
if "%BUILD_TYPE%"=="Clean" (
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% %BUILD_TYPE%
)

:: Preprocessing rule
if "%BUILD_TYPE%"=="Preprocessing" (
%MAKE_CMD% %MAKE_PARAM% --jobs=1 -k -f %MAKE_GMK_FILE% PreRebuild
%MAKE_CMD% %MAKE_PARAM% --jobs=%NUMBER_OF_PROCESSORS% --output-sync=none -k -f %MAKE_GMK_FILE% Preprocessing
)

:: Check the global result
:End
if ERRORLEVEL 1 (set IsOk=FALSE)

:: Calculate the building time

for /F "tokens=1-4 delims=:.," %%a in ("%time%") do (
   set /A "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

set /A elapsed=end-start

set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%

echo.

echo Build Duration: %hh%:%mm%:%ss%,%cc%

echo.

:: Display the result
if %IsOk%==TRUE (%XECHO% -color 0x20 Build succeeded) else (%XECHO% -color 0x80 Build failed)

:: Save the build log
::%SCONSOLE% -logFile=%currentDir%\%BUILD_TYPE%Log.txt

exit 0
