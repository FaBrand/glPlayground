#include <functional>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGlDebugLogger.h"

void OpenGlDebugLogger::Enable()
{
    CheckDebuggerAvailability();

    if (debugging_available)
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

    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
}

void OpenGlDebugLogger::CheckDebuggerAvailability()
{
    if (!GL_VERSION_4_3)
    {
        std::cout << "OpenGl 4.3 not supported.\n"
                  << "Update Graphics Driver!" << std::endl;
        debugging_available = false;
    }
    else if (glDebugMessageCallback)
    {
        debugging_available = true;
    }
    else
    {
        debugging_available = false;
    }
}

void OpenGlDebugLogger::SetContextDebugOption() const
{
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}

OpenGlDebugLogger::~OpenGlDebugLogger()
{
    if (debugging_available)
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
