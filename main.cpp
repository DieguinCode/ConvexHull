#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "alg.hpp"

using namespace std;

// Function to create and load a shader
static GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Compiling vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o vertex shader:\n" << infoLog << std::endl;
    }

    // Compiling fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o fragment shader:\n" << infoLog << std::endl;
    }

    // Link shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro ao linkar o programa de shader:\n" << infoLog << std::endl;
    }

    // Delete mid shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Function to draw input's points
static void drawInputPoints(const std::vector<vec2>& points, GLuint shaderProgram) {
    // Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object (VBO)
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec2), points.data(), GL_STATIC_DRAW);

    // Attributes's layout
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(posAttrib);

    glUseProgram(shaderProgram);
    glPointSize(5.0f); // Size

    glDrawArrays(GL_POINTS, 0, points.size());

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

// Function to draw Convex Hull's Edges
static void drawConvexHull(const std::vector<vec2>& points, GLuint shaderProgram) {
    // Check if there are enough points to build a convex hull
    if (points.size() < 3) {
        std::cerr << "É necessário pelo menos 3 pontos para desenhar o convex hull!" << std::endl;
        return;
    }

    // Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object (VBO) 
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(vec2), points.data(), GL_STATIC_DRAW);

    // Attributes's layout
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posAttrib, 2, GL_DOUBLE, GL_FALSE, sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(posAttrib);

    glUseProgram(shaderProgram);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw just the lines

    glDrawArrays(GL_LINE_LOOP, 0, points.size());

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

// Vertex Shader Source
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec2 minPos;
uniform vec2 maxPos;

void main() {
    vec2 scaledPos = (aPos - minPos) / (maxPos - minPos) * 2.0 - 1.0;
    gl_Position = vec4(scaledPos.x, scaledPos.y, 0.0, 1.0);
}
)";

// Fragment Shader Source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Cor branca
}
)";

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW!" << std::endl;
        return -1;
    }

    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Janela OpenGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    
    glfwMakeContextCurrent(window);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD!" << std::endl;
        return -1;
    }

    
    glViewport(0, 0, 800, 600);

    // Set the BackGround Color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black

    // Here would be possible to read some .obj input file
    std::vector<vec2> inputPoints = {
        {1, 1}, {2, 1}, {4, 3}, {3, 2}, {1, -3}, {3, -2}, {-2, -1}, {-4, -3}, {-3, 3}, {-2, 2}
    };

    // Call the magic!
    std::vector<vec2> convexHull = mergeHull(inputPoints);
    
    //I would like to see at a terminal too
    for (int i = 0; i < convexHull.size(); i++) {
        cout << convexHull.at(i) << endl;
    }

    // Build the shader program
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Calculate the limits (points)
    vec2 minPos = inputPoints[0];
    vec2 maxPos = inputPoints[0];
    for (const auto& point : inputPoints) {
        minPos.x = std::min(minPos.x, point.x);
        minPos.y = std::min(minPos.y, point.y);
        maxPos.x = std::max(maxPos.x, point.x);
        maxPos.y = std::max(maxPos.y, point.y);
    }

    // Add a padding to improve visualization
    double padding = 0.1 * std::max(maxPos.x - minPos.x, maxPos.y - minPos.y);
    minPos.x -= padding;
    minPos.y -= padding;
    maxPos.x += padding;
    maxPos.y += padding;

    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);

        // To define Shader's uniforms
        glUseProgram(shaderProgram);
        GLint minPosLoc = glGetUniformLocation(shaderProgram, "minPos");
        glUniform2f(minPosLoc, minPos.x, minPos.y);
        GLint maxPosLoc = glGetUniformLocation(shaderProgram, "maxPos");
        glUniform2f(maxPosLoc, maxPos.x, maxPos.y);

        // Draw the points
        drawInputPoints(inputPoints, shaderProgram);

        // Draw the edges
        drawConvexHull(convexHull, shaderProgram);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
