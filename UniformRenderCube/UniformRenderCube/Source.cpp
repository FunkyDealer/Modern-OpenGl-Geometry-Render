#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "LoadShaders.h"

using namespace std;
using namespace glm;

int screenWidth = 800;
int screenHeight = 600;
char title[20] = "Exercicio Cubo";

void init();
void display();
void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods);
void modsInput(GLFWwindow* window, unsigned int codepoint, int mods);
void setFullScreen(GLFWwindow* window);
void setWindowedScreen(GLFWwindow* window);
void scrollCallback(GLFWwindow * window, double xoffset, double yoffset);
GLfloat* LoadCube();
GLfloat* LoadColors();

//VAOS && VBOs
#define NumVAOs 1
#define NumBuffers 2 // Vertices, Cores
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
//const GLuint NumVertices = 3;
GLuint NumVertices = 6 * 6;

GLuint ShaderProgram;

glm::mat4 LocalWorld, View, Projection;

GLfloat zoom = 10.0f;
GLfloat ANGLE = 0.0f; //ANGLE OF MODEL
float rotateSpeed = 0.01f; //Speed at which it rotates
const vec3 DEFAULT_DIR = vec3(1.0f, 1.0f, 0.0f);

const vec3 UP = vec3(0.0f, 1.0f, 0.0f);
const vec3 DOWN = vec3(0.0f, -1.0f, 0.0f);
const vec3 RIGHT = vec3(1.0f, 0.0f, 0.0f);
const vec3 LEFT = vec3(-1.0f, 1.0f, 0.0f);
const vec3 BACKWARD = vec3(0.0f, 0.0f, -1.0f);
const vec3 FORWARD = vec3(0.0f, 0.0f, 1.0f);

const mat4 IDENTITY = mat4(1.0f);

float nearPlane = 0.1f;
float farPlane = 100.f;

float aspectRatio = float(screenWidth) / float(screenHeight);

int main() {
	GLFWwindow *window;

	if (!glfwInit()) return -1;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor(); //gets monitor
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor); //Gets Video Settings

	cout << "w: " << videoMode->width << "h: " << videoMode->height << "@ " << videoMode->refreshRate << "fps" << endl;

	window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr); //Creates window with glfw


	if (!window) { //If window is null
		glfwTerminate(); //Terminate glfw //~bug preventing measure
		return -1;
	}

	glfwMakeContextCurrent(window); //make the context the window

	glewInit();
	glfwSwapInterval(1); //sets v-Sync

	init();

	//Callback Functions
	glfwSetKeyCallback(window, InputManager); //Input with keys
	glfwSetScrollCallback(window, scrollCallback); //Input with mousewheel
	glfwSetCharModsCallback(window, modsInput); //Input with Mods

	mat4 projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);
	mat4 LocalWorld = mat4(1.0f); //Model World //identity matrix

	while (!glfwWindowShouldClose(window)) { //Indica pedido de fecho glfwSetWindowShouldClose(window, 1); //Pede para fechar


		display();

		glfwSwapBuffers(window); //Buffers
		glfwPollEvents(); //Events
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void init() {
	//--------------------------- Create arrays in RAM ---------------------------

	//GLfloat vertices[NumVertices][3] = {
	//	{ -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 0.0f,  1.0f, 0.0f } // Triangulo coordenadas vertices
	//};
	//

	//GLfloat colors[NumVertices][3] = {
	//	{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } // Triangulo Cores Vertices
	//};

	   
	GLfloat *vertices = LoadCube(); //Load Cube Vertices
	GLfloat *colors = LoadColors(); //Load Colors Vertices

	cout << "Ended Model Creation" << endl;

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);

	View = glm::lookAt(
		vec3(0.0f, 0.0f, zoom),	// eye (posicao da camara). Cmaara na posicao (x=0, y=0, z=5). Nota que movo o mundo em sentido contrario.
		BACKWARD,	// center (para onde esta a "olhar")
		UP		// up
	);

	LocalWorld = IDENTITY;
	mat4 ModelViewProjection = Projection * View * LocalWorld;

	// --------------------------- VAOs - Vertex Array Objects ---------------------------
	glGenVertexArrays(NumVAOs, VAOs); //Generate VAO
	glBindVertexArray(VAOs[0]); //Bind VAO "0"

	// --------------------------- VBOs - Vertex Buffer Objects ---------------------------
	glGenBuffers(NumBuffers, Buffers); //Generate NumBufffer names for VBOs

	for (int i = 0; i < NumBuffers; i++) { //For each Name of VBO

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]); //Bind VBO to buffer GL_ARRAY_BUFFER
		if (i == 0) {
			//glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices) /*3 * 3 * sizeof(float)*/, vertices, 0); //Initialize the VBO that's active
			glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(float), vertices, 0); //Initialize the VBO that's active
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Initialize VBO 
		}
		else {
			//glBufferStorage(GL_ARRAY_BUFFER, sizeof(colors) /*3 * 3 * sizeof(float)*/, colors, 0); //Initialize the VBO that's active 	
			glBufferStorage(GL_ARRAY_BUFFER, NumVertices * 3 * sizeof(float), colors, 0); //Initialize the VBO that's active 	
			//glBufferData(GL_ARRAY_BUFFER, sizeof(cores), vertices, GL_STATIC_DRAW);
		}
	}
	
	//---------------------- Shaders ---------------------------
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	ShaderProgram = LoadShaders(shaders);
	if (!ShaderProgram) exit(EXIT_FAILURE);
	glUseProgram(ShaderProgram);

	// --------------------------- Connect Atributtes to Shaders ---------------------------

	// Get the Location of Attribute vPosition in Shader Program
	//GLint Coors_ID = glGetAttribLocation(shaderProgram, "vPosition"); // for versions older than 4.3
	GLint Coords_ID = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vPosition"); // for versions new or equal to 4.3
	// Get the Location of Attribute vColors in Shader Program
	//GLint Colors_ID = glGetAttribLocation(ShaderProgram, "vColors"); // for versions older than 4.3
	GLint Colors_ID = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vColors"); // for versions new or equal to 4.3

	// put a valor in uniform MVP
	GLint mvp_ID = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(ShaderProgram, mvp_ID, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	//glBindVertexArray(VAOs[0]); // it's not necessary to bind the VAO, as it is already active in opengl context.

	// Activate the VBO buffer[0]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	// connects the attribute 'vPosition' from shaders to the active VBO and VAO
	glVertexAttribPointer(Coords_ID, 3 /*3 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);

	// Activate the VBO buffer[1]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	//  connects the attribute 'vPosition' from shaders to the active VBO and VAO
	glVertexAttribPointer(Colors_ID, 3 /*3 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);


	glEnableVertexAttribArray(Coords_ID); //Activate the Coordenate Attribute for the active VAO

	glEnableVertexAttribArray(Colors_ID); //Activate the Color Attribute for the Active VAO


	glViewport(0, 0, screenWidth, screenHeight); //Define viewport Window



}

void display() {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //Black Color

	glClearBufferfv(GL_COLOR, 0, black); //Clears screen all black

	// Update Uniform data

	//View Matrix
	View = lookAt(
		vec3(0.0f, 0.0f, zoom),	// Camera Position in the World
		BACKWARD,	// Direction at which the camera Is Pointing
		UP		// Camera Up Vector
	);

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);
	LocalWorld = rotate(IDENTITY, ANGLE += rotateSpeed, normalize(UP + RIGHT)); //Rotate Model Automatically


	mat4 ModelViewProjection = Projection * View * LocalWorld;

	GLint mvp_ID = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(ShaderProgram, mvp_ID, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// Activates the VAOs
	glBindVertexArray(VAOs[0]);

	//Draws Primitives GL_TRIANGLES using active VAOs
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}



GLfloat* LoadCube() {
	NumVertices = 6 * 6;

	GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,	//Start
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,	//End
	1.0f, 1.0f,-1.0f,	//Start
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,	//End
	-1.0f,-1.0f,-1.0f,	//Start
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	//End
	1.0f,-1.0f, 1.0f,	//Start
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	//End
	-1.0f, 1.0f, 1.0f,	//Start
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,	//End
	1.0f,-1.0f,-1.0f,	//Start
	1.0f, 1.0f, 1.0f,	
	1.0f,-1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	1.0f, 1.0f,-1.0f,	
	-1.0f, 1.0f,-1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f	//End
	};

	GLfloat *vertices = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < NumVertices * 3; i++)
	{
		vertices[i] = g_vertex_buffer_data[i];
	}

	return vertices;
}


GLfloat* LoadColors() {
	GLfloat *colors = (GLfloat *)malloc(NumVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < NumVertices; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//vertices[i][j] = 1.0f, 0.0f, 0.0f
			switch (j) {
			case 0:
				colors[i * 3 + j] = 1.0f;
				break;
			case 1:
				colors[i * 3 + j] = 0.0f;
				break;
			case 2:
				colors[i * 3 + j] = 0.0f;
				break;
			}
		};

	}

	return colors;
}


//-----------------------------------Inputs bellow -----------------------------------------

void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods) { //Keys Input manager
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			cout << "pressed esc" << endl;
			glfwSetWindowShouldClose(window, 1); //Pede para fechar
			break;
		case GLFW_KEY_1:
			setFullScreen(window);
			break;
		case GLFW_KEY_2:
			setWindowedScreen(window);
			break;
		}
	}
}

void modsInput(GLFWwindow* window, unsigned int codepoint, int mods) { //Mods Inputmanager

	if (codepoint == 'a' && mods == GLFW_MOD_SHIFT) {
		cout << "shift + a" << endl;
	}
}

void setFullScreen(GLFWwindow* window) { //Set Full screen
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	int fullWidth = videoMode->width;
	int fullHeight = videoMode->height;

	cout << "width: " << fullWidth << " heigth " << fullHeight << endl;

	aspectRatio = float(fullWidth) / float(fullHeight);

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwSetWindowMonitor(window, monitor, 0, 0, fullWidth, fullHeight, videoMode->refreshRate);

}

void setWindowedScreen(GLFWwindow* window) { //Set Windowed
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	aspectRatio = float(screenWidth) / float(screenHeight);

	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowMonitor(window, NULL, videoMode->width / 3, videoMode->height / 3, screenWidth, screenHeight, videoMode->refreshRate);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	if (yoffset == 1) zoom += fabs(zoom) * 0.1f;
	else if (yoffset == -1) zoom -= fabs(zoom) * 0.1f;
}
