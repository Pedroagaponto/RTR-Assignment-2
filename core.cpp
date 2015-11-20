#include "core.h"
#include "shaders.h"

#include <stdio.h>

bool debug[d_nflags] = { false, false, false, false, false, false, false };
Global g = { false, true, true, false, false, false, false, false, 0.0, 0.0, 0.0, 1.0, 0, 0, 8, 2, 0, 0, 20, line };
struct camera_t camera = { 0, 0, 30.0, -30.0, 1.0, inactive };
GLuint shaderProgram;
glm::mat4 modelViewMatrix;
glm::mat3 normalMatrix;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	if (g.twoside)
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	shaderProgram = getShader("shader.vert", "shader.frag");
}

void reshape(int w, int h)
{
	g.width = w;
	g.height = h;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	float t, dt;

	t = glutGet(GLUT_ELAPSED_TIME) / milli;

	// Accumulate time if animation enabled
	if (g.animate) {
		dt = t - g.lastT;
		g.t += dt;
		g.lastT = t;
		if (debug[d_animation])
			printf("idle: animate %f\n", g.t);
	}

	// Update stats, although could make conditional on a flag set interactively
	dt = (t - g.lastStatsDisplayT);
	if (dt > g.displayStatsInterval) {
		g.frameRate = g.frameCount / dt;
		g.lastStatsDisplayT = t;
		g.frameCount = 0;
	}

	glutPostRedisplay();
}

