add_executable(lume
    main.cpp
    core/window.cpp
    core/app.cpp
    gfx/gl_debug.cpp
)

target_include_directories(lume PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lume PRIVATE
    glad
    glfw
    glm::glm
    imgui::imgui
    assimp::assimp
)

if(MSVC)
    target_compile_options(lume PRIVATE /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

target_compile_definitions(lume PRIVATE LUME_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")
