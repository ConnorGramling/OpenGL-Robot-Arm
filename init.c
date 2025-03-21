/*
 * init.c
 *
 *  Created on: December 4, 2024
 *      Author: Connor Gramling
 */

#include "main.h"
#include "myLib.h"
#include "init.h"

CylinderSpec cylinders[] = {
  {{0.5, 0.5, 0.5, 1.0}, {0.0, 0.0, 0.0, 1.0}, {0.0, -0.03, 0.0, 1.0}, RADIUS * 5}, // Floor
  {{1.0, 0.0, 0.0, 1.0}, {0.0, 0.2, 0.0, 1.0}, {0.0, 0.0, 0.0, 1.0}, RADIUS * 1.2}, // Base
  {{0.0, 1.0, 0.0, 1.0}, {-0.2, 0.2, 0.0, 1.0}, {0.2, 0.2, 0.0, 1.0}, RADIUS * 1.2}, // Lower Joint
  {{0.0, 0.0, 1.0, 1.0}, {0.0, 0.6, 0.0, 1.0}, {0.0, 0.2, 0.0, 1.0}, RADIUS * 0.8}, // Lower Arm
  {{1.0, 1.0, 0.0, 1.0}, {-0.15, 0.6, 0.0, 1.0}, {0.15, 0.6, 0.0, 1.0}, RADIUS * 1.0}, // Middle Joint
  {{0.0, 1.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.6, 0.0, 1.0}, RADIUS * 0.6}, // Middle Arm
  {{1.0, 0.0, 1.0, 1.0}, {-0.1, 1.0, 0.0, 1.0}, {0.1, 1.0, 0.0, 1.0}, RADIUS * 1.0}, // Upper Joint
  {{1.0, 0.5, 0.0, 1.0}, {0.0, 1.4, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, RADIUS * 0.4}, // Upper Arm
  {{0.5, 0.5, 1.0, 1.0}, {-0.2, 1.4, 0.0, 1.0}, {0.2, 1.4, 0.0, 1.0}, RADIUS * 0.5}, // Wrist
  {{0.8, 0.4, 0.4, 1.0}, {-0.1, 1.6, 0.0, 1.0}, {-0.1, 1.4, 0.0, 1.0}, RADIUS * 0.2}, // Left Finger
  {{0.8, 0.4, 0.4, 1.0}, {0.1, 1.6, 0.0, 1.0}, {0.1, 1.4, 0.0, 1.0}, RADIUS * 0.2} // Right Finger
};

int idx = 0;

void createCylinder(int stacks, int slices, float radius, vec4 topCenter, vec4 bottomCenter, vec4 color)
{
  // Calculate the height and direction of the cylinder
  vec4 direction = {
      topCenter.x - bottomCenter.x,
      topCenter.y - bottomCenter.y,
      topCenter.z - bottomCenter.z,
      0.0};

  float height = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
  float stackHeight = height / stacks;

  // Normalize the direction vector
  direction.x /= height;
  direction.y /= height;
  direction.z /= height;

  // Create a rotation matrix to align the cylinder's axis with the direction vector
  mat4 rotationMatrix = computeRotationMatrix(direction);

  // Find two orthonormal vectors perpendicular to the direction vector
  vec4 axis1, axis2;
  if (fabs(direction.x) > fabs(direction.y))
  {
    axis1 = norm_v(cross_prod(direction, (vec4){0, 1, 0, 0}));
    axis2 = cross_prod(direction, axis1);
  }
  else
  {
    axis1 = norm_v(cross_prod(direction, (vec4){1, 0, 0, 0}));
    axis2 = cross_prod(direction, axis1);
  }

  // Generate vertices for the cylinder (sides and caps together)
  for (int i = 0; i <= stacks; ++i)
  {
    float t = (float)i / stacks;
    vec4 stackCenter = add_vv(bottomCenter, scal_v_mult(t * height, direction));

    for (int j = 0; j < slices; ++j)
    {
      float theta = 2.0f * M_PI * (float)j / slices;
      float thetaNext = 2.0f * M_PI * (float)(j + 1) / slices;

      // Compute vertices using axis1 and axis2
      vec4 v1 = add_vv(stackCenter, add_vv(scal_v_mult(radius * cos(theta), axis1), scal_v_mult(radius * sin(theta), axis2)));
      vec4 v2 = add_vv(stackCenter, add_vv(scal_v_mult(radius * cos(thetaNext), axis1), scal_v_mult(radius * sin(thetaNext), axis2)));

      // Compute normals
      vec4 normal1 = norm_v(add_vv(scal_v_mult(cos(theta), axis1), scal_v_mult(sin(theta), axis2)));
      vec4 normal2 = norm_v(add_vv(scal_v_mult(cos(thetaNext), axis1), scal_v_mult(sin(thetaNext), axis2)));

      if (i <= 0)
      {
        // Bottom cap
        vec4 bottomOuter1 = add_vv(bottomCenter, add_vv(scal_v_mult(radius * cos(theta), axis1), scal_v_mult(radius * sin(theta), axis2)));
        vec4 bottomOuter2 = add_vv(bottomCenter, add_vv(scal_v_mult(radius * cos(thetaNext), axis1), scal_v_mult(radius * sin(thetaNext), axis2)));

        positions[idx] = bottomCenter;
        normals[idx] = (vec4){-direction.x, -direction.y, -direction.z, 0.0};
        colors[idx++] = color;

        positions[idx] = bottomOuter1;
        normals[idx] = (vec4){-direction.x, -direction.y, -direction.z, 0.0};
        colors[idx++] = color;

        positions[idx] = bottomOuter2;
        normals[idx] = (vec4){-direction.x, -direction.y, -direction.z, 0.0};
        colors[idx++] = color;
      }
      else if (i == stacks)
      {
        // Top cap
        positions[idx] = topCenter;
        normals[idx] = direction;
        colors[idx++] = color;

        positions[idx] = v1;
        normals[idx] = direction;
        colors[idx++] = color;

        positions[idx] = v2;
        normals[idx] = direction;
        colors[idx++] = color;
      }
      if (i < stacks)
      {
        vec4 nextStackCenter = add_vv(bottomCenter, scal_v_mult((t + 1.0f / stacks) * height, direction));
        vec4 v3 = add_vv(nextStackCenter, add_vv(scal_v_mult(radius * cos(theta), axis1), scal_v_mult(radius * sin(theta), axis2)));
        vec4 v4 = add_vv(nextStackCenter, add_vv(scal_v_mult(radius * cos(thetaNext), axis1), scal_v_mult(radius * sin(thetaNext), axis2)));

        positions[idx] = v1;
        normals[idx] = normal1;
        colors[idx++] = color;

        positions[idx] = v3;
        normals[idx] = normal1;
        colors[idx++] = color;

        positions[idx] = v2;
        normals[idx] = normal2;
        colors[idx++] = color;

        positions[idx] = v2;
        normals[idx] = normal2;
        colors[idx++] = color;

        positions[idx] = v3;
        normals[idx] = normal1;
        colors[idx++] = color;

        positions[idx] = v4;
        normals[idx] = normal2;
        colors[idx++] = color;
      }
    }
  }
  printf("idx: %d \n", idx);
}

void createCylinders(int stacks, int slices, const CylinderSpec* specs, int numCylinders) 
{
  for (int i = 0; i < numCylinders; i++)
  {
    // Extract the current cylinder's specifications
    vec4 color = specs[i].color;
    vec4 topCenter = specs[i].topCenter;
    vec4 bottomCenter = specs[i].bottomCenter;
    float radius = specs[i].radius;

    // Create the cylinder using the current specifications
    createCylinder(stacks, slices, radius, topCenter, bottomCenter, color);

    // Optionally apply transformations if needed
    ctms[i] = mat_mult(translate(0, 0, 0), ctms[i]);
  }
}

// Helper function to compute a rotation matrix that aligns a vector with the Z-axis
mat4 computeRotationMatrix(vec4 direction)
{
  // Normalize direction
  vec4 normDir = norm_v((vec4){direction.x, direction.y, direction.z, 1});

  // Compute the axis of rotation (cross product of direction and Z-axis)
  vec4 axis = cross_prod((vec4){0.0, 0.0, 1.0, 1.0}, normDir);
  float angle = acos(dot_prod((vec4){0.0, 0.0, 1.0, 1.0}, normDir));

  // Use axis-angle formula to create rotation matrix
  mat4 rotationMatrix = mat4_rotation_axis(angle, axis);

  return rotationMatrix;
}

mat4 mat4_rotation_axis(float angle, vec4 axis)
{
  // Normalize the axis of rotation
  axis = norm_v(axis);

  // Compute the components of the rotation matrix using the axis-angle formula
  float cosAngle = cos(angle);
  float sinAngle = sin(angle);
  float oneMinusCos = 1.0f - cosAngle;

  mat4 rotationMatrix = {
      cosAngle + axis.x * axis.x * oneMinusCos, axis.x * axis.y * oneMinusCos - axis.z * sinAngle, axis.x * axis.z * oneMinusCos + axis.y * sinAngle, 0.0f,
      axis.y * axis.x * oneMinusCos + axis.z * sinAngle, cosAngle + axis.y * axis.y * oneMinusCos, axis.y * axis.z * oneMinusCos - axis.x * sinAngle, 0.0f,
      axis.z * axis.x * oneMinusCos - axis.y * sinAngle, axis.z * axis.y * oneMinusCos + axis.x * sinAngle, cosAngle + axis.z * axis.z * oneMinusCos, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f};

  return rotationMatrix;
}

// NEEDS UPDATE
void createSphere(int stacks, int slices, float radius, vec4 color, vec4 center)
{
  // Calculate total vertices needed
  num_vertices = stacks * slices * 6;

  // Allocate memory for positions and colors
  positions = malloc(num_vertices * sizeof(vec4));
  colors = malloc(num_vertices * sizeof(vec4));

  int idx = 0;
  for (int i = 0; i < stacks; ++i)
  {
    float phi = M_PI * (float)i / stacks;
    float phiNext = M_PI * (float)(i + 1) / stacks;

    for (int j = 0; j < slices; ++j)
    {
      float theta = 2.0f * M_PI * (float)j / slices;
      float thetaNext = 2.0f * M_PI * (float)(j + 1) / slices;

      // Compute sphere vertices, offset by center position
      vec4 v1 = {center.x + radius * sin(phi) * cos(theta),
                 center.y + radius * cos(phi),
                 center.z + radius * sin(phi) * sin(theta),
                 1.0};
      vec4 v2 = {center.x + radius * sin(phiNext) * cos(theta),
                 center.y + radius * cos(phiNext),
                 center.z + radius * sin(phiNext) * sin(theta),
                 1.0};
      vec4 v3 = {center.x + radius * sin(phi) * cos(thetaNext),
                 center.y + radius * cos(phi),
                 center.z + radius * sin(phi) * sin(thetaNext),
                 1.0};
      vec4 v4 = {center.x + radius * sin(phiNext) * cos(thetaNext),
                 center.y + radius * cos(phiNext),
                 center.z + radius * sin(phiNext) * sin(thetaNext),
                 1.0};

      // First triangle
      positions[idx] = v1;
      colors[idx++] = color;
      positions[idx] = v2;
      colors[idx++] = color;
      positions[idx] = v3;
      colors[idx++] = color;

      // Second triangle
      positions[idx] = v2;
      colors[idx++] = color;
      positions[idx] = v4;
      colors[idx++] = color;
      positions[idx] = v3;
      colors[idx++] = color;
    }
  }
}
