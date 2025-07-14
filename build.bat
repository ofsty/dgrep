@echo off

where cl >nul 2>&1
if errorlevel 1 (
    echo cl.exe not found. Please open this script in the "Developer Command Prompt for Visual Studio".
    exit /b 1
)

cl /EHsc /std:c++17 /I src src\bmh.cpp src\dgrep.cpp /Fe:dgrep.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Executable: dgrep.exe
) else (
    echo Build failed.
)
pause
