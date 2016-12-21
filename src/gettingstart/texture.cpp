#define GLFW_STATIC
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "utils.h"
#include "ShaderHelper.h"
#include "soil/SOIL.h"
#include "environment.cmake.h"

GLfloat vertices[] = {
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
};

GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void drawWindow(GLFWwindow *window)
{
    GLuint VBO;
    glGenBuffers(1, &VBO);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
    GLuint EBO;
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    int width = 0;
    int height = 0;
    unsigned char *image = NULL;

    GLuint texture0 = 0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = SOIL_load_image("/home/luojianghua/Code/opengl/code/out/bin/gettingstart/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (NULL == image) {
        std::cout << "open file error: " << SOIL_last_result() << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    image = NULL;
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texture1 = 0;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    image = SOIL_load_image("/home/luojianghua/Code/opengl/code/out/bin/gettingstart/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    if (NULL == image) {
        std::cout << "open file error: " << SOIL_last_result() << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    image = NULL;
    glBindTexture(GL_TEXTURE_2D, 0);

    ShaderHelper shaderhelper;//helper must be defined before event loop and dou't know why
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//        shaderhelper.setUniform("xoffset", 0.5f);
        shaderhelper.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        shaderhelper.setUniform("textureSampler0", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shaderhelper.setUniform("textureSampler0", 1);

        glBindVertexArray(VAO);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "hello triangle", nullptr, nullptr);
    if (nullptr == window) {
        printf("Fail to create window!!!!!!!!!\n");
        glfwTerminate();
        return -1;
    }
    
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    int width = 0;
    int hight = 0;
    glfwGetFramebufferSize(window, &width, &hight);
    glViewport(0, 0, width, hight);

    //if get a crash executing glGenVertexArrays(), try set glewExperimental to true
	glewExperimental = GL_TRUE;
    glewInit();

    drawWindow(window);

    glfwTerminate();
    return 0;
}
