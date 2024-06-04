add_library(runtimeTools 
    src/GameExecutor.cpp
)

target_include_directories(runtimeTools
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
    runtimeTools
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    PREFIX ""
)

target_link_libraries(
    runtimeTools
    rules
    # engineTools
)