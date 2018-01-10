#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <array>
#include <iostream>

#include "FragmentShader.h"
#include "IndexBuffer.h"
#include "OpenGlDebugLogger.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

#define GRAD2RAD(grad) ((grad) / 180.f * 3.14f)

template <typename T>
void SetYWidthOfBox(float height, T& vertex_array)
{
    height = std::max(std::fabs(height), 0.1f);

    float top = height / 2;
    float bottom = -height / 2;

    // Set the Y coordinate of the vertices.
    // The z-coordinate is pointing towards the camera
    vertex_array[1] = top;
    vertex_array[4] = bottom;
    vertex_array[7] = bottom;
    vertex_array[10] = top;

    vertex_array[13] = top;
    vertex_array[16] = bottom;
    vertex_array[19] = bottom;
    vertex_array[22] = top;
}

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
    // Don't draw faces facing away from the camera
    glEnable(GL_CULL_FACE);
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
        4, 7, 6, 5,  // Back
        0, 3, 7, 4,  // Top
        1, 5, 6, 2,  // Bottom
        3, 2, 6, 7,  // Right Side
        0, 4, 5, 1   // Left Side

    };

    GLuint vao{0};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Vertices
    VertexBuffer vb(&vertices, vertices.size() * sizeof(float));

    const std::size_t vertex_attrib_array_index{0};
    glEnableVertexAttribArray(vertex_attrib_array_index);
    glVertexAttribPointer(vertex_attrib_array_index,          // zero-th attribute, must match layout in shader
                          vertex_components,                  // size
                          GL_FLOAT,                           // type
                          GL_FALSE,                           // normalized?
                          sizeof(float) * vertex_components,  // stride, if 0 is passed OpenGl calculates the stride as
                                                              // the product of the type and the size passed to it
                                                              // e.g. sizeof(GL_FLOAT) * vertex_components
                          0                                   // array buffer offset
                          );

    // Color
    VertexBuffer cb(&vertex_color_buffer, vertex_color_buffer.size() * sizeof(float));

    const std::size_t color_attrib_array_index{1};
    glEnableVertexAttribArray(color_attrib_array_index);
    glVertexAttribPointer(color_attrib_array_index,           // first attribute, must match layout in shader
                          vertex_components,                  // size
                          GL_FLOAT,                           // type
                          GL_FALSE,                           // normalized?
                          sizeof(float) * vertex_components,  // stride, if 0 is passed OpenGl calculates the stride as
                                                              // the product of the type and the size passed to it
                                                              // e.g. sizeof(GL_FLOAT) * vertex_components
                          0                                   // array buffer offset
                          );

    // Indices
    IndexBuffer ib(&indices, indices.size());

    // Shader
    ShaderProgram shader({"shaders/basic.vshader"}, {"shaders/basic.fshader"});

    glBindVertexArray(0);
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    glm::mat4 model_matrix{glm::mat4(1.f)};
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(5, 0, 10),  // Camera is at (4,3,3), in World Space
                                        glm::vec3(0, 0, 0),   // and looks at the origin
                                        glm::vec3(0, 1, 0)    // Head is up (set to 0,-1,0 to look upside-down)
                                        );
    glm::mat4 projection_matrix{glm::perspective(GRAD2RAD(55.0f), 640.f / 480.f, 0.1f, 20.0f)};

    float value{0}, step{0.01};
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        value += step;

        glBindVertexArray(vao);
        shader.Bind();
        vb.Bind();
        ib.Bind();

        // Rotate model itself around y-axis
        model_matrix = glm::rotate(GRAD2RAD(value * 10), glm::vec3(0.f, 1.f, 0.f));

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
    vb.Unbind();
    ib.Unbind();

    glfwTerminate();

    return 0;
}
