@echo off

rmdir /s /q build >nul 2>&1

set PRESET_NAME=mingw-debug

cmake --preset %PRESET_NAME%
cmake --build build/%PRESET_NAME%