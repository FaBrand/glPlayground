#ifndef OPENGLDEBUGLOGGER_H
#define OPENGLDEBUGLOGGER_H

class OpenGlDebugLogger
{
  public:
    ~OpenGlDebugLogger();

    void SetContextDebugOption() const;
    void Enable();

  private:
    bool debugging_available{false};
    static const GLenum logging_level{GL_DEBUG_SEVERITY_LOW};

    void CheckDebuggerAvailability();
    void EnableDebugging() const;
    static void OpenGlDebugCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* userParam);
};

#endif /* OPENGLDEBUGLOGGER_H */
