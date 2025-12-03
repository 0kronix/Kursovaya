cmake_minimum_required(VERSION 3.10)
project(FBToSVGConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared library" OFF)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" OFF)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(FBToSVGConverter
    "${SRC_DIR}/FBToSVGConverter.cpp"
)

target_include_directories(FBToSVGConverter
    PUBLIC 
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        "${SRC_DIR}"
)

target_compile_features(FBToSVGConverter PUBLIC cxx_std_11)
set_target_properties(FBToSVGConverter PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(TARGETS FBToSVGConverter
    EXPORT FBToSVGConverterTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES "${INCLUDE_DIR}/FBToSVGConverter.h"
    DESTINATION include
)

install(EXPORT FBToSVGConverterTargets
    FILE FBToSVGConverterConfig.cmake
    NAMESPACE FBToSVG::
    DESTINATION lib/cmake/FBToSVGConverter
)