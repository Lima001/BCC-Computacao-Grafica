// Window
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int width, height;

// Frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Bezier curve
glm::vec3 p0(-50.0, 0.0, 0.0);
glm::vec3 p1(-25.0, 0.0, -50.0);
glm::vec3 p2(25.0, 0.0, -50.0);
glm::vec3 p3(50.0, 0.0, 0.0);

// Scene
const float sceneData[] = {
    // Coordinates
    -100.0f, -0.0f, -100.0f,
    100.0f, 0.0f, 0.0f,
    -100.0f, 0.0f, 0.0f,
    100.0f, -0.0f, -100.0f
};

const unsigned int sceneIndices[] = {
    0, 1, 2,
    0, 3, 1
};

float cubeData[] = {
    // Position                 // Color

    // Body
    -0.5f, -0.5f, -0.5f,        1.0f, 0.5f, 0.2f,
    0.5f, -0.5f, -0.5f,         1.0f, 0.5f, 0.2f,
    0.5f, 0.5f, -0.5f,          1.0f, 0.5f, 0.2f,
    -0.5f, 0.5f, -0.5f,         1.0f, 0.5f, 0.2f,
    -0.5f, -0.5f, 0.5f,         1.0f, 0.5f, 0.2f,
    0.5f, -0.5f, 0.5f,          1.0f, 0.5f, 0.2f,
    0.5f, 0.5f, 0.5f,           1.0f, 0.5f, 0.2f,
    -0.5f, 0.5f, 0.5f,          1.0f, 0.5f, 0.2f
};

unsigned int cubeIndices[] = {
    // Body
    0, 1, 2, 2, 3, 0,    // Front
    1, 5, 6, 6, 2, 1,    // Right
    7, 6, 5, 5, 4, 7,    // Back
    4, 0, 3, 3, 7, 4,    // Left
    4, 5, 1, 1, 0, 4,    // Bottom
    3, 2, 6, 6, 7, 3,     // Top
};

float lineData[] = {
    // Position                     // Color

    // Axes
    -100.0f, 0.0f, 0.0f,            1.0f, 0.0f, 0.0f,
    0.0f, -100.0f, 0.0f,            0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -100.0f,            0.0f, 0.0f, 1.0f,
    100.0f, 0.0f, 0.0f,             1.0f, 0.0f, 0.0f,
    0.0f, 100.0f, 0.0f,             0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 100.0f,             0.0f, 0.0f, 1.0f
};

unsigned int lineIndices[] = {
    0, 3,     // X-axis
    1, 4,     // Y-axis
    2, 5      // Z-axis
};