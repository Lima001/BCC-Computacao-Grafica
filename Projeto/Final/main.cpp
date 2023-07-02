#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <math.h>

#include "config.h"
#include "graphical_obj_data.h"
#include "utils.h"
#include "obj_shader.h"
#include "spline_shader.h"

int main(){
    // Frame
    float currentFrame,  lastFrame=0.0f, deltaTime=0.0f;
    
    // Camera
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    
    float yaw   = -90.0f;
    float pitch =  0.0f;
    float fov   =  45.0f;

    // Coordinates
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    // Init GLFW and create Window
    GLFWwindow* window = initializeGLFW();
    if (!window)
        return -1;

    // Opengl State
    glEnable(GL_DEPTH_TEST);

    // Shaders
    unsigned int objVertexShader = compileShader(GL_VERTEX_SHADER, objVertexShaderSource);
    unsigned int objFragmentShader = compileShader(GL_FRAGMENT_SHADER, objFragmentShaderSource);
    unsigned int objShaderProgram = createShaderProgram(objVertexShader, objFragmentShader);

    unsigned int splineVertexShader = compileShader(GL_VERTEX_SHADER, splineVertexShaderSource);
    unsigned int splineFragmentShader = compileShader(GL_FRAGMENT_SHADER, splineFragmentShaderSource);
    unsigned int splineShaderProgram = createShaderProgram(splineVertexShader, splineFragmentShader);


    glDeleteShader(objVertexShader);
    glDeleteShader(objFragmentShader);
    glDeleteShader(splineVertexShader);
    glDeleteShader(splineFragmentShader);

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

    while (!glfwWindowShouldClose(window)){    
        currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(worldBackgroundColor.x, worldBackgroundColor.y, worldBackgroundColor.z, worldBackgroundColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window,deltaTime,pitch,yaw,cameraPos,cameraFront,cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);

        // Use shader program - bezier curve
        glUseProgram(splineShaderProgram);

        setMat4Uniform(splineShaderProgram, "projection", projection);
        setMat4Uniform(splineShaderProgram, "view", view);
        setMat4Uniform(splineShaderProgram, "model", model);
        setMat4Uniform(splineShaderProgram, "inverseMat", glm::inverse(projection*view));
        setVec3Uniform(splineShaderProgram, "p0", p0);
        setVec3Uniform(splineShaderProgram, "p1", p1);
        setVec3Uniform(splineShaderProgram, "p2", p2);
        setVec3Uniform(splineShaderProgram, "p3", p3);
        //setMat3x4ListUniform(splineShaderProgram, "cp", controlPointsList);
        //setIntUniform(splineShaderProgram, "n", N_BEZIER_CURVE);
        setVec4Uniform(splineShaderProgram, "b_color", splineBackgroundColor);
        setVec4Uniform(splineShaderProgram, "l_color", splineFillColor);
        setVec4Uniform(splineShaderProgram, "p_color", splinePointColor);
        setFloatUniform(splineShaderProgram, "l_width", lineWidth);
        setFloatUniform(splineShaderProgram, "p_width", pointWidth);

        glBindVertexArray(sceneVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Use the shader program - Objects
        glUseProgram(objShaderProgram);

        setMat4Uniform(objShaderProgram, "projection", projection);
        setMat4Uniform(objShaderProgram, "view", view);
        setMat4Uniform(objShaderProgram, "model", model);

        // Draw the axes
        glBindVertexArray(lineVAO);
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        for (int i=0; i<N_CUBES; i++){
            model = glm::mat4(1.0f);
            model = glm::rotate(model, -theta, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(x,0.0f,z));
            model = glm::translate(model, cubeTransfomation[2*i]);

            if (i==N_CUBES-1 || i==N_CUBES-2){
                glm::vec3 color = getPixelColor((projection*view*model),cubeTransfomation[2*i]);
                std::cout << "Color S" << N_CUBES-i << " " << color.x << ", " << color.y << ", " <<color.z << std::endl;
            }

            model = glm::scale(model, cubeTransfomation[2*i+1]);
            
            setMat4Uniform(objShaderProgram, "model", model);

            glBindVertexArray(cubeVAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        // Movement
        theta += w; 
        x += glm::sin(theta)*v;
        z += glm::cos(theta)*v;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

};