add_library(gamespecification game_specification.cpp)

set_target_properties(gamespecification
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

target_include_directories(gamespecification
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gamespecification PUBLIC
  configuration
  )

install(TARGETS gamespecification
    RUNTIME DESTINATION bin
    )