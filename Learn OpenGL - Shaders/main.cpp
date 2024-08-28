#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

const char* vertexShaderSource = "#version 410 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 410 core\n"
"out vec4 FragColor;\n"
"\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor, 1.0f);\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(600, 800, "LearnOpenGL - Shaders", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed creating GLFW Window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize glad!" << std::endl;
        return -1;
    }

    // start compiling shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int vertexSucess;
    char vertexInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexSucess);
    if (!vertexSucess)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, vertexInfoLog);
        std::cout << "ERROR::VERTEX::SHADER::COMPILE_ERROR: " << vertexInfoLog;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int fragmentSucess;
    char fragmentInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentSucess);
    if (!fragmentSucess)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentInfoLog);
        std::cout << "ERROR::FRAGMENT::SHADER::COMPILE_ERROR: " << fragmentInfoLog;
    }

    // compiling program shader
    unsigned int programShader;
    programShader = glCreateProgram();
    glAttachShader(programShader, vertexShader);
    glAttachShader(programShader, fragmentShader);
    glLinkProgram(programShader);

    int programSucess;
    char programInfoLog[512];
    glGetShaderiv(programShader, GL_LINK_STATUS, &programSucess);
    if (!programSucess)
    {
        glGetShaderInfoLog(programShader, 512, NULL, programInfoLog);
        std::cout << "ERROR::PROGRAM::SHADER::LINK_ERROR: " << programInfoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        //position          //colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind no VAO depois define os buffers de vértices e configura os atributos
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programShader);

        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        int vertexColorLocation = glGetUniformLocation(programShader, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}