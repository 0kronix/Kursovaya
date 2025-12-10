@echo off

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
    pause
    exit /b 1
)

echo Step 4: Building project...
cmake --build .

if %errorlevel% equ 0 (
    echo.
    echo SUCCESS: Build completed!
    echo.
    
    cd ..
    if exist fb_to_svg_converter.exe (
        echo Executable: fb_to_svg_converter.exe
        echo.
        fb_to_svg_converter.exe --help
        echo.
        cmd /k
    ) else (
        echo ERROR: Executable not created!
        pause
    )
) else (
    echo.
    echo ERROR: Build failed!
    pause
)