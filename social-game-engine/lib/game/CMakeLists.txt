add_library(parser_objects_common 
CommonSrc/Common.cpp)



target_include_directories(
    parser_objects_common
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/CommonInclude>   
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/CommonSrc
)

target_link_libraries(
    parser_objects_common
    tree-sitter-socialgaming
    cpp-tree-sitter
)

add_subdirectory(Game)
add_subdirectory(GameControls)
add_subdirectory(GameSpecification)
