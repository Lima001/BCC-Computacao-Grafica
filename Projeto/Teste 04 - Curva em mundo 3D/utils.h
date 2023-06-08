void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float yaw   = -90.0f;
float pitch =  0.0f;
float fov   =  45.0f;

// Coordinates
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
glm::vec3 tposition  = glm::vec3(0.0f, 0.0f,  0.0f);
float angle = 0;

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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

void setMat4Uniform(unsigned int shaderProgram, const std::string &name, const glm::mat4 &mat){
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void setVec3Uniform(unsigned int shaderProgram, const std::string &name, const glm::vec3 &vec){
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
}

void setVec2Uniform(unsigned int shaderProgram, const std::string &name, const glm::vec2 &vec){
    glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &vec[0]);
}

void processInput(GLFWwindow *window){

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

        tposition = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        tposition += cameraSpeed/2.f *  glm::vec3(0.0f, 0.0f,  -1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        tposition += cameraSpeed/2.f * glm::vec3(0.0f, 0.0f,  1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        tposition += cameraSpeed/2.f * glm::vec3(-1.0f, 0.0f,  0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        tposition += cameraSpeed/2.f * glm::vec3(1.0f, 0.0f,  0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
        tposition += cameraSpeed/2.f * glm::vec3(0.0f, -1.0f,  0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        tposition += cameraSpeed * glm::vec3(0.0f, 1.0f,  0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        angle = fmod((angle+15.0f),360.0f);
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
};

/*
// http://www.songho.ca/opengl/gl_transform.html
glm::vec2 getScreenCoord(const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model, const glm::vec3 &point){
    glm::vec4 aux = glm::vec4(point.x, point.y, point.z, 1.0);
    glm::vec3 out;
    glm::vec2 viewport;
    glm::vec2 depth;

    glGetFloatv(GL_MAX_VIEWPORT_DIMS, &viewport[0]);
    glGetFloatv(GL_DEPTH_RANGE, &depth[0]);

    aux = projection * view * model * aux;
    out = glm::vec3(aux.x/aux.w, aux.y/aux.w, aux.z/aux.w);

    out.x = viewport.x/2 * out.x + viewport.x/2 * out.x; 
    out.y = viewport.y/2 * out.y + viewport.y/2 * out.y;
    out.z = (depth.y-depth.x)/2 * out.z + (depth.x+depth.y)/2;

    return glm::vec2(out.x,out.y);
}*/