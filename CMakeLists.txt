cmake_minimum_required(VERSION 3.10)
project(FBToSVGConverter VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()

add_executable(fb_to_svg_converter
    main.cpp
    src/FBToSVGConverter.cpp
)

target_include_directories(fb_to_svg_converter PRIVATE src)

if(MINGW)
    target_link_libraries(fb_to_svg_converter stdc++fs)
endif()

add_custom_command(TARGET fb_to_svg_converter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/resources/fbt"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/resources/svg"
)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")