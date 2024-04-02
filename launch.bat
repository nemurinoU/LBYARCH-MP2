@echo off

rem
cd /d "%~dp0"
cd "LBYARCH"

rem
"../x64/Debug/MP2_Interface_Programming.exe"
cd ../../
rem
cd /d "%~dp0"
cmd /k
