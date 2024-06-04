#[[
    Base classes for the control structures
]]

add_library(game_controls
    src/Discard.cpp
    src/Message.cpp
    src/InputChoice.cpp
    src/For.cpp
    src/ParallelFor.cpp
    src/Scores.cpp
    src/Extend.cpp
    src/Match.cpp
    src/Assignment.cpp
)

target_include_directories(
    game_controls
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(game_controls
    configuration
    parser_objects_common
    tree-sitter-socialgaming
    cpp-tree-sitter
    engineTools
)