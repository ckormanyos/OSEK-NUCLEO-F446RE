@echo off

set SEARCH_PATH=..\..\Code

set GEN_PATH=..\Build\Makefiles

cd %SEARCH_PATH%

:: Src Files List
dir /s /b /o *.c *.cpp *.s *.asm *.850 > %GEN_PATH%\SrcFiles.txt

:: Inc Files List
dir /s /b /o *.h *.hpp > %GEN_PATH%\IncFiles.txt

:: Include Paths
dir /ad /b /s > %GEN_PATH%\IncPaths.txt

echo Files lists are generated in "%GEN_PATH%"

pause