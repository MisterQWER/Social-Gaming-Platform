add_subdirectory(gtest)

add_executable(runAllTests
  # You can create new source files for your tests and add them here,
  # or you can add your tests to Dummy.cpp. Either way will work.
  ConfigurationTest.cpp
  ConfigurationKindTest.cpp
  GameSessionsTest.cpp
  PlayerTest.cpp
  GameSessionManagerTest.cpp
  VariableResolutionTest.cpp
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    configuration
    constants
    variable_resolution
    networking
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)

# Creating a test for Executors
add_executable(executorTests
  ExecutorTests.cpp
)

target_link_libraries(executorTests
  GTest::gmock GTest::gtest GTest::gtest_main
  runtimeTools
  rules
  tree-sitter-socialgaming
  cpp-tree-sitter
)

