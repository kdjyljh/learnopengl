#ifndef SHADERHELPER_H
#define SHADERHELPER_H
#include <GL/glew.h>
#include <string>

class ShaderHelper
{
public:
    ShaderHelper(std::string vPath = "/home/luojianghua/Code/opengl/code/out/bin/gettingstart/vertex.sd",
                 std::string fPath = "/home/luojianghua/Code/opengl/code/out/bin/gettingstart/fragment.sd");
    ~ShaderHelper();
    void use();
    bool setUniform(const char *name, float value);
    GLuint program() {return mProgram;}
    bool createProgram();

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
