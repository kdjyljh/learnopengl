#define GLFW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

GLfloat vertices[] = {
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.0f, 0.0f
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

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        printf("---------!!!error occurs!!!-------------------\n");
        printf("error code: %d\n", errorCode);
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const GLvoid *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    printf("------------------\n");
    printf("Debug Message:%d %s\n", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             printf("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other"); break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               printf("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:               printf("Type: Other"); break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
    }

    printf("\n\n");
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
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
