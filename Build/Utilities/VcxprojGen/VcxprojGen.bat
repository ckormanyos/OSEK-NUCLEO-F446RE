@echo off

set __SolutionDirRel=..\..\..\VS
set __SolutionDirAbs=

set __SearchDirRel=..\..\..\
set __SearchDirAbs=

:: Convert relative paths to absolute paths
pushd %__SolutionDirRel%
set __SolutionDirAbs=%CD%
popd

pushd %__SearchDirRel%
set __SearchDirAbs=%CD%
popd

:: Search for the name of the visual studio project
pushd %CD%
cd %__SolutionDirAbs%
dir  /s /b /o *.vcxproj > vcxprojName.txt
set /p VCXPROJ_FILE=<vcxprojName.txt
del vcxprojName.txt
popd

:: Call VcxprojGen to generate the filters
set SOLUTION_DIR=-SolutionDir=%__SolutionDirAbs%
set SEARCH_PATH=-SearchDir=%__SearchDirAbs%
set VCXPROJ=-VcxprojFile=%VCXPROJ_FILE%
set VCXFILTERS=-OtherFilters=*.gmk *.arm *.ld

VcxprojGen.exe %SOLUTION_DIR%  %SEARCH_PATH%  %VCXPROJ% %VCXFILTERS%


pause
