#define GLFW_STATIC
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils.h"
#include "ShaderHelper.h"
#include "Camera.h"

GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

//helper must be defined before event loop and dou't know why
ShaderHelper objShaderhelper("materials.vsd", "materials.fsd");
ShaderHelper lightShaderhelper("colors.vsd", "colors_light.fsd");

glm::vec3 lightPos(1.0f, 1.0f, -2.0f);
glm::vec3 viewPos(1.0f, 1.0f, 1.0f);

//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void lighting()
{
    glm::vec3 objColor(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    objShaderhelper.setUniform("objColor", MY_GL_TYPE_3FV, glm::value_ptr(objColor));
    objShaderhelper.setUniform("lightColor", MY_GL_TYPE_3FV, glm::value_ptr(lightColor));
}

void drawWindow(GLFWwindow *window)
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //be carfaul abaout last two parameter, if wrong pass a parameter will make a huge mistake not easy to find out
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        //        model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        //        model = glm::translate(view, glm::vec3(-0.2f, 0.0f, -0.0f));
        view = glm::lookAt(viewPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
        objShaderhelper.use();
        objShaderhelper.setUniform("model", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(model));
        objShaderhelper.setUniform("view", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(view));
        objShaderhelper.setUniform("projection", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(projection));
//        glm::vec3 objColor(1.0f, 0.5f, 0.31f);
//        objShaderhelper.setUniform("objColor", MY_GL_TYPE_3FV, glm::value_ptr(objColor));
//        objShaderhelper.setUniform("lightColor", MY_GL_TYPE_3FV, glm::value_ptr(lightColor));
        objShaderhelper.setUniform("viewPos", MY_GL_TYPE_3FV, glm::value_ptr(viewPos));
        objShaderhelper.setUniform("material.ambient", MY_GL_TYPE_3FV, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        objShaderhelper.setUniform("material.diffuse", MY_GL_TYPE_3FV, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        objShaderhelper.setUniform("material.spacular", MY_GL_TYPE_3FV, glm::value_ptr(glm::vec3(0.5f)));
        GLfloat shininess = 32.0f;
        objShaderhelper.setUniform("material.shininess", MY_GL_TYPE_1F, &shininess);
        objShaderhelper.setUniform("light.position", MY_GL_TYPE_3FV, glm::value_ptr(lightPos));
        glm::vec3 lightColor(1.0f);
        lightColor.x = sin(glfwGetTime() * 1.2f);
        lightColor.y = sin(glfwGetTime() * 1.2f);
        lightColor.z = sin(glfwGetTime() * 10.2f);
        glm::vec3 lightAmbient;
        glm::vec3 lightDiffuse;
        lightAmbient = lightColor * glm::vec3(0.2f);
        lightDiffuse = lightColor * glm::vec3(1.0f);
        objShaderhelper.setUniform("light.ambient", MY_GL_TYPE_3FV, glm::value_ptr(lightAmbient));
        objShaderhelper.setUniform("light.diffuse", MY_GL_TYPE_3FV, glm::value_ptr(lightDiffuse));
        objShaderhelper.setUniform("light.specular", MY_GL_TYPE_3FV, glm::value_ptr(glm::vec3(1.0f)));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        model = glm::mat4();
        //        view = glm::mat4();
        //        model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        model = glm::translate(model, lightPos);
        lightShaderhelper.use();
        lightShaderhelper.setUniform("model", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(model));
        lightShaderhelper.setUniform("view", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(view));
        lightShaderhelper.setUniform("projection", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(projection));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "colors", nullptr, nullptr);
    if (nullptr == window) {
        printf("Fail to create window!!!!!!!!!\n");
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    //if you get a crash executing glGenVertexArrays(), try set glewExperimental to true
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    drawWindow(window);

    glCheckError();

    glfwTerminate();
    return 0;
}
