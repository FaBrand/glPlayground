find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(triangles
    ./src/application.cpp
)


target_include_directories(triangles
    PUBLIC
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(triangles
    PUBLIC
        ${GLEW_LIBRARIES}
        glfw
        # This fixes the glClear reference not found linker error
        GL
)
