#define GLFW_STATIC
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "utils.h"
#include "ShaderHelper.h"

GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

// GLfloat triangle_left[] = {
//     -0.5f, 0.5f, 0.0f,
//     -0.5f, -0.5f, 0.0f,
//     0.0f, 0.0f, 0.0f
// };

// GLfloat triangle_right[] = {
//     0.5f, 0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f, 0.0f, 0.0f
// };

// GLfloat vertices[] = {
//      0.5f,  0.5f, 0.0f,  // Top Right
//      0.5f, -0.5f, 0.0f,  // Bottom Right
//     -0.5f, -0.5f, 0.0f,  // Bottom Left
//     -0.5f,  0.5f, 0.0f   // Top Left 
// };

GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
};

const GLchar *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 myColor;\n"
    "out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "   vertexColor = vec4(myColor.x, myColor.y, myColor.z, 1.0f);\n"
	"}";

const GLchar *fragmentShaderSource = 
	"#version 330 core\n"
    "in vec4 vertexColor;"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"	color = vertexColor;\n"
	"}";

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLuint createShaderProgram()
{
	GLuint shaderProgram = 0;
    GLint success = 1;
    GLchar infoLOG[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS,  &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLOG);
        printf("ERROR: vertex shader compilation failed:%s\n", infoLOG);
		return shaderProgram;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLOG);
        printf("ERROR: fragment shader compile failed:%s\n", infoLOG);
		return shaderProgram;
    }
	
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLOG);
		printf("ERROR: shader program link failed:%s\n", infoLOG);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void drawWindow(GLFWwindow *window)
{
	GLuint shaderProgram = createShaderProgram();
	if (!shaderProgram) {
        printf("create shader program failed!!!!!!!!!!!!\n");
		return;
	}

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // GLfloat timeValue = glfwGetTime();
        // GLfloat greenColor = sin(timeValue) / 2 + 0.5;
        // GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//		glUseProgram(shaderProgram);

        ShaderHelper helper;
        helper.use();

        // glUniform4f(vertexColorLocation, 0.0f, greenColor, 0.0f, 1.0f);

		glBindVertexArray(VAO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        printf("open debug output success\n");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else {
        printf("open debug output failed\n");
    }

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

    //if you get a crash executing glGenVertexArrays(), try set glewExperimental to true
	glewExperimental = GL_TRUE;
	glewInit();
    glGetError();

	drawWindow(window);

    glfwTerminate();
    return 0;
}
