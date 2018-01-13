find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(trianglesModern
    ./src/application.cpp
)


configure_file(shaders/basic.vshader shaders/basic.vshader COPYONLY)
configure_file(shaders/basic.fshader shaders/basic.fshader COPYONLY)

target_link_libraries(trianglesModern
    PUBLIC
	gl::utilities
)
