#include "Geometry.h"


GLfloat* LoadCube() {
	int numVertices = 6 * 6;

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

	GLfloat *vertices = (GLfloat *)malloc(numVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < numVertices * 3; i++)
	{
		vertices[i] = g_vertex_buffer_data[i];
	}

	return vertices;
}


GLfloat* LoadColors() {
	int numVertices = 6 * 6;

	GLfloat *colors = (GLfloat *)malloc(numVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < numVertices; i++)
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

GLfloat* LoadTriangle() {
	int numVertices = 3;

	GLfloat g_vertex_buffer_data[] = {
	 -1.0f, -1.0f, 0.0f ,
	 1.0f, -1.0f, 0.0f ,
	 0.0f,  1.0f, 0.0f }; // Triangulo coordenadas vertices

	GLfloat *vertices = (GLfloat *)malloc(numVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < numVertices * 3; i++)
	{
		vertices[i] = g_vertex_buffer_data[i];
	}

	return vertices;
}

GLfloat* LoadTriangleColors() {
	int numVertices = 3;

	GLfloat g_vertex_data_data[] = {
	1.0f, 0.0f, 0.0f ,
	0.0f, 1.0f, 0.0f ,
	0.0f, 0.0f, 1.0f  // Triangulo Cores Vertices
	};

	GLfloat *colors = (GLfloat *)malloc(numVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < numVertices * 3; i++)
	{
		colors[i] = g_vertex_data_data[i];

	}

	return colors;
}