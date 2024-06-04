cmake_minimum_required(VERSION 3.24)
project(SocialGamingPlatform)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-rtti")
add_compile_options(
        "$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wconversion>"
        "$<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:DEBUG>>:-fno-omit-frame-pointer;-O0;-g;-fsanitize=address,undefined>"
)
add_link_options(
        "$<$<AND:$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:DEBUG>>:-fsanitize=address,undefined>"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

# Downloads this wrapper library and tree-sitter.
# Makes them available via the `cpp-tree-sitter` CMake library target.
CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.2
)


# Downloads a tree-sitter grammar from github and makes it available as a
# cmake library target.
add_grammar_from_repo(tree-sitter-socialgaming
        https://github.com/nsumner/tree-sitter-socialgaming
        0.0.1
)

file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/rock-paper-scissors.game
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/games/
)


#[[
    Emscripten is an open source compiler to for WebAssembly

    - Essentially compiles C++/C into WebAssembly

    https://emscripten.org/docs/introducing_emscripten/about_emscripten.html
]]
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
  set(NETWORKING_EMSCRIPTEN_BUILD ON)
  set(CMAKE_CXX_FLAGS_DEBUG
          "-O0 -g -fno-omit-frame-pointer"
  )
else()
  set(NETWORKING_EMSCRIPTEN_BUILD OFF)
  set(CMAKE_CXX_FLAGS_DEBUG
          "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
  )
endif()

#[[
    - Commenting out the Flutter Option

    option(ENABLE_FLUTTER_CLIENT "Build the Flutter client" ON)
]]
option(ENABLE_NCURSES_CLIENT "Build the NCurses client" ON)
option(ENABLE_FTXUI_CLIENT   "Build the FTXUI client" ON)
option(ENABLE_ENGINE_TOOLS "Build the Engine Tools Driver (for Engine testing)" ON)

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(src)
add_subdirectory(test)
