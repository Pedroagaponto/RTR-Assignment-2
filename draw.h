#ifndef DRAW_H
#define DRAW_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

void drawAxes(float length);
void drawVector(glm::vec3 & o, glm::vec3 & v, float s, bool normalize, glm::vec3 & c);
void drawSineWave(int tess);

#endif
