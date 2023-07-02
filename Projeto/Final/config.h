#ifndef CONFIG_H
#define CONFIG_H

#include <glm/glm.hpp>

// Spline
glm::vec4 worldBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
glm::vec4 splineBackgroundColor(0.2f, 0.2f, 0.3f, 1.0);
glm::vec4 splineFillColor(0.7f, 0.7f, 0.7f, 1.0f);
glm::vec4 splinePointColor(1.0f, 0.0f, 0.0f, 1.0f);
float pointWidth = 0.0f;
float lineWidth = 0.7f;

// Screen
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Movement
float wr = 0.02;
float wl = 0.01;
float L = 1.0f;
float v = (wl+wr)/2.0f;
float w = (wl-wr)/L;
float x = 0.0f, z = 0.0f;
float theta = 0.0f;

#endif