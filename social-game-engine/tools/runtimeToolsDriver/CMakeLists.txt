add_executable(runtimeDriver
    runtimeDriver.cpp
)

set_target_properties(
    runtimeDriver
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    PREFIX ""
)

target_link_libraries(runtimeDriver
    runtimeTools
    parser_objects_common
    configuration
    rules
)

install(TARGETS runtimeDriver
  RUNTIME DESTINATION bin
)