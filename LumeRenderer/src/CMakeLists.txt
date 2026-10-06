add_executable(lume
    main.cpp
    core/window.cpp
)

# src/ is the only include root: #include "gfx/shader.h"
target_include_directories(lume PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lume PRIVATE
    glad
    glfw
    glm::glm
    imgui::imgui
)

if(MSVC)
    target_compile_options(lume PRIVATE /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

set_target_properties(lume PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
