#include <iostream>
#include <fstream>
#include <sstream>
#include <memory.h>
#include "ShaderHelper.h"

ShaderHelper::ShaderHelper(char *vPath, char *fPath)
{
    if (NULL != vPath) {
        mVertexShaderFilePath = vPath;
    }
    if (NULL != fPath) {
        mFragmentShaderFilePath = fPath;
    }

    mProgram = 0;
}

ShaderHelper::~ShaderHelper()
{

}

bool ShaderHelper::initSource()
{
    if (mVertexShaderSource.empty()) {
        std::ifstream infile(mVertexShaderFilePath);
        if (!infile.is_open()) {
            std::cout << "open file" << mVertexShaderFilePath << "error errorno=" << strerror(errno) << std::endl;
            return false;
        }
        std::stringstream stream;
        stream << infile.rdbuf();
        mVertexShaderSource = stream.str();
    }

    if (mFragmentShaderSource.empty()) {
        std::ifstream infile(mFragmentShaderFilePath);
        if (!infile.is_open()) {
            std::cout << "open file" << mFragmentShaderFilePath << "error errorno=" << strerror(errno) << std::endl;
            return false;
        }
        std::stringstream stream;
        stream << infile.rdbuf();
        mFragmentShaderSource = stream.str();
    }
}

bool ShaderHelper::createProgram()
{
    if (!initSource()) {
        return false;
    }

    GLint success = 1;
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

void ShaderHelper::use()
{
    if (0 == mProgram) {
        if (!createProgram()) {
            return;
        }
    }

    glUseProgram(mProgram);
}
