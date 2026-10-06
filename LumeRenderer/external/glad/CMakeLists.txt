# glad 2.0.8 — gl:core=4.6 with GL_ARB_bindless_texture, GL_ARB_gpu_shader_int64,
# GL_ARB_shader_viewport_layer_array, GL_EXT_texture_compression_s3tc.
# Regenerate with the permalink in the comment header of include/glad/gl.h.

add_library(glad STATIC
    src/gl.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Generated code: don't hold it to the project's warning level.
if(MSVC)
    target_compile_options(glad PRIVATE /W0)
endif()
