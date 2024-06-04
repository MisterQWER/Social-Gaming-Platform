if (NETWORKING_EMSCRIPTEN_BUILD)
  message("Skipping server and NCurses client under emscripten build.")
else()
  add_subdirectory(server)
  if (ENABLE_NCURSES_CLIENT)
    add_subdirectory(client-ncurses)
  endif()
endif()

#[[ 

-- Commenting out the Flutter Client

if (ENABLE_FLUTTER_CLIENT)
  add_subdirectory(client_flutter)
endif()
]]
if (ENABLE_FTXUI_CLIENT)
  add_subdirectory(client-ftxui)
endif()

if(ENABLE_ENGINE_TOOLS)
  add_subdirectory(engineToolsDriver)
endif()

add_subdirectory(parser)
add_subdirectory(runtimeToolsDriver)


