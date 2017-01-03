#define GLFW_STATIC
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "soil/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "utils.h"
#include "ShaderHelper.h"
#include "environment.cmake.h"

//GLfloat vertices[] = {
//     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
//     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
//    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
//    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
//};
GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

GLfloat speed = 10.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFram = 0.0f;

GLfloat pitch = 0.0f;
GLfloat yaw = 0.0f;

bool firstMouse = true;

GLfloat lastXPos = 400.0f;
GLfloat lastYPos = 300.0f;

glm::vec3 camPos(0.0f, 0.0f, -1.0f);
glm::vec3 camFront(0.0f, 0.0f, -5.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);

GLfloat aspect = 45.0f;

bool press_keys[1024] = {0};


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (GLFW_PRESS == action) {
        press_keys[key] = true;
    }
    if (GLFW_RELEASE == action) {
        press_keys[key] = false;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if(firstMouse) {
        lastXPos = xpos;
        lastYPos = ypos;
        firstMouse = false;
    }

    GLfloat sensitive = 0.05f;
    yaw += (xpos - lastXPos) * sensitive;
    pitch += (lastYPos - ypos) * sensitive;
    lastXPos = xpos;
    lastYPos = ypos;

    if (pitch > 89) {
        pitch = 89;
    }
    else if (pitch < -89) {
        pitch = -89;
    }

    camFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camFront.y = sin(glm::radians(pitch));
    camFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camFront = glm::normalize(camFront);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    aspect -= yoffset;

    if (aspect > 45.0f) {
        aspect = 45.0f;
    }
    else if (aspect < 1.0f) {
        aspect = 1.0f;
    }
}

void doMovement()
{
    GLfloat tmpspeed = deltaTime * speed;
    if (press_keys[GLFW_KEY_A]) {
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * tmpspeed;
    }
    if (press_keys[GLFW_KEY_D]) {
        camPos += glm::normalize(glm::cross(camFront, camUp)) * tmpspeed;
    }
    if (press_keys[GLFW_KEY_W]) {
        camPos += camFront * tmpspeed;
    }
    if (press_keys[GLFW_KEY_S]) {
        camPos -= camFront * tmpspeed;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    int width = 0;
    int height = 0;
    unsigned char *image = NULL;
    std::string file_dir;

    GLuint texture0 = 0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    file_dir = (getRuntimeDir() + "/resources/images/container.jpg");
    image = SOIL_load_image(file_dir.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (NULL == image) {
        std::cout << "open file:" << file_dir << "error:" << SOIL_last_result() << std::endl;
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
    file_dir = (getRuntimeDir() + "/resources/images/awesomeface.png");
    image = SOIL_load_image(file_dir.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if (NULL == image) {
        std::cout << "open file:" << file_dir << "error:" << SOIL_last_result() << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    image = NULL;
    glBindTexture(GL_TEXTURE_2D, 0);

    //helper must be defined before event loop and dou't know why
    ShaderHelper shaderhelper("coordinate.vsd");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderhelper.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        shaderhelper.setUniform("textureSampler0", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shaderhelper.setUniform("textureSampler0", 1);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(aspect), (float)width / (float)height, 0.1f, 100.0f);
        shaderhelper.setUniform("projection", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(projection));

        glm::mat4 view;
        deltaTime = glfwGetTime() - lastFram;
        lastFram = glfwGetTime();
        doMovement();
        view = glm::lookAt(camPos, camPos + camFront, camUp);
//            GLfloat radius = 10.0f;
//            GLfloat camX = radius * sin(glfwGetTime());
//            GLfloat camZ = radius * cos(glfwGetTime());
//            view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shaderhelper.setUniform("view", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(view));

        for (int i = 0; i < 10; ++i) {
            glBindVertexArray(VAO);

            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians((float)sin(glfwGetTime()) * (i % 3) * 30.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderhelper.setUniform("model", MY_GL_TYPE_MATRIX4FV, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    int width = 0;
    int hight = 0;
    glfwGetFramebufferSize(window, &width, &hight);

    //if get a crash executing glGenVertexArrays(), try set glewExperimental to true
    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, width, hight);

    glEnable(GL_DEPTH_TEST);

    drawWindow(window);

    glfwTerminate();
    return 0;
}
