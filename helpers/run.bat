@echo off

call "%~dp0\build.bat" >nul 2>&1

if %ERRORLEVEL% neq 0 (
    type "%LOGFILE%"
    exit /b 1
)

build\msvc-debug\Debug\pz-map-generator.exe %*