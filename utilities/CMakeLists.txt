find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_library(utilities STATIC
    ./src/OpenGlDebugLogger.cpp
    ./src/FragmentShader.cpp
    ./src/VertexShader.cpp
    ./src/VertexBuffer.cpp
    ./src/IndexBuffer.cpp
    ./src/Shader.cpp
    ./src/ShaderProgram.cpp
    ./src/ObjModel.cpp
)

add_library(gl::utilities ALIAS utilities)

target_include_directories(utilities
    PUBLIC
        ./include
	${GLEW_INCLUDE_DIRS}
)

target_link_libraries(utilities
    PUBLIC
	${GLEW_LIBRARIES}
        glfw
        # This fixes the glClear reference not found linker error
        GL

)
