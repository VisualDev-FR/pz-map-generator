@echo off

set LOGFILE=%TEMP%\zomboid_build_log.tests.txt

call "%~dp0\build.bat" > "%LOGFILE%" 2>&1

if %ERRORLEVEL% neq 0 (
    type "%LOGFILE%"
    exit /b 1
)

del "%LOGFILE%"

build\msvc-debug\Debug\tests.exe %*

