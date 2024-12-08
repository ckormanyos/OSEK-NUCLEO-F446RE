@echo off

set __SearchDirRel=..\..\..\
set __SearchDirAbs=
set __CurrentDir=%CD%

:: Convert relative paths to absolute paths
pushd %__SearchDirRel%
set __SearchDirAbs=%CD%
popd

:: Change the active directory to search dir
pushd %CD%
cd %__SearchDirAbs%

:: Src Files List
dir /s /b /o *.c *.cpp *.s *.asm *.850 > %__CurrentDir%\SrcFiles.txt

:: Inc Files List
dir /s /b /o *.h *.hpp > %__CurrentDir%\IncFiles.txt

:: Include Paths
dir /ad /b /s > %__CurrentDir%\IncPaths.txt

:: Restore the active directory
popd

