#ifndef SHADERHELPER_H
#define SHADERHELPER_H
#include <GL/glew.h>
#include <string>
#include <typeinfo>

#include "utils.h"

class ShaderHelper
{
public:
    ShaderHelper(std::string vPath = getRuntimeDir() + "/resources/shader/texture.vsd",
                 std::string fPath = getRuntimeDir() + "/resources/shader/texture.fsd");
    ~ShaderHelper();
    void use();
    GLuint program() {return mProgram;}
    bool createProgram();

    template<class T>
    bool setUniform(const char *name, T value)
    {
        if (0 == mProgram) {
            if (!createProgram()) {
                return false;
            }
        }

        GLint location = glGetUniformLocation(mProgram, name);
        if (-1 == location) {
            return false;
        }

        GLint TYPE_GLINT = 0;
        GLuint TYPE_GLUINT = 0u;
        GLfloat TYPE_GLFLOAT = 1.0f;
        if (typeid(T).name() == typeid(TYPE_GLFLOAT).name()) {
            glUniform1f(location, value);
        }
        else if (typeid(T).name() == typeid(TYPE_GLUINT).name()) {
            glUniform1ui(location, value);
        }
        else if (typeid(T).name() == typeid(TYPE_GLINT).name()) {
            glUniform1i(location, value);
        }
    //    glCheckError(); //there will always be a error:INVALID_OPERATION and dou't know why
        return true;
    }

private:
    bool initSource();

private:
    std::string mVertexShaderFilePath;
    std::string mFragmentShaderFilePath;
    std::string mVertexShaderSource;
    std::string mFragmentShaderSource;

    GLuint mProgram;
};
#endif
