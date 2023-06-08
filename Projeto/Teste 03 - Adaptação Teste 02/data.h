// Window
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
int width, height;

// Frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Bezier curve
const float P_WIDTH = 5.0;
glm::vec2 p0(10.0, 10.0);
glm::vec2 p1(200.0, 300.0);
glm::vec2 p2(400.0, 300.0);
glm::vec2 p3(590.0, 10.0);

// Point to be moved (by keybord, for now)
glm::vec2* pm = &p0;

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