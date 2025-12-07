add_library(FBToSVGConverter
    FBToSVGConverter.cpp
    FBToSVGConverter.h
)

target_include_directories(FBToSVGConverter
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(FBToSVGConverter PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "FBToSVGConverter.h"
)

install(TARGETS FBToSVGConverter
    EXPORT FBToSVGConverterTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

install(EXPORT FBToSVGConverterTargets
    FILE FBToSVGConverterConfig.cmake
    NAMESPACE FBToSVG::
    DESTINATION lib/cmake/FBToSVGConverter
)