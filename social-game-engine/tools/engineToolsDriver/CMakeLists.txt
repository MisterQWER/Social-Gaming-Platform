add_executable(engineDriver
    engineDriver.cpp
)

set_target_properties(
    engineDriver
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    PREFIX ""
)

target_link_libraries(engineDriver
    engineTools
    configuration
)

install(TARGETS engineDriver
  RUNTIME DESTINATION bin
)