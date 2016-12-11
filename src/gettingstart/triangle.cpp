#define GLFW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLfloat vertices[] = {
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const GLchar *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}";

const GLchar *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
		return;
	}

    GLuint VBO;
    glGenBuffers(1, &VBO);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
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

	glewExperimental = GL_TRUE;
	glewInit();

	drawWindow(window);

    glfwTerminate();
    return 0;
}
