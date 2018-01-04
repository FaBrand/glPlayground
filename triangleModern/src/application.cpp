#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

void APIENTRY OpenGlDebugCallbackFunction(GLenum /* source */,
                                          GLenum type,
                                          GLuint id,
                                          GLenum severity,
                                          GLsizei /* length */,
                                          const GLchar* message,
                                          const void* /* userParam */)
{

    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "type: ";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "OTHER";
            break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "HIGH";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "NOTIFICATION";
            break;
        default:
            std::cout << "UNKNOWN";
            break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

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
    if (!GL_VERSION_4_3)
    {
        std::cout << "OpenGl 4.3 not supported.\n"
                  << "Update Graphics Driver!" << std::endl;
        return -1;
    }

    if (!glfwInit())
    {
        std::cout << "Could not initialize glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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

    if (glDebugMessageCallback)
    {
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGlDebugCallbackFunction, nullptr);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
    }
    else
    {
        std::cout << "glDebugMessageCallback not available" << std::endl;
    }

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

    std::string vertex_shader_file{"shaders/basic.vshader"};
    std::string fragment_shader_file{"shaders/basic.fshader"};

    GLuint shader = CreateShader(vertex_shader_file, fragment_shader_file);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();

    return 0;
}
