#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <math.h>

#include "data.h"
#include "utils.h"
#include "shaders1.h"
#include "shaders2.h"

int main(){

    // Init GLFW and create Window
    GLFWwindow* window = initializeGLFW();
    if (!window)
        return -1;

    // Opengl State
    glEnable(GL_DEPTH_TEST);

    // Shaders
    unsigned int vertexShader1 = compileShader(GL_VERTEX_SHADER, vertexShaderSource1);
    unsigned int fragmentShader1 = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource1);
    unsigned int shaderProgram1 = createShaderProgram(vertexShader1, fragmentShader1);

    unsigned int vertexShader2 = compileShader(GL_VERTEX_SHADER, vertexShaderSource2);
    unsigned int fragmentShader2 = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource2);
    unsigned int shaderProgram2 = createShaderProgram(vertexShader2, fragmentShader2);


    glDeleteShader(vertexShader1);
    glDeleteShader(fragmentShader1);
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader2);


    // Create and bind VAOs and VBOs for cube and line
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    unsigned int lineVAO, lineVBO, lineEBO;
    unsigned int sceneVAO, sceneVBO, sceneEBO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &lineEBO);
    glGenVertexArrays(1, &sceneVAO);
    glGenBuffers(1, &sceneVBO);
    glGenBuffers(1, &sceneEBO);

    // Upload cube data
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Upload line data
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineData), lineData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Upload scene data
    glBindVertexArray(sceneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sceneData), sceneData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sceneEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sceneIndices), sceneIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Frame calculation
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        // Updating coordinate matrices
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);

        // Use shader program - bezier curve
        glUseProgram(shaderProgram2);

        setMat4Uniform(shaderProgram2, "projection", projection);
        setMat4Uniform(shaderProgram2, "view", view);
        setMat4Uniform(shaderProgram2, "model", model);
        setMat4Uniform(shaderProgram2, "inverseMat", glm::inverse(projection*view));
        setVec3Uniform(shaderProgram2, "p0", p0);
        setVec3Uniform(shaderProgram2, "p1", p1);
        setVec3Uniform(shaderProgram2, "p2", p2);
        setVec3Uniform(shaderProgram2, "p3", p3);

        glBindVertexArray(sceneVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // Use the shader program - Objects
        glUseProgram(shaderProgram1);

        setMat4Uniform(shaderProgram1, "projection", projection);
        setMat4Uniform(shaderProgram1, "view", view);
        setMat4Uniform(shaderProgram1, "model", model);

        // Draw the axes
        glBindVertexArray(lineVAO);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        model = glm::translate(model, tposition);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
        setMat4Uniform(shaderProgram1, "model", model);

        // Draw the cube
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteVertexArrays(1, &lineVAO);
    glDeleteBuffers(1, &lineVBO);
    glDeleteBuffers(1, &lineEBO);
    glDeleteVertexArrays(1, &sceneVAO);
    glDeleteBuffers(1, &sceneVBO);
    glDeleteBuffers(1, &sceneEBO);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);
    glfwTerminate();

    return 0;
}