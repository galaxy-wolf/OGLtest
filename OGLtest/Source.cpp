
#include <gl/glew.h>
#include <glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "GLSL_Shader_Loader\shader_glsl.h"

const int windowWidth = 500;
const int windowHeight = 500;

GLSLShaderManager *_shaderManager;

void myDisplay()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 0.001f, 10000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
	glutSolidTeapot(1.0f);

	glutSwapBuffers();
}


GLuint VBO, IBO;

void initLine()
{
	float vertex[8] = {
		-0.9f, 0.9f, 0.5f, 1.0f,
		0.9f, 0.9f, 0.5f, 1.0f,
	};

	int index[2] = { 0, 1 };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		8 * sizeof(float),
		vertex,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		2* sizeof(GLuint),
		index,
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void testLine()
{
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	glUseProgram(_shaderManager->program[0].handle);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0,
		4,
		GL_FLOAT,
		GL_FALSE,
		4 * sizeof(float),
		0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();

}

void InitializeGLEW()
{
	glewInit();

	if (!glewIsSupported("GL_EXT_framebuffer_object"))
	{
		printf("Unable to load the necessary extensions\n");
		printf("This sample requires:\n"
			"OpenGL 4.0\n"
			"GL_EXT_framebuffer_object\n"
			"Exiting...\n");
		exit(-1);
	}

}


void init(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("OGLtest");
	glutDisplayFunc(&testLine);
	glutIdleFunc(&testLine);


	InitializeGLEW();

	initLine();
	_shaderManager = new GLSLShaderManager(".");
	if (!_shaderManager->startup()) {
		fprintf(stderr, "%s %d: failed to create shaderManager", __FILE__, __LINE__);
		exit(-1);
	}
	_shaderManager->loadProgram("line", 0, 0, 0, "line");

}


int main(int argc, char *argv[])
{
	init(argc, argv);
	glutMainLoop();

	return 0;
}