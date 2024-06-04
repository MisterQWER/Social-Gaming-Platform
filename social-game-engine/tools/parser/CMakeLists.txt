add_executable(demo)
target_sources(demo
  PRIVATE
    demo.cpp
)
target_link_libraries(demo
  rules
  tree-sitter-socialgaming
  cpp-tree-sitter
)


add_executable(parserDriver)
target_sources(parserDriver
    PRIVATE
        parserDriver.cpp
)

target_link_libraries(parserDriver
    configuration
    game_specs
    constants
    variables
    rules
)
