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

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    std::cout << "OpenGl Version: " << glGetString(GL_VERSION) << std::endl;
    debugging.Enable();

    constexpr unsigned vertex_components{3};
    constexpr unsigned unique_vertices{8};
    std::array<float, unique_vertices * vertex_components> vertices{
        // clang-format off
        -0.5f , 0.5f  , 0.5f ,
        -0.5f , -0.5f , 0.5f ,
        0.5f  , -0.5f , 0.5f ,
        0.5f  , 0.5f  , 0.5f ,
        -0.5f , 0.5f  , -0.5f ,
        -0.5f , -0.5f , -0.5f ,
        0.5f  , -0.5f , -0.5f ,
        0.5f  , 0.5f  , -0.5f
        // clang-format on
    };

    std::array<float, unique_vertices * vertex_components> vertex_color_buffer{
        // clang-format off
        0.2f , 0.3f , 0.8f ,
        0.2f , 0.8f , 0.3f ,
        0.0f , 0.0f , 0.0f ,
        0.0f , 0.6f , 0.0f ,
        0.4f , 0.0f , 1.0f ,
        0.7f , 0.7f , 0.4f ,
        0.3f , 0.0f , 0.8f ,
        0.5f , 0.2f , 0.0f
        // clang-format on
    };

    constexpr unsigned vertex_position{6 * 4};
    std::array<unsigned, vertex_position> indices{
        0, 1, 2, 3,  // Front
        4, 5, 6, 7,  // Back
        0, 3, 7, 4,  // Top
        1, 2, 6, 5,  // Bottom
        3, 2, 6, 7,  // Right Side
        0, 1, 5, 4   // Left Side

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

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_color_buffer.size() * sizeof(float), &vertex_color_buffer, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,         // attribute. No particular reason for 1, but must match the layout in the shader
                          3,         // size
                          GL_FLOAT,  // type
                          GL_FALSE,  // normalized?
                          0,         // stride
                          0          // array buffer offset
                          );

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    shader.Unbind();

    glfwTerminate();

    return 0;
}
