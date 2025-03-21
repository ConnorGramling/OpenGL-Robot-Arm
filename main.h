/*
 * main.h
 *
 *  Created on: November 22, 2024
 *      Author: Connor Gramlinnng
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "initShader.h"
#include "myLib.h"
#include "perspFunc.h"
#include "init.h"
#include "lighting.h"
#include "sun.h"

extern int num_vertices;
extern vec4 *positions;
extern vec4 *colors;

#define VIEW_START 2

// Number of cylinders (or cylinders if needed)
#define NUM_CYLINDERS 11

// Individual CTM for each cylinders
extern mat4 ctms[NUM_CYLINDERS];

void updateFrustum();
 

extern GLuint buffer;
extern GLuint light_position_location;
/*
extern GLuint flashlight_position_location;

extern GLuint use_texture_location;*/
extern GLuint ambient_on_location, diffuse_on_location, specular_on_location;

extern int num_vertices;
extern vec4 *positions;
extern vec4 *normals;
extern vec4 *colors;

extern vec4 light_position;

extern float sun_x;
extern float sun_y;
extern float sun_z;
extern float rotation_angle; // Current rotation angle in degrees

extern vec4 *ambient_colors;
extern vec4 *diffuse_colors;
extern vec4 *specular_colors;

extern vec4 eye, at, up; 



#endif /* MAIN_H_ */