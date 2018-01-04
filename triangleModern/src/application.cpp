#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>
int main(int, char**)
{
    if (!glfwInit())
    {
        std::cout << "Could not initialize glfw" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr unsigned vertex_position{3};
    constexpr unsigned vertex_components{2};

    std::array<float, vertex_position * vertex_components> positions{-0.5f, 0.5f, 0.0f, 0.5f, -0.5f, -0.5f};

    GLuint buffer_id{};
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions, GL_STATIC_DRAW);

    constexpr auto normalized{GL_FALSE};
    constexpr auto vertex_datatype{GL_FLOAT};
    constexpr const void* offset{0};
    constexpr auto attribute_index{0};

    glVertexAttribPointer(
        attribute_index, vertex_components, vertex_datatype, normalized, sizeof(float) * vertex_components, offset);

    glEnableVertexAttribArray(attribute_index);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, vertex_position);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
