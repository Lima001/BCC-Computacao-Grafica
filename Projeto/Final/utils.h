#ifndef UTILS_H
#define UTILS_H

#include "config.h"

//void framebufferSizeCallback(GLFWwindow* window, int width, int height);

GLFWwindow* initializeGLFW()
{
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed." << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {
        std::cout << "GLFW window creation failed." << std::endl;
        glfwTerminate();
        return nullptr;
    }

    //glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
};

unsigned int compileShader(unsigned int type, const char* source){

    unsigned int shader = glCreateShader(type);
    int success;
    char infoLog[512];

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
};

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader){
    
    unsigned int shaderProgram = glCreateProgram();
    int success;
    char infoLog[512];

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    return shaderProgram;
};

void processInput(GLFWwindow *window, float deltaTime, float &pitch, float &yaw, glm::vec3 &cameraPos, glm::vec3 &cameraFront, glm::vec3 &cameraUp){

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(5 * deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
        yaw -= 0.5f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
        yaw += 0.5f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        pitch -= 0.5f;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        pitch += 0.5f;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);  
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        yaw   = -90.0f;
        pitch =  0.0f;

    } 
};

glm::vec3 getPixelColor(glm::mat4 transformation, glm::vec3 coordinate){
    glm::vec4 clip = transformation * glm::vec4(coordinate.x, coordinate.y, coordinate.z, 1.0f);
    glm::vec3 normalized(clip.x/clip.w, clip.y/clip.w, clip.z/clip.w);

    glm::vec2 depthRange;
    glGetFloatv(GL_DEPTH_RANGE,&depthRange[0]);

    glm::vec2 window((SCR_WIDTH/2.0f*normalized.x)+SCR_WIDTH/2.0f, (SCR_HEIGHT/2.0f*normalized.y)+SCR_HEIGHT/2.0f);

    glm::vec3 color;
    glReadPixels(window.x,window.y,1,1,GL_RGB,GL_FLOAT,&color[0]);
    return color;
}

/*void framebufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
};*/

void setMat3x4ListUniform(unsigned int shaderProgram, const std::string &name, const glm::mat3x4 mat[]){
    glUniformMatrix3x4fv(glGetUniformLocation(shaderProgram, name.c_str()), MAX_BEZIER_CURVES, GL_TRUE, &mat[0][0][0]);
}

void setMat4Uniform(unsigned int shaderProgram, const std::string &name, const glm::mat4 &mat){
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void setVec3Uniform(unsigned int shaderProgram, const std::string &name, const glm::vec3 &vec){
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
}

void setVec4Uniform(unsigned int shaderProgram, const std::string &name, const glm::vec4 &vec){
    glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
}

void setFloatUniform(unsigned int shaderProgram, const std::string &name, const float &value){
    glUniform1fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value);
}

void setIntUniform(unsigned int shaderProgram, const std::string &name, const int &value){
    glUniform1iv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value);
}

#endif