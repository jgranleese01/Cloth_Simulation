#ifdef _WIN32
#include <windows.h> 
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <SOIL.h>

#include <math.h>
#include <vector>
#include <iostream>

#include "Ball.h"
#include "Spring.h"
#include "Colour.h"
#include "Vector3D.h"
#include "Vector4D.h"

//Screen width and height
float screenW = 640;
float screenH = 480;

//Grid size for balls
const int gridSize = 13;

Colour backgroundColor(0.0f, 0.0f, 0.0f, 0.0f);

//2 Ball arrays
int numBalls;
Ball *balls1 = NULL;
Ball *balls2 = NULL;

//Points for the arrays
Ball *currentBalls = NULL;
Ball *nextBalls = NULL;

//Gravity
Vector3D gravity(0.0f, -0.98f, 0.0f);

//Values given to each Spring
float springConstant = 15.0f;
float naturalLength = 1.0f;

//Values given to each Ball
float mass = 0.01f;

//Damping factor. Velocities are multiplied by this
float dampFactor = 0.9f;

//Array of Springs
int numSprings;
Spring *springs = NULL;

//Sphere
float sphereRadius = 4.0f;

bool drawBalls = false;
bool drawSprings = false;
bool drawTriangles = false;
bool drawPatches = true;

int patchTesselation = 5;

//Floor texture
GLuint floorTexture;

//Load and bind the floor texture
void LoadGLTexture()
{
	glGenTextures(1, &floorTexture);

	int width, height;
	unsigned char* image;
	int ch;

	glBindTexture(GL_TEXTURE_2D, floorTexture);
	
	image = SOIL_load_image("floor.tga", &width, &height, &ch, SOIL_LOAD_RGB);
	
	if (!image) {
		std::cout << "Failed to load texture: " << SOIL_last_result << std::endl;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, width, height,
		GL_RGB, GL_UNSIGNED_BYTE, image);
	
	SOIL_free_image_data(image);
}

//
void ResetCloth()
{
	//Re-initalise balls back into the grid
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			balls1[i*gridSize + j].position.Set(float(j) - float(gridSize - 1) / 2,
				8.5f,
				float(i) - float(gridSize - 1) / 2);
			balls1[i*gridSize + j].velocity.LoadZero();
			balls1[i*gridSize + j].mass = mass;
			balls1[i*gridSize + j].fixed = false;
		}
	}

	//Fix the corner balls in place
	balls1[0].fixed = true;
	balls1[gridSize - 1].fixed = true;
	balls1[gridSize*(gridSize - 1)].fixed = true;
	balls1[gridSize*gridSize - 1].fixed = true;

	//Copy balls array into a 2nd array
	for (int i = 0; i<numBalls; ++i)
		balls2[i] = balls1[i];

	//Set pointers
	currentBalls = balls1;
	nextBalls = balls2;

	//Re-initalise springs
	Spring *currentSpring = &springs[0];

	//Springs go from one ball to one next to it but excludes those on right edge
	for (int i = 0; i<gridSize; ++i)
	{
		for (int j = 0; j<gridSize - 1; ++j)
		{
			currentSpring->ball1 = i * gridSize + j;
			currentSpring->ball2 = i * gridSize + j + 1;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength;

			currentSpring++;
		}
	}

	//Springs go from one ball to one below it but excludes those on bottom edge
	for (int i = 0; i < gridSize - 1; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			currentSpring->ball1 = i * gridSize + j;
			currentSpring->ball2 = (i + 1) * gridSize + j;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength;

			currentSpring++;
		}
	}

	//Springs go from one ball to below and right but excludes those on bottom and right edge
	for (int i = 0; i < gridSize - 1; i++)
	{
		for (int j = 0; j < gridSize - 1; j++)
		{
			currentSpring->ball1 = i * gridSize + j;
			currentSpring->ball2 = (i + 1) * gridSize + j + 1;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength * sqrt(2.0f);

			++currentSpring;
		}
	}

	//Springs go from one ball to below and left but excludes those on bottom and right edge
	for (int i = 0; i < gridSize - 1; i++)
	{
		for (int j = 1; j < gridSize; j++)
		{
			currentSpring->ball1 = i * gridSize + j;
			currentSpring->ball2 = (i + 1) * gridSize + j - 1;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength * sqrt(2.0f);

			++currentSpring;
		}
	}

	//Springs go from one ball to the next but excludes those on or next to the right hand edge
	for (int i = 0; i<gridSize; ++i)
	{
		for (int j = 0; j<gridSize - 2; ++j)
		{
			currentSpring->ball1 = i*gridSize + j;
			currentSpring->ball2 = i*gridSize + j + 2;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength * 2;

			++currentSpring;
		}
	}

	//Springs go from one ball to the next but excludes those on or next to the right hand edge
	for (int i = 0; i<gridSize - 2; ++i)
	{
		for (int j = 0; j<gridSize; ++j)
		{
			currentSpring->ball1 = i*gridSize + j;
			currentSpring->ball2 = (i + 2)*gridSize + j;

			currentSpring->springConstant = springConstant;
			currentSpring->naturalLength = naturalLength * 2;

			++currentSpring;
		}
	}
}