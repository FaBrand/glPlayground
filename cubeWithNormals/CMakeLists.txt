find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(cubeWithNormals
    ./src/application.cpp
)


configure_file(shaders/basic.vshader shaders/basic.vshader COPYONLY)
configure_file(shaders/basic.fshader shaders/basic.fshader COPYONLY)
configure_file(./res/cube_tutorial.obj res/cube_tutorial.obj COPYONLY)
configure_file(./res/simple_mesh.obj res/simple_mesh.obj COPYONLY)
configure_file(./res/blender_object.obj res/blender_object.obj COPYONLY)

target_link_libraries(cubeWithNormals
    PUBLIC
        gl::utilities
)
