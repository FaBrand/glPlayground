#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>
#include <memory>
#include <string>

GLuint CompileShader(GLuint type, const std::string& source)
{
    const auto id{glCreateShader(type)};

    const char* src = source.c_str();
    constexpr auto source_count{1};
    glShaderSource(id, source_count, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto output = std::make_unique<char[]>(length);

        glGetShaderInfoLog(id, length, &length, output.get());
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
                  << std::endl;

        std::cout << output.get() << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

GLuint CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(int, char**)
{
    if (!glfwInit())
    {
        std::cout << "Could not initialize glfw" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
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

    constexpr unsigned vertex_position{3};
    constexpr unsigned vertex_components{2};
    std::array<float, vertex_position * vertex_components> positions{-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

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

    if (!GL_VERSION_3_3)
    {
        std::cout << "OpenGl 3.3 not supported.\n"
                  << "Update Graphics Driver!" << std::endl;
        return -1;
    }

    std::string vertex_shader{
        "#version 330 core \n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n"};

    std::string fragment_shader{
        "#version 330 core \n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n"};

    GLuint shader = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader);

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
