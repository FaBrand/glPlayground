#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

GLuint CompileShader(GLuint type, const std::string& source_file_path)
{

    std::ifstream shader_file(source_file_path);
    if (!shader_file.is_open() || !shader_file.good())
    {
        std::cout << "Error while opening shader_file " << source_file_path << std::endl;
    }

    std::string source{std::istreambuf_iterator<char>(shader_file), std::istreambuf_iterator<char>()};

    const char* source_string = source.c_str();
    const auto shader_id{glCreateShader(type)};
    constexpr auto source_count{1};
    glShaderSource(shader_id, source_count, &source_string, nullptr);
    glCompileShader(shader_id);

    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        auto output = std::make_unique<char[]>(length);

        glGetShaderInfoLog(shader_id, length, &length, output.get());
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
                  << std::endl;

        std::cout << output.get() << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }
    return shader_id;
}

GLuint CreateShader(const std::string& vertex_shader_file, const std::string& fragment_shader_file)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertex_shader_file);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_file);

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

    std::string vertex_shader_file{"shaders/basic.vshader"};
    std::string fragment_shader_file{"shaders/basic.fshader"};

    GLuint shader = CreateShader(vertex_shader_file, fragment_shader_file);
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
