cmake_minimum_required(VERSION 3.14)
project(FBToSVGConverter VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(FetchContent)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG v2.9
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG 9.0.0
)
set(TINYXML2_BUILD_TESTING OFF CACHE BOOL "Disable testing" FORCE)
set(TINYXML2_BUILD_SAMPLES OFF CACHE BOOL "Disable samples" FORCE)
FetchContent_MakeAvailable(tinyxml2)

add_executable(fb_to_svg_converter
    main.cpp
    src/FBToSVGConverter.cpp
)

target_include_directories(fb_to_svg_converter 
    PRIVATE 
    src
    ${argparse_SOURCE_DIR}/include
)

target_link_libraries(fb_to_svg_converter PRIVATE tinyxml2)
target_include_directories(fb_to_svg_converter PRIVATE src)