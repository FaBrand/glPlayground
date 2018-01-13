#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include <array>
#include <iostream>

#include "FragmentShader.h"
#include "IndexBuffer.h"
#include "ObjModel.h"
#include "OpenGlDebugLogger.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
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
    // Don't draw faces facing away from the camera
    glEnable(GL_CULL_FACE);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    std::cout << "OpenGl Version: " << glGetString(GL_VERSION) << std::endl;
    debugging.Enable();

    ObjModel tutorial_model{"res/blender_object.obj"};
    const auto& vertices = tutorial_model.GetOutVertices();
    const auto& normals = tutorial_model.GetOutNormals();
    const auto& indices = tutorial_model.GetIndicesVertices();

    GLuint vao{0};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Indices
    IndexBuffer ib(&(indices.front()), indices.size());

    // Vertices
    VertexBuffer vb(&(vertices.front()), vertices.size() * 3 * sizeof(float));

    const std::size_t vertex_attrib_array_index{0};
    const unsigned int vertex_components{3};
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

    VertexBuffer nb(&(normals.front()), normals.size() * 3 * sizeof(float));

    const std::size_t normals_attrib_array_index{1};
    glEnableVertexAttribArray(normals_attrib_array_index);
    glVertexAttribPointer(normals_attrib_array_index,         // zero-th attribute, must match layout in shader
                          vertex_components,                  // size
                          GL_FLOAT,                           // type
                          GL_FALSE,                           // normalized?
                          sizeof(float) * vertex_components,  // stride, if 0 is passed OpenGl calculates the stride as
                                                              // the product of the type and the size passed to it
                                                              // e.g. sizeof(GL_FLOAT) * vertex_components
                          0                                   // array buffer offset
                          );
    // Shader
    ShaderProgram shader({"shaders/basic.vshader"}, {"shaders/basic.fshader"});

    glBindVertexArray(0);
    shader.Unbind();
    nb.Unbind();
    vb.Unbind();
    ib.Unbind();

    glm::mat4 model_matrix{glm::mat4(1.f)};
    model_matrix = glm::rotate(GRAD2RAD(90), glm::vec3(-1, 0, 0));
    glm::mat4 view_matrix = glm::lookAt(glm::vec3(0, 0, 10),  // Camera is at (4,3,3), in World Space
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

        // Rotate camera around model in a circular motion
        view_matrix =
            glm::lookAt(glm::vec3(std::cos(value) * 5, std::sin(value) * 5, 15),  // Camera is at (4,3,3) Worldspace
                        glm::vec3(0, 0, 0),                                       // and looks at the origin
                        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
                        );

        shader.SetUniform("u_model_matrix", model_matrix);
        shader.SetUniform("u_view_matrix", view_matrix);
        shader.SetUniform("u_projection_matrix", projection_matrix);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    glfwTerminate();

    return 0;
}
