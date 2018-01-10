find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(trianglesModern
    ./src/application.cpp
    ./src/OpenGlDebugLogger.cpp
    ./src/FragmentShader.cpp
    ./src/VertexShader.cpp
    ./src/VertexBuffer.cpp
    ./src/IndexBuffer.cpp
    ./src/Shader.cpp
    ./src/ShaderProgram.cpp
)


configure_file(shaders/basic.vshader shaders/basic.vshader COPYONLY)
configure_file(shaders/basic.fshader shaders/basic.fshader COPYONLY)

target_include_directories(trianglesModern
    PUBLIC
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(trianglesModern
    PUBLIC
        ${GLEW_LIBRARIES}
        glfw
        # This fixes the glClear reference not found linker error
        GL
)
