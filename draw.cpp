#include "draw.h"
#include "core.h"
#include "calc.h"

#include <stdio.h>

static glm::vec3 colour(1.0, 0.0, 0.0);
static glm::vec3 cyan(1.0, 0.0, 1.0);
static int err;

void drawAxes(float length)
{
	glm::vec4 v;

	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_LINES);

	/* x axis */
	glColor3f(colour.x, colour.y, colour.z);
	v = modelViewMatrix * glm::vec4(-length, 0.0, 0.0, 1.0);
	glVertex3fv(&v[0]);
	v = modelViewMatrix * glm::vec4(length, 0.0, 0.0, 1.0);
	glVertex3fv(&v[0]);

	/* y axis */
	glColor3f(0.0, 1.0, 0.0);
	v = modelViewMatrix * glm::vec4(0.0, -length, 0.0, 1.0);
	glVertex3fv(&v[0]);
	v = modelViewMatrix * glm::vec4(0.0, length, 0.0, 1.0);
	glVertex3fv(&v[0]);

	/* z axis */
	glColor3f(0.0, 0.0, 1.0);
	v = modelViewMatrix * glm::vec4(0.0, 0.0, -length, 1.0);
	glVertex3fv(&v[0]);
	v = modelViewMatrix * glm::vec4(0.0, 0.0, length, 1.0);
	glVertex3fv(&v[0]);

	glEnd();
	glPopAttrib();
}

void drawVector(glm::vec3 & o, glm::vec3 & v, float s, bool normalize, glm::vec3 & c)
{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(&c[0]);
	glBegin(GL_LINES);
	if (normalize)
		v = glm::normalize(v);

	glVertex3fv(&o[0]);
	glm::vec3 e(o + s * v);
	glVertex3fv(&e[0]);
	glEnd();
	glPopAttrib();
}

void drawSineWave(int tess)
{
	const float A1 = 0.25, k1 = 2.0 * M_PI, w1 = 0.25;
	const float A2 = 0.25, k2 = 2.0 * M_PI, w2 = 0.25;
	float stepSize = 2.0 / tess;
	glm::vec3 r, n, rEC, nEC;
	int i, j;
	float t = g.t;

	if (g.lighting && g.fixed) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
		if (g.twoside)
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	} else {
		glDisable(GL_LIGHTING);
	}

	if (g.polygonMode == line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Sine wave
	for (j = 0; j < tess; j++) {
		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= tess; i++) {
			r.x = -1.0 + i * stepSize;
			r.z = -1.0 + j * stepSize;

			if (g.waveDim == 2) {
				r.y = A1 * sinf(k1 * r.x + w1 * t);
				if (g.lighting) {
					n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
					n.y = 1.0;
					n.z = 0.0;
				}
			} else if (g.waveDim == 3) {
				r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
				if (g.lighting) {
					n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
					n.y = 1.0;
					n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
				}
			}

			rEC = glm::vec3(modelViewMatrix * glm::vec4(r, g.shininess));
			if (g.lighting) {
				nEC = normalMatrix * glm::normalize(n);
				if (g.fixed) {
					glNormal3fv(&nEC[0]);
				} else {
					glm::vec3 c = computeLighting(rEC, nEC);
					glColor3fv(&c[0]);
				}
			}
			glVertex3fv(&rEC[0]);

			r.z += stepSize;

			if (g.waveDim == 3) {
				r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
				if (g.lighting) {
					n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
				}
			}

			rEC = glm::vec3(modelViewMatrix * glm::vec4(r, g.shininess));
			if (g.lighting) {
				nEC = normalMatrix * glm::normalize(n);
				if (g.fixed) {
					glNormal3fv(&nEC[0]);
				} else {
					glm::vec3 c = computeLighting(rEC, nEC);
					glColor3fv(&c[0]);
				}
			}
			glVertex3fv(&rEC[0]);
		}

		glEnd();

	}

	if (g.lighting) {
		glDisable(GL_LIGHTING);
	}

	// Normals
	if (g.drawNormals) {
		for (j = 0; j <= tess; j++) {
			for (i = 0; i <= tess; i++) {
				r.x = -1.0 + i * stepSize;
				r.z = -1.0 + j * stepSize;

				n.y = 1.0;
				n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
				if (g.waveDim == 2) {
					r.y = A1 * sinf(k1 * r.x + w1 * t);
					n.z = 0.0;
				} else {
					r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
					n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
				}

				rEC = glm::vec3(modelViewMatrix * glm::vec4(r, 1.0));
				nEC = normalMatrix * glm::normalize(n);
				drawVector(rEC, nEC, 0.05, true, cyan);
			}
		}
	}

	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("%s %d\n", __FILE__, __LINE__);
		printf("displaySineWave: %s\n", gluErrorString(err));
	}
}

