/*
 * init.h
 *
 *  Created on: December 4, 2024
 *      Author: Connor Gramlinnng
 */

#ifndef INIT_H
#define INIT_H

#include "main.h"
#include "myLib.h"
#include "lighting.h"

// Common settings for the cylinders
#define STACKS 20
#define SLICES 20
#define RADIUS 0.1

typedef struct {
  vec4 color;
  vec4 topCenter;
  vec4 bottomCenter;
  float radius;
} CylinderSpec;

extern CylinderSpec cylinders[];

void createCylinders(int stacks, int slices, const CylinderSpec* specs, int numCylinders);

void createCylinder(int stacks, int slices, float radius, vec4 topCenter, vec4 bottomCenter, vec4 color);

void createSphere(int stacks, int slices, float radius, vec4 color, vec4 center);

mat4 computeRotationMatrix(vec4 direction);

mat4 mat4_rotation_axis(float angle, vec4 axis);

vec4 rotateVector(vec4 v, mat4 rotationMatrix);

#endif // INIT_H
