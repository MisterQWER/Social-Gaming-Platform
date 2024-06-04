add_library(rockpaperscissors rock_paper_scissors.cpp)

set_target_properties(rockpaperscissors
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

target_include_directories(rockpaperscissors
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS rockpaperscissors
    RUNTIME DESTINATION bin
    )