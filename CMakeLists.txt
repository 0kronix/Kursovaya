cmake_minimum_required(VERSION 3.14)
project(FBToSVGConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if(MINGW)
    add_compile_options(-Wall -Wextra)
endif()

if(MSVC)
    add_compile_options(/W4 /WX /EHsc)
endif()

add_executable(fb_to_svg_converter
    main.cpp
    src/FBToSVGConverter.cpp
)

target_include_directories(fb_to_svg_converter PRIVATE src)