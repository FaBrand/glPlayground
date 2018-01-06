#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

#include "FragmentShader.h"
#include "OpenGlDebugLogger.h"
#include "ShaderProgram.h"
#include "VertexShader.h"

int main(int, char**)
{
    if (!glfwInit())
    {
        std::cout << "Could not initialize glfw" << std::endl;
        return -1;
    }

    OpenGlDebugLogger debugging;
    debugging.SetContextDebugOption();

    GLFWwindow* window{glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL)};
    if (!window)
    {
        std::cout << "Could not create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error when initializing GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGl Version: " << glGetString(GL_VERSION) << std::endl;
    debugging.Enable();

    constexpr unsigned vertex_components{2};
    constexpr unsigned unique_vertices{8};
    std::array<float, unique_vertices * vertex_components> vertices{
        // clang-format off
        // Vertices are
        -0.5f , -0.5f ,
         0.5f , -0.5f ,
         0.5f , 0.5f  ,
        -0.5f , 0.5f  ,
         0.0f , 1.0f  ,
         0.0f , -0.5f ,
         0.5f , -1.0f ,
        -0.5f , -1.0f
        // clang-format on
    };

    constexpr unsigned vertex_position{4 * 3};
    std::array<unsigned, vertex_position> indices{
        0,
        1,
        2,  // First Triangle
        2,
        3,
        0,  // Second Triangle
        3,
        4,
        2,  // Third Triangle
        5,
        6,
        7  // Fourth Triangle

    };

    GLuint vertex_buffer_object{};
    glGenBuffers(1, &vertex_buffer_object);
    constexpr auto vertex_buffer_type{GL_ARRAY_BUFFER};
    glBindBuffer(vertex_buffer_type, vertex_buffer_object);
    glBufferData(vertex_buffer_type, vertices.size() * sizeof(float), &vertices, GL_STATIC_DRAW);

    GLuint index_buffer_object{};
    glGenBuffers(1, &index_buffer_object);
    constexpr auto index_buffer_type{GL_ELEMENT_ARRAY_BUFFER};
    glBindBuffer(index_buffer_type, index_buffer_object);
    glBufferData(index_buffer_type, indices.size() * sizeof(unsigned), &indices, GL_STATIC_DRAW);

    constexpr auto normalized{GL_FALSE};
    constexpr auto vertex_datatype{GL_FLOAT};
    constexpr const void* offset{0};
    constexpr auto attribute_index{0};

    glVertexAttribPointer(
        attribute_index, vertex_components, vertex_datatype, normalized, sizeof(float) * vertex_components, offset);

    glEnableVertexAttribArray(attribute_index);

    ShaderProgram shader({"shaders/basic.vshader"}, {"shaders/basic.fshader"});

    shader.Bind();

    float value{0}, step{0.05};
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        if (value > 1.0f)
            step = -step;
        if (value < 0.0f)
            step = -step;
        value += step;
        std::cout << value << std::endl;

        shader.SetUniform("u_Color", value, 1.0f - value, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    shader.Unbind();

    glfwTerminate();

    return 0;
}
