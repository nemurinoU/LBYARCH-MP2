@echo off
setlocal enabledelayedexpansion
set "envFile=.env"
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)

for /f "tokens=1,* delims==" %%a in ('type "%envFile%"') do (
    set "key=%%a"
    set "value=%%b"

    REM Set environment variables
    set "!key!=!value!"
)

cd __tests__
call :colorEcho 0e "======================================"
echo.
call :colorEcho 0e "AVAILABLE TESTS"
echo.
call :colorEcho 0a "[NOTE] If you do not select a file, it will default to input1.txt"
echo.
call :colorEcho 0e "======================================"
echo.
dir /A:-D /B
echo.
echo.

cd../

set /p TEST_NAME=Enter filename:


rem
cd /d "%~dp0"
cd "LBYARCH"




REM Loop from 1 to %count%
for /l %%i in (1,1,%count%) do (
    @echo on
    call :colorEcho 0a "======================================"
    call :colorEcho 0a "[STATUS] Running Iteration # %%i..."
    call :colorEcho 0a "======================================"
    echo.
    echo.
    echo %TEST_NAME%>temp.txt
    type temp.txt | "../x64/Debug/MP2_Interface_Programming.exe"
    @echo off
)

@echo off
cd ../../
rem
cd /d "%~dp0"
cmd /k

pause
exit
:colorEcho
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1i
