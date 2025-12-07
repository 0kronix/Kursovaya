@echo off

cd /d "%~dp0"

if exist build rmdir /s /q build
del fb_to_svg_converter.exe 2>nul

mkdir build
cd build

cmake -G "MinGW Makefiles" ..

if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

cmake --build .

if %errorlevel% equ 0 (
    cd ..
    if not exist fb_to_svg_converter.exe (
        echo ERROR: Executable not created!
    )
) else (
    echo.
    echo ERROR: Build failed!
)

echo.
pause