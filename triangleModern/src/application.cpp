#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <array>
#include <iostream>

#include "FragmentShader.h"
#include "OpenGlDebugLogger.h"
#include "ShaderProgram.h"
#include "VertexShader.h"

#define GRAD2RAD(grad) ((grad) / 180.f * 3.14f)

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

    glm::mat4 model_matrix{glm::mat4(1.f)};
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(5, 0, 10),  // Camera is at (4,3,3), in World Space
                                        glm::vec3(0, 0, 0),   // and looks at the origin
                                        glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
                                        );
    glm::mat4 projection_matrix{glm::perspective(GRAD2RAD(55.0f), 640.f / 480.f, 0.1f, 20.0f)};

    ShaderProgram shader({"shaders/basic.vshader"}, {"shaders/basic.fshader"});
    shader.Bind();

    float value{0}, step{0.01};
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        value += step;

        // Rotate model itself around y-axis
        model_matrix = glm::rotate(GRAD2RAD(value), glm::vec3(0.f, 1.f, 0.f));

        // Rotate camera around model in a circular motion
        view_matrix =
            glm::lookAt(glm::vec3(std::cos(value) * 5, std::sin(value) * 5, 5),  // Camera is at (4,3,3), in World space
                        glm::vec3(0, 0, 0),                                      // and looks at the origin
                        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
                        );

        shader.SetUniform("u_model_matrix", model_matrix);
        shader.SetUniform("u_view_matrix", view_matrix);
        shader.SetUniform("u_projection_matrix", projection_matrix);
        shader.SetUniform("u_Color", std::fabs(std::sin(value)), std::fabs(std::cos(value)), 0.0f, 1.0f);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    shader.Unbind();

    glfwTerminate();

    return 0;
}
