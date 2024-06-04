add_library(engineTools
    src/game_event.cpp
    src/input_event.cpp
    src/timed_event.cpp
    src/BaseEnvironment.cpp
    src/GlobalVariables.cpp
    src/GameInstance.cpp
)

target_include_directories(engineTools
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#[[ 
    If you need other libraries do let me know, I've included Boost incase you need it.
]]

find_package(Boost 1.72 COMPONENTS system REQUIRED)
target_link_libraries(engineTools
  
  PRIVATE
    ${Boost_LIBRARIES}
  PUBLIC
    rules
)
target_include_directories(engineTools
  PRIVATE
    ${Boost_INCLUDE_DIR}
)

install(TARGETS engineTools
  ARCHIVE DESTINATION lib
)
