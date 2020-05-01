#pragma once
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <fstream>

#include <iostream>
#include <vector>
#include <string>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

using namespace glm;
using namespace std;


const vec3 DEFAULT_DIR = vec3(1.0f, 1.0f, 0.0f);
const vec3 UP = vec3(0.0f, 1.0f, 0.0f);
const vec3 DOWN = vec3(0.0f, -1.0f, 0.0f);
const vec3 RIGHT = vec3(1.0f, 0.0f, 0.0f);
const vec3 LEFT = vec3(-1.0f, 1.0f, 0.0f);
const vec3 BACKWARD = vec3(0.0f, 0.0f, -1.0f);
const vec3 FORWARD = vec3(0.0f, 0.0f, 1.0f);

const mat4 IDENTITY = mat4(1.0f);

//Geometry.cpp Functions
GLfloat* LoadCube();
GLfloat* LoadColors();
GLfloat* LoadTriangle();
GLfloat* LoadTriangleColors();

//Source.cpp Functions
void init();
void display();
void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods);
void modsInput(GLFWwindow* window, unsigned int codepoint, int mods);
void setFullScreen(GLFWwindow* window);
void setWindowedScreen(GLFWwindow* window);
void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);



