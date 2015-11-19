#include "io.h"
#include "shaders.h"
#include "core.h"

#include <stdio.h>

static int err;

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			printf("exit\n");
			exit(0);
			break;
		case 'a':
			g.animate = !g.animate;
			if (g.animate) {
				g.lastT = glutGet(GLUT_ELAPSED_TIME) / milli;
			}
			break;
		case 'd':
			//TODO toggle directional, positional lighting
			break;
		case 'f':
			//TODO toggle smooth, flat shading
			break;
		case 'H':
			//TODO increase shininess
			break;
		case 'h':
			//TODO decrease shininess
			break;
		case 'l':
			g.lighting = !g.lighting;
			glutPostRedisplay();
			break;
		case 'm':
			//TODO Blinn-Phong or Phong specular lighting model
			break;
		case 'n':
			g.drawNormals = !g.drawNormals;
			glutPostRedisplay();
			break;
		case 'o':
			if (g.displayOSD) {
				g.displayOSD = false;
				g.consolePM = true;
			} else if (g.consolePM) {
				g.consolePM = false;
			} else {
				g.displayOSD = true;
			}

			glutPostRedisplay();
			break;
		case 'p':
			//TODO per (vertex, pixel) lighting
			break;
		case 's':
			g.fixed = !g.fixed;
			if (g.fixed)
				setShader(0);
			else
				setShader(shaderProgram);
			glutPostRedisplay();
			break;
		case 'T':
			g.tess *= 2;
			glutPostRedisplay();
			break;
		case 't':
			g.tess /= 2;
			if (g.tess < 8)
				g.tess = 8;
			glutPostRedisplay();
			break;
		case 'v':
			g.consolePM = !g.consolePM;
			glutPostRedisplay();
			break;
		case 'w':
			if (g.polygonMode == line)
				g.polygonMode = fill;
			else
				g.polygonMode = line;
			glutPostRedisplay();
			break;
		case 'z':
			g.waveDim++;
			if (g.waveDim > 3)
				g.waveDim = 2;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (debug[d_mouse])
		printf("mouse: %d %d %d\n", button, x, y);

	camera.lastX = x;
	camera.lastY = y;

	if (state == GLUT_DOWN)
		switch(button) {
			case GLUT_LEFT_BUTTON:
				camera.control = rotate;
				break;
			case GLUT_MIDDLE_BUTTON:
				camera.control = pan;
				break;
			case GLUT_RIGHT_BUTTON:
				camera.control = zoom;
				break;
		}
	else if (state == GLUT_UP)
		camera.control = inactive;
}

void motion(int x, int y)
{
	float dx, dy;

	if (debug[d_mouse]) {
		printf("motion: %d %d\n", x, y);
		printf("camera.rotate: %f %f\n", camera.rotateX, camera.rotateY);
		printf("camera.scale:%f\n", camera.scale);
	}

	dx = x - camera.lastX;
	dy = y - camera.lastY;
	camera.lastX = x;
	camera.lastY = y;

	switch (camera.control) {
		case inactive:
			break;
		case rotate:
			camera.rotateX += dy;
			camera.rotateY += dx;
			break;
		case pan:
			break;
		case zoom:
			camera.scale += dy / 100.0;
			break;
	}

	glutPostRedisplay();
}

// Console performance meter
void consolePM()
{
	printf("frame rate (f/s):  %5.0f\n", g.frameRate);
	printf("frame time (ms/f): %5.0f\n", 1.0 / g.frameRate * 1000.0);
	printf("tesselation:       %5d\n\n", g.tess);
}

// On screen display
void displayOSD()
{
	char buffer[30];
	char *bufp;
	int w, h;

	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	/* Set up orthographic coordinate system to match the window,
	   i.e. (0,0)-(w,h) */
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* Frame rate */
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(10, 60);
	snprintf(buffer, sizeof buffer, "frame rate (f/s):  %5.0f", g.frameRate);
	for (bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

	/* Frame time */
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(10, 40);
	snprintf(buffer, sizeof buffer, "frame time (ms/f): %5.0f", 1.0 / g.frameRate * milli);
	for (bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

	/* Tesselation */
	glColor3f(1.0, 1.0, 0.0);
	glRasterPos2i(10, 20);
	snprintf(buffer, sizeof buffer, "tesselation:       %5d", g.tess);
	for (bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

	glPopMatrix();  /* Pop modelview */
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();  /* Pop projection */
	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glViewport(0, 0, g.width, g.height);

	modelViewMatrix = glm::mat4(1.0);
	normalMatrix = glm::mat3(1.0);

	modelViewMatrix = glm::rotate(modelViewMatrix,
			camera.rotateX * glm::pi<float>() / 180.0f, glm::vec3(1.0, 0.0, 0.0));
	modelViewMatrix = glm::rotate(modelViewMatrix,
			camera.rotateY * glm::pi<float>() / 180.0f, glm::vec3(0.0, 1.0, 0.0));
	modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(camera.scale));

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

	drawAxes(5.0);
	drawSineWave(g.tess);

	if (g.displayOSD)
		displayOSD();

	if (g.consolePM)
		consolePM();

	glutSwapBuffers();

	g.frameCount++;

	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("%s %d\n", __FILE__, __LINE__);
		printf("display: %s\n", gluErrorString(err));
	}
}

void displayMultiView()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 modelViewMatrixSave(modelViewMatrix);
	glm::mat3 normalMatrixSave(normalMatrix);

	// Front view
	modelViewMatrix = glm::mat4(1.0);
	glViewport(g.width / 16.0, g.height * 9.0 / 16.0, g.width * 6.0 / 16.0, g.height * 6.0 / 16.0);
	drawAxes(5.0);
	drawSineWave(g.tess);

	// Top view
	modelViewMatrix = glm::mat4(1.0);
	modelViewMatrix = glm::rotate(modelViewMatrix, glm::pi<float>() / 2.0f, glm::vec3(1.0, 0.0, 0.0));
	glViewport(g.width / 16.0, g.height / 16.0, g.width * 6.0 / 16.0, g.height * 6.0 / 16);
	drawAxes(5.0);
	drawSineWave(g.tess);

	// Left view
	modelViewMatrix = glm::mat4(1.0);
	modelViewMatrix = glm::rotate(modelViewMatrix, glm::pi<float>() / 2.0f, glm::vec3(0.0, 1.0, 0.0));
	glViewport(g.width * 9.0 / 16.0, g.height * 9.0 / 16.0, g.width * 6.0 / 16.0, g.height * 6.0 / 16.0);
	drawAxes(5.0);
	drawSineWave(g.tess);

	// General view
	modelViewMatrix = glm::rotate(modelViewMatrix,
			camera.rotateX * glm::pi<float>() / 180.0f, glm::vec3(1.0, 0.0, 0.0));
	modelViewMatrix = glm::rotate(modelViewMatrix,
			camera.rotateY * glm::pi<float>() / 180.0f, glm::vec3(0.0, 1.0, 0.0));
	modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(camera.scale));
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	glViewport(g.width * 9.0 / 16.0, g.width / 16.0, g.width * 6.0 / 16.0, g.height * 6.0 / 16.0);
	drawAxes(5.0);
	drawSineWave(g.tess);


	glutSwapBuffers();
}

void printVec(float *v, int n)
{
	int i;

	for (i = 0; i < n; i++)
		printf("%4.2f ", v[i]);
	printf("\n");
}

void printMatrixLinear(float *m, int n)
{
	int i;

	for (i = 0; i < n; i++)
		printf("%4.2f ", m[i]);
	printf("\n");
}

void printMatrixColumnMajor(float *m, int n)
{
	int i, j;

	for (j = 0; j < n; j++) {
		for (i = 0; i < n; i++) {
			printf("%5.2f ", m[i*4+j]);
		}
		printf("\n");
	}
	printf("\n");
}

