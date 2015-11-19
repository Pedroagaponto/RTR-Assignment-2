/*
 * Simple 3D sine wave animation example.
 * $Id: sinewave3D-glm.cpp,v 1.1 2014/08/25 02:08:39 gl Exp gl $
 */

#include "core.h"
#include "io.h"

#include <stdbool.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1024, 1024);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
