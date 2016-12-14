#ifndef SHADERHELPER_H
#define SHADERHELPER_H
#include <GL/glew.h>
#include <string>

char *DEFAULT_VERTEX_PATH = "/home/luojianghua/Code/opengl/code/out/bin/gettingstart/vertex.sd";
char *DEFAULT_FRAGMENT_PATH = "/home/luojianghua/Code/opengl/code/out/bin/gettingstart/fragment.sd";
class ShaderHelper
{
public:
    ShaderHelper(char *vPath = DEFAULT_VERTEX_PATH, char *fPath = DEFAULT_FRAGMENT_PATH);
    ~ShaderHelper();
    void use();

private:
    bool createProgram();
    bool initSource();

private:
    char *mVertexShaderFilePath;
    char *mFragmentShaderFilePath;
    std::string mVertexShaderSource;
    std::string mFragmentShaderSource;

    GLuint mProgram;
};
#endif
