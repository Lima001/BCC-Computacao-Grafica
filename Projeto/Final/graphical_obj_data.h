#include <glm/glm.hpp>

#define MAX_BEZIER_CURVES 4
#define N_BEZIER_CURVE 2
#define N_CUBES 5

const float sceneData[] = {
    -500.0f,    -0.0f,      -500.0f,
    +500.0f,    +0.0f,      +500.0f,
    -500.0f,    +0.0f,      +500.0f,
    +500.0f,    -0.0f,      -500.0f
};

const unsigned int sceneIndices[] = {
    0, 1, 2,
    0, 3, 1
};

float lineData[] = {
    // Position                               // Color
    -100.0f,    +0.0f,      +0.0f,            +1.0f, +0.0f, +0.0f,
    +0.0f,      -100.0f,    +0.0f,            +0.0f, +1.0f, +0.0f,
    +0.0f,      +0.0f,      -100.0f,          +0.0f, +0.0f, +1.0f,
    +100.0f,    +0.0f,      +0.0f,            +1.0f, +0.0f, +0.0f,
    +0.0f,      +100.0f,    +0.0f,            +0.0f, +1.0f, +0.0f,
    +0.0f,      +0.0f,      +100.0f,          +0.0f, +0.0f, +1.0f
};

unsigned int lineIndices[] = {
    0, 3,
    1, 4,
    2, 5 
};

float cubeData[] = {
    // Position                           // Color
    -0.5f,      +0.0f,      -0.5f,        +1.0f, +0.5f, +0.2f,
    +0.5f,      +0.0f,      -0.5f,        +1.0f, +0.5f, +0.2f,
    +0.5f,      +1.0f,      -0.5f,        +1.0f, +0.5f, +0.2f,
    -0.5f,      +1.0f,      -0.5f,        +1.0f, +0.5f, +0.2f,
    -0.5f,      +0.0f,      +0.5f,        +1.0f, +0.5f, +0.2f,
    +0.5f,      +0.0f,      +0.5f,        +1.0f, +0.5f, +0.2f,
    +0.5f,      +1.0f,      +0.5f,        +1.0f, +0.5f, +0.2f,
    -0.5f,      +1.0f,      +0.5f,        +1.0f, +0.5f, +0.2f
};

unsigned int cubeIndices[] = {
    // Body
    0, 1, 2, 2, 3, 0, 
    1, 5, 6, 6, 2, 1, 
    7, 6, 5, 5, 4, 7, 
    4, 0, 3, 3, 7, 4, 
    4, 5, 1, 1, 0, 4,
    3, 2, 6, 6, 7, 3,
};

glm::vec3 cubeTransfomation[] = {
    // Translate        // Scale
    glm::vec3(+0.0f, +0.0f, +0.0f),         glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(+0.625f, 0.0f, 0.0f),         glm::vec3(0.25f, 0.5f, 0.75f),
    glm::vec3(-0.625f, 0.0f, 0.0f),         glm::vec3(0.25f, 0.5f, 0.75f),
    glm::vec3(0.5f, 0.5f, 1.2f),           glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(-0.5f, 0.5f, 1.2f),           glm::vec3(0.2f, 0.2f, 0.2f)
};

/*
glm::mat3x4 controlPointSet1(
    -50.0f, 0.0f, 0.0f,
    -25.0f, 0.0f, 50.0f,
    25.0f, 0.0f, 50.0f,
    50.0f, 0.0f, 0.0f
);

glm::mat3x4 controlPointSet2(
    75.0f, 0.0f, 0.0f,
    90.0f, 0.0f, 50.0f,
    135.0f, 0.0f, 50.0f,
    150.0f, 0.0f, 0.0f
);

glm::mat3x4 controlPointSet3(0.0f);
glm::mat3x4 controlPointSet4(0.0f);

glm::mat3x4 controlPointsList[MAX_BEZIER_CURVES] = {
    controlPointSet1, controlPointSet2, controlPointSet3, controlPointSet4
};*/

glm::vec3 p0(0.0f, 0.0f, 0.0f);
glm::vec3 p1(25.0f, 0.0f, 50.0f);
glm::vec3 p2(50.0f, 0.0f, 50.0f);
glm::vec3 p3(75.0f, 0.0f, 0.0f);