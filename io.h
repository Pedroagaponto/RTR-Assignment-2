#ifndef IO_H
#define IO_H

#include "draw.h"

void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void consolePM();
void displayOSD();
void display();
void displayMultiView();
void printVec(float *v, int n);
void printMatrixLinear(float *m, int n);
void printMatrixColumnMajor(float *m, int n);

#endif
