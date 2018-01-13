#include <functional>
#include <iomanip>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include "OpenGlDebugLogger.h"
void OpenGlDebugLogger::Enable()
{
    CheckDebuggerAvailability();

    if (debugging_available_)
    {
        EnableDebugging();
    }
    else
    {
        std::cout << "Debugging not available" << std::endl;
    }
}

void OpenGlDebugLogger::EnableDebugging() const
{
    std::cout << "Registered OpenGL debug callback " << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(OpenGlDebugLogger::OpenGlDebugCallback, nullptr);

    GLuint unusedIds{0};
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
}

void OpenGlDebugLogger::CheckDebuggerAvailability()
{
    if (!GL_VERSION_4_3)
    {
        std::cout << "OpenGl 4.3 not supported.\n"
                  << "Update Graphics Driver!" << std::endl;
        debugging_available_ = false;
    }
    else if (glDebugMessageCallback)
    {
        debugging_available_ = true;
    }
    else
    {
        debugging_available_ = false;
    }
}

void OpenGlDebugLogger::SetContextDebugOption() const
{
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}

OpenGlDebugLogger::~OpenGlDebugLogger()
{
    if (debugging_available_)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }
}

void OpenGlDebugLogger::OpenGlDebugCallback(GLenum /* source */,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei /* length */,
                                            const GLchar* message,
                                            const void* /* userParam */)
{
    if (severity < logging_level)
        return;

    const std::map<GLenum, std::string> debug_type_map{{GL_DEBUG_TYPE_ERROR, "ERROR"},
                                                       {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
                                                       {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
                                                       {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
                                                       {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
                                                       {GL_DEBUG_TYPE_OTHER, "OTHER"}};

    const std::map<GLenum, std::string> severity_map{{GL_DEBUG_SEVERITY_LOW, "LOW"},
                                                     {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
                                                     {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
                                                     {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}};

    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    try
    {
        std::cout << "message: " << message << std::endl;
        std::cout << "type: " << debug_type_map.at(type) << std::endl;
        std::cout << "id: " << id << std::endl;
        std::cout << "severity: " << severity_map.at(severity) << std::endl;
    }
    catch (std::out_of_range& e)
    {
        std::cout << "Unknown type or severity debug message:\n" << std::quoted(e.what()) << std::endl;
    }
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}
