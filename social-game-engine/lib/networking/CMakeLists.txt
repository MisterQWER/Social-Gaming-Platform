add_library(networking
  src/Server.cpp
  src/Client.cpp
  src/GameServer.cpp
  src/GameSession.cpp
  src/GameSessionsManager.cpp
  src/GameMessage.cpp
)

target_include_directories(networking
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#[[
    For Emscript Builds
]]
if (NETWORKING_EMSCRIPTEN_BUILD)
  target_link_libraries(networking
    PUBLIC
      engineTools
      rockpaperscissors
      gamespecification
    PRIVATE
      websocket.js
  )
else()
#[[
    For Boost Builds
]]
  find_package(Boost 1.72 COMPONENTS system REQUIRED)
  target_link_libraries(networking
    PUBLIC
      engineTools
      rockpaperscissors
      gamespecification
    PRIVATE
      ${Boost_LIBRARIES}
  )
  target_include_directories(networking
    PRIVATE
      ${Boost_INCLUDE_DIR}
  )
endif()

set_target_properties(networking
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
)

add_subdirectory(src)

install(TARGETS networking clientInfo
  ARCHIVE DESTINATION lib
)