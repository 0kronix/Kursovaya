@echo off
echo ========================================
echo    FB to SVG Converter - Build Script
echo ========================================
echo.

REM Переходим в папку скрипта
cd /d "%~dp0"

echo Step 1: Cleaning old build...
if exist build rmdir /s /q build
del fb_to_svg_converter.exe 2>nul

echo Step 2: Creating build directory...
mkdir build
cd build

echo Step 3: Configuring CMake...
cmake -G "MinGW Makefiles" ..

if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    echo.
    echo Solutions:
    echo 1. Install MinGW: https://winlibs.com/
    echo 2. Add MinGW to PATH
    echo 3. Run from Visual Studio Developer Command Prompt
    pause
    exit /b 1
)

echo Step 4: Building project...
cmake --build .

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo SUCCESS: Build completed!
    echo.
    
    REM Проверяем создался ли exe в корне
    cd ..
    if exist fb_to_svg_converter.exe (
        echo Executable: fb_to_svg_converter.exe
        echo Size: %~z0 bytes
        echo.
        echo To run: fb_to_svg_converter.exe
        echo.
        echo Running program now...
        echo ========================================
        fb_to_svg_converter.exe
    ) else (
        echo ERROR: Executable not created!
    )
) else (
    echo.
    echo ERROR: Build failed!
)

echo.
pause