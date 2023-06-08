// Window
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
int width, height;

// Colors
glm::vec4 black(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec4 green(0.0f, 1.0f, 0.0f, 1.0f);

// Frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Scene
const float sceneData[] = {
    // Coordinates
    -1.0f, -1.0f, -0.0f,
    1.0f, 1.0f, -0.0f,
    -1.0f, 1.0f, -0.0f,
    1.0f, -1.0f, -0.0f
};

const unsigned int sceneIndices[] = {
    0, 1, 2,
    0, 3, 1
};