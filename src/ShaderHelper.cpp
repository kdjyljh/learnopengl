#include <iostream>
#include <fstream>
#include <sstream>
#include <memory.h>
#include <unistd.h>

#include "ShaderHelper.h"

ShaderHelper::ShaderHelper(std::string vPath, std::string fPath)
    : SHADER_DIR(getRuntimeDir() + "/resources/shader/")
    , mVertexShaderFilePath(SHADER_DIR + vPath)
    , mFragmentShaderFilePath(SHADER_DIR + fPath)
{
    mProgram = 0;
}

ShaderHelper::~ShaderHelper()
{

}

bool ShaderHelper::initSource()
{
    mVertexShaderSource.clear();
    mFragmentShaderSource.clear();

//    if (mVertexShaderFilePath[0] != '/') {
//        mVertexShaderFilePath = (std::string(getcwd(NULL, 0)) + "/" + std::string(mVertexShaderFilePath));
//    }
//    if (mFragmentShaderFilePath[0] != '/') {
//        mFragmentShaderFilePath = std::string(getcwd(NULL, 0)) + "/" + std::string(mFragmentShaderFilePath);
//    }

    std::ifstream vinfile(mVertexShaderFilePath);
    std::stringstream stream;
    if (!vinfile.is_open()) {
        std::cout << "open file " << mVertexShaderFilePath << " failed error:" << strerror(errno) << std::endl;
        return false;
    }
    stream << vinfile.rdbuf();
    mVertexShaderSource = stream.str();
    stream.str("");
    vinfile.close();

    std::ifstream finfile(mFragmentShaderFilePath);
    if (!finfile.is_open()) {
        std::cout << "open file " << mFragmentShaderFilePath << " failed error:" << strerror(errno) << std::endl;
        return false;
    }
    stream << finfile.rdbuf();
    mFragmentShaderSource = stream.str();
    finfile.close();

    return true;
}

bool ShaderHelper::createProgram()
{
    if (!initSource()) {
        return false;
    }

    GLint success = 0;
    GLchar infoLog[512];
    memset(infoLog, 0, sizeof(infoLog));
    const char *source = NULL;

    source = mVertexShaderSource.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "compile vertex shader failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = mFragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "compile fragment shader failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "link program failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    mProgram = program;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

bool ShaderHelper::setUniform(const char *name, MY_GL_TYPE type, const void *value, GLsizei count)
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

    switch(type) {
    case MY_GL_TYPE_1F:
    {
        glUniform1f(location, *static_cast<GLfloat*>(const_cast<void*>(value)));
        break;
    }
    case MY_GL_TYPE_1UI:
    {
        glUniform1ui(location, *static_cast<GLuint*>(const_cast<void*>(value)));
        break;
    }
    case MY_GL_TYPE_1I:
    {
        glUniform1i(location, *static_cast<GLint*>(const_cast<void*>(value)));
        break;
    }
    case MY_GL_TYPE_MATRIX4FV:
    {
        glUniformMatrix4fv(location, count, GL_FALSE, static_cast<GLfloat*>(const_cast<void*>(value)));
        break;
    }
    case MY_GL_TYPE_3FV:
    {
        glUniform3fv(location, count, static_cast<GLfloat*>(const_cast<void*>(value)));
        break;
    }
    defalte:
        break;
    }

    return true;
}

void ShaderHelper::use()
{
    if (0 == mProgram) {
        if (!createProgram()) {
            return;
        }
    }

    glUseProgram(mProgram);
}

//template<class T>
//bool ShaderHelper::setUniform(const char *name, T value)

