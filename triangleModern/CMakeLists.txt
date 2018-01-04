find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(trianglesModern
    ./src/application.cpp
)


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
