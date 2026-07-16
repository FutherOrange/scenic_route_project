@echo off
REM ===============================
REM Scenic Route System Launcher
REM ===============================

title Scenic Route Planning System

cd /d "C:\Users\ZhuanZ1\scenic_route_project"

echo.
echo ========================================
echo   Scenic Route Planning System
echo ========================================
echo.

set PATH=C:\MinGW\bin;%PATH%

REM Clean old object files
del /Q *.o 2>nul

echo [1/2] Compiling source code...
gcc -finput-charset=UTF-8 -fexec-charset=GBK main.c graph.c -o scenic_route.exe

if errorlevel 1 (
    echo.
    echo [ERROR] Compile FAILED!
    echo         Check if MinGW GCC is installed at C:\MinGW\bin
    echo.
    pause
    exit /b 1
)

echo [OK] Compile success.
echo.
echo [2/2] Starting program...
echo ========================================
echo.

pause
cls
scenic_route.exe

echo.
echo Program exited. Press any key to close...
pause >nul
