#ifndef CORE_H
#define CORE_H

#include <stdbool.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

typedef enum {
	d_drawSineWave,
	d_mouse,
	d_key,
	d_animation,
	d_lighting,
	d_OSD,
	d_computeLighting,
	d_nflags
} DebugFlags;
extern bool debug[d_nflags];

typedef struct { float r, g, b; } color3f;
typedef enum { line, fill } polygonMode_t;

typedef struct {
	bool animate;
	bool lighting;
	bool fixed;
	bool twoside;
	bool displayOSD;
	bool consolePM;
	bool drawNormals;
	float t, lastT;
	float frameRate;
	float displayStatsInterval;
	int width, height;
	int tess;
	int waveDim;
	int frameCount;
	int lastStatsDisplayT;
	int shininess;
	polygonMode_t polygonMode;
} Global;
extern Global g;

typedef enum { inactive, rotate, pan, zoom } CameraControl;
struct camera_t {
	int lastX, lastY;
	float rotateX, rotateY;
	float scale;
	CameraControl control;
};
extern struct camera_t camera;

extern GLuint shaderProgram;
extern glm::mat4 modelViewMatrix;
extern glm::mat3 normalMatrix;

const float milli = 1000.0;

void init(void);
void reshape(int w, int h);
void idle();
#endif
