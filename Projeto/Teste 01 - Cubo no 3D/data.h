float cubeData[] = {
    // Position                 // Color
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