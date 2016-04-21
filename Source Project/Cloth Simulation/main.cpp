#include "main.h"

void init(GLvoid)
{
	//Set viewport
	glViewport(0, 0, screenW, screenH);

	//Projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, screenW / screenH, 1.0f, 100.0f);

	//Model-View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	glColor4f(1.0f, 1.0f, +1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//Depth states
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	//Lighting
	glLightfv(GL_LIGHT1, GL_POSITION, Vector4D(1.0f, 1.0f, 1.0f, 0.0f));
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white * 0.2f);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glEnable(GL_LIGHT1);

	//Use 2-sided lighting
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);

	//Number of balls
	numBalls = gridSize * gridSize;

	//Number of springs, right + down
	numSprings = (gridSize - 1) * gridSize * 2;

	//Number of springs, down/right + down/left
	numSprings += (gridSize - 1) * (gridSize - 1) * 2;

	//Number of springs, right(but one) + down
	numSprings += (gridSize - 2) * gridSize * 2;

	//Store balls and springs
	balls1 = new Ball[numBalls];
	balls2 = new Ball[numBalls];
	springs = new Spring[numSprings];
	
	//Set cloth to start position
	ResetCloth();

	//Load floor texture
	LoadGLTexture();
}

//Draw on each frame
void display(void)
{
	static double lastTime = timeGetTime();
	double currentTime = timeGetTime();
	double timePassed = currentTime - lastTime;
	lastTime = currentTime;

	float timePassedInSeconds = 0.01f;

	// calculating positions
	for (int i = 0; i<numSprings; ++i)
	{
		float springLength = (currentBalls[springs[i].ball1].position -
			currentBalls[springs[i].ball2].position).GetLength();

		float extension = springLength - springs[i].naturalLength;

		springs[i].tension = springs[i].springConstant * extension / springs[i].naturalLength;
	}

	//Calculate the nextBalls from the currentBalls
	for (int i = 0; i<numBalls; ++i)
	{
		//Transfer properties which do not change
		nextBalls[i].fixed = currentBalls[i].fixed;
		nextBalls[i].mass = currentBalls[i].mass;

		//If the ball is fixed, transfer the position and zero the velocity, otherwise calculate
		//the new position and velocity
		if (currentBalls[i].fixed)
		{
			nextBalls[i].position = currentBalls[i].position;
			nextBalls[i].velocity.LoadZero();
		}
		else
		{
			//Calculate the force on this ball
			Vector3D force = gravity;

			//Loop through springs
			for (int j = 0; j < numSprings; ++j)
			{
				//if ball1, add the tension to the force
				if (springs[j].ball1 == i)
				{
					Vector3D tensionDirection = currentBalls[springs[j].ball2].position -
						currentBalls[i].position;
					tensionDirection.Normalize();

					force += springs[j].tension*tensionDirection;
				}

				//If ball2, add the tension to the force
				if (springs[j].ball2 == i)
				{
					Vector3D tensionDirection = currentBalls[springs[j].ball1].position -
						currentBalls[i].position;
					tensionDirection.Normalize();

					force += springs[j].tension*tensionDirection;
				}
			}

			//Calculate the acceleration
			Vector3D acceleration = force / currentBalls[i].mass;

			//Update velocity
			nextBalls[i].velocity = currentBalls[i].velocity + acceleration *
				(float)timePassedInSeconds;

			//Damp the velocity
			nextBalls[i].velocity *= dampFactor;

			//Calculate new position
			nextBalls[i].position = currentBalls[i].position + (nextBalls[i].velocity + 
				currentBalls[i].velocity) * (float)timePassedInSeconds / 2;

			//Check against sphere (at origin)
			if (nextBalls[i].position.GetSquaredLength()<sphereRadius * 1.08f * sphereRadius * 1.08f)
				nextBalls[i].position = nextBalls[i].position.GetNormalized() * sphereRadius*1.08f;

			//Check against floor
			if (nextBalls[i].position.y<-8.5f)
				nextBalls[i].position.y = -8.5f;
		}
	}

	//Swap the currentBalls and newBalls pointers
	Ball *temp = currentBalls;
	currentBalls = nextBalls;
	nextBalls = currentBalls;

	//Zero the normals on each ball
	for (int i = 0; i<numBalls; ++i)
		currentBalls[i].normal.LoadZero();

	//Calculate the normals on the current balls
	for (int i = 0; i<gridSize - 1; ++i)
	{
		for (int j = 0; j<gridSize - 1; ++j)
		{
			Vector3D & p0 = currentBalls[i*gridSize + j].position;
			Vector3D & p1 = currentBalls[i*gridSize + j + 1].position;
			Vector3D & p2 = currentBalls[(i + 1)*gridSize + j].position;
			Vector3D & p3 = currentBalls[(i + 1)*gridSize + j + 1].position;

			Vector3D & n0 = currentBalls[i*gridSize + j].normal;
			Vector3D & n1 = currentBalls[i*gridSize + j + 1].normal;
			Vector3D & n2 = currentBalls[(i + 1)*gridSize + j].normal;
			Vector3D & n3 = currentBalls[(i + 1)*gridSize + j + 1].normal;

			//Calculate the normals for the 2 triangles and add on
			Vector3D normal = (p1 - p0).CrossProduct(p2 - p0);

			n0 += normal;
			n1 += normal;
			n2 += normal;

			normal = (p1 - p2).CrossProduct(p3 - p2);

			n1 += normal;
			n2 += normal;
			n3 += normal;
		}
	}

	//Normalize normals
	for (int i = 0; i<numBalls; ++i)
		currentBalls[i].normal.Normalize();

	//Clear window for drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();										
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glTranslatef(0.0f, 0.0f, -28.0f);

	//Draw sphere
	static GLUquadricObj * sphere = gluNewQuadric();
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Colour(0.2f, 0.8f, 0.8f, 0.0f));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Colour(0.2f, 0.8f, 0.8f, 0.0f));
	glMaterialfv(GL_FRONT, GL_SPECULAR, Colour(1.0f, 1.0f, 1.0f, 1.0f));
	glMaterialf(GL_FRONT, GL_SHININESS, 32.0f);
	glEnable(GL_CULL_FACE);

	gluSphere(sphere, sphereRadius, 48, 24);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);

	//Draw floor
	glColor4fv(white);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-25.0f, -8.6f, 10.0f);
		glTexCoord2f(2.0f, 0.0f);
		glVertex3f(25.0f, -8.6f, 10.0f);
		glTexCoord2f(0.0f, 2.0f);
		glVertex3f(-25.0f, -8.6f, -45.0f);
		glTexCoord2f(2.0f, 2.0f);
		glVertex3f(25.0f, -8.6f, -45.0f);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Draw cloth using triangles
	if (drawTriangles)
	{
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Colour(0.8f, 0.0f, 1.0f));	//set material
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Colour(0.8f, 0.0f, 1.0f));
		glMaterialfv(GL_BACK, GL_AMBIENT, Colour(1.0f, 1.0f, 0.0f));
		glMaterialfv(GL_BACK, GL_DIFFUSE, Colour(1.0f, 1.0f, 0.0f));
		glBegin(GL_TRIANGLES);
		{
			for (int i = 0; i<gridSize - 1; ++i)
			{
				for (int j = 0; j<gridSize - 1; ++j)
				{
					glNormal3fv(currentBalls[i*gridSize + j].normal);
					glVertex3fv(currentBalls[i*gridSize + j].position);
					glNormal3fv(currentBalls[i*gridSize + j + 1].normal);
					glVertex3fv(currentBalls[i*gridSize + j + 1].position);
					glNormal3fv(currentBalls[(i + 1)*gridSize + j].normal);
					glVertex3fv(currentBalls[(i + 1)*gridSize + j].position);

					glNormal3fv(currentBalls[(i + 1)*gridSize + j].normal);
					glVertex3fv(currentBalls[(i + 1)*gridSize + j].position);
					glNormal3fv(currentBalls[i*gridSize + j + 1].normal);
					glVertex3fv(currentBalls[i*gridSize + j + 1].position);
					glNormal3fv(currentBalls[(i + 1)*gridSize + j + 1].normal);
					glVertex3fv(currentBalls[(i + 1)*gridSize + j + 1].position);
				}
			}
		}
		glEnd();
		glDisable(GL_LIGHTING);
	}



	//Draw cloth using OpenGL evaluators
	if (drawPatches)
	{
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT, Colour(0.8f, 0.0f, 1.0f));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Colour(0.8f, 0.0f, 1.0f));
		glMaterialfv(GL_BACK, GL_AMBIENT, Colour(1.0f, 1.0f, 0.0f));
		glMaterialfv(GL_BACK, GL_DIFFUSE, Colour(1.0f, 1.0f, 0.0f));

		glFrontFace(GL_CW);

		glEnable(GL_MAP2_VERTEX_3);
		glEnable(GL_MAP2_NORMAL);

		for (int i = 0; i<gridSize - 1; i += 3)
		{
			for (int j = 0; j<gridSize - 1; j += 3)
			{
				glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, sizeof(Ball) / sizeof(float), 4,
					0.0f, 1.0f, gridSize*sizeof(Ball) / sizeof(float), 4,
					currentBalls[i*gridSize + j].position);
				glMap2f(GL_MAP2_NORMAL, 0.0f, 1.0f, sizeof(Ball) / sizeof(float), 4,
					0.0f, 1.0f, gridSize*sizeof(Ball) / sizeof(float), 4,
					currentBalls[i*gridSize + j].normal);
				glMapGrid2f(patchTesselation, 0.0f, 1.0f, patchTesselation, 0.0f, 1.0f);
				glEvalMesh2(GL_FILL, 0, patchTesselation, 0, patchTesselation);
			}
		}

		glDisable(GL_LIGHTING);
		glFrontFace(GL_CCW);

		glDisable(GL_MAP2_VERTEX_3);
		glDisable(GL_MAP2_NORMAL);
	}

	//Draw balls
	if (drawBalls)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		{
			for (int i = 0; i < numBalls; ++i)
				glVertex3fv(currentBalls[i].position);
		}
		glEnd();
	}

	//Draw springs
	if (drawSprings)
	{
		glColor3f(0.0f, 0.0f, 1.0f);

		//bring the lines closer to prevent z-fighting with triangles
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.5f);

		glBegin(GL_LINES);
		{
			for (int i = 0; i<numSprings; ++i)
			{
				//Check the spring has been initialised and the ball numbers are in bounds
				if (springs[i].ball1 != -1 && springs[i].ball2 != -1 &&
					springs[i].ball1<numBalls && springs[i].ball2 < numBalls)
				{
					glVertex3fv(currentBalls[springs[i].ball1].position);
					glVertex3fv(currentBalls[springs[i].ball2].position);
				}
			}
		}
		glEnd();

		glPopMatrix();
	}
	
	glEnd();
	glDisable(GL_LIGHTING);

	glutSwapBuffers();
	glutPostRedisplay();
}

//Takes in keyboard strokes
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//Controls to release each corner of the cloth or to reset it
void arrow_keys(int a_keys, int x, int y)
{
	switch (a_keys) 
	{
	case GLUT_KEY_UP:
		//Releases back-left corner
		currentBalls[0].fixed = false;
		break;
	case GLUT_KEY_DOWN:
		//Releases back-right corner
		currentBalls[gridSize - 1].fixed = false;
		break;
	case GLUT_KEY_LEFT:
		//Releases front-left corner
		currentBalls[gridSize*(gridSize - 1)].fixed = false;
		break;
	case GLUT_KEY_RIGHT:
		//Releases front-right corner
		currentBalls[gridSize*gridSize - 1].fixed = false;
		break;
	case GLUT_KEY_INSERT:
		ResetCloth();
		break;
	case GLUT_KEY_PAGE_UP:
		drawPatches = !drawPatches;
		drawTriangles = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		drawTriangles = !drawTriangles;
		drawPatches = false;
		break;
	case GLUT_KEY_HOME:
		drawSprings = !drawSprings;
		drawBalls = !drawBalls;
		break;
	case GLUT_KEY_END:
		//Release all corners at once
		currentBalls[0].fixed = false;
		currentBalls[gridSize - 1].fixed = false;
		currentBalls[gridSize*(gridSize - 1)].fixed = false;
		currentBalls[gridSize*gridSize - 1].fixed = false;
		break;
	default:
		break;
	}
}

//Main method
int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(screenW, screenH);

	glutCreateWindow("Cloth Simulation");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);

	glutMainLoop();
}