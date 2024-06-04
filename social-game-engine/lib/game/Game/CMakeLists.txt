#[[
    Configuration
]]
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_library(configuration 
src/Configuration.cpp)

target_include_directories(
    configuration
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
    
target_link_libraries(
    configuration
    parser_objects_common
    nlohmann_json::nlohmann_json
)

#[[
    Rules
]]

add_library(rules 
src/Rules.cpp)

target_include_directories(
    rules
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    rules
    parser_objects_common
    game_controls
)

#[[
   Game Specification
]]

add_library(game_specs
        src/GameSpecification.cpp
        include/Value.h)

target_include_directories(
        game_specs
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        game_specs
        configuration
        constants
        rules
        parser_objects_common
)

#[[
   Variable Resolution
]]

 add_library(variable_resolution
         src/VariableResolution.cpp
         include/Value.h)

 target_include_directories(
         variable_resolution
         PUBLIC
         $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         PRIVATE
         ${CMAKE_CURRENT_SOURCE_DIR}/src
 )

 target_link_libraries(
         variable_resolution
         parser_objects_common
 )

#[[
    Constants
]]
add_library(constants
        src/Constants.cpp)

target_include_directories(
        constants
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        constants
        variable_resolution
)

#[[
    Variables
]]
 add_library(variables
         src/Variables.cpp)

 target_include_directories(
         variables
         PUBLIC
         $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         PRIVATE
         ${CMAKE_CURRENT_SOURCE_DIR}/src
 )

 target_link_libraries(
         variables
         variable_resolution
 )

#[[
    Per-Audience
]]

add_library(audience
        src/PerAudience.cpp
        include/Value.h)

target_include_directories(
        audience
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        audience
        variable_resolution
)


#[[
    Per-player
]]

add_library(player
        src/PerPlayer.cpp
        include/Value.h)

target_include_directories(
        player
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        player
        variable_resolution
)
