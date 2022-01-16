#pragma once

#define GLFW_DLL
#define GLEW32_DLL

#include <vector>
#include <iostream>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderDZ5.h"
#include <fstream>

void InitLineRendering();

void RenderLine(std::vector<glm::vec2>& points);