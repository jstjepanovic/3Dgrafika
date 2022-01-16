#pragma once

#define GLFW_DLL
#define GLEW32_DLL

#include <iostream>
#include "GL/glew.h"
#include <glm/glm.hpp>
#include "ShaderDZ5.h"
#include <fstream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void InitCircleRendering(int in_vertexCountPerCicle);

void RenderCircle(glm::vec2 position, float scale);
