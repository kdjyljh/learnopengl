#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum MY_GL_TYPE
{
    MY_GL_TYPE_1F,
    MY_GL_TYPE_1UI,
    MY_GL_TYPE_1I,
    MY_GL_TYPE_MATRIX4FV
};

GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const GLvoid *userParam);

std::string getRuntimeDir();
#endif
