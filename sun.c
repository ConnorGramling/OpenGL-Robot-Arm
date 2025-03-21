/*
 * sun.c
 *
 *  Created on: November 22, 2024
 *      Author: Connor Gramling
 */

#include "sun.h"
#include "myLib.h"
#include "main.h"
#include <math.h>

float sun_x = 0.0f;
float sun_y = 3.0f;
float sun_z = 0.0f;
float rotation_angle = 0.0f;

void rotateSunX(float angle)
{
    // Convert angle to radians for trigonometric functions
    float radians = angle * M_PI / 180.0f;

    // Calculate new sun position using a rotation matrix around the X-axis
    float new_y = sun_y * cos(radians) - sun_z * sin(radians);
    float new_z = sun_y * sin(radians) + sun_z * cos(radians);

    // Update the sun's position
    sun_y = new_y;
    sun_z = new_z;

    // Update the light position
    updateLightPosition();

    //printf("Sun position updated: (%.2f, %.2f, %.2f)\n", sun_x, sun_y, sun_z);
}

void rotateSunY(float angle)
{
    // Convert angle to radians for trigonometric functions
    float radians = angle * M_PI / 180.0f;

    // Calculate new sun position using a rotation matrix around the Y-axis
    float new_x = sun_x * cos(radians) + sun_z * sin(radians);
    float new_z = -sun_x * sin(radians) + sun_z * cos(radians);

    // Update the sun's position
    sun_x = new_x;
    sun_z = new_z;

    // Update the light position
    updateLightPosition();

    //printf("Sun position updated: (%.2f, %.2f, %.2f)\n", sun_x, sun_y, sun_z);
}

void updateLightPosition()
{
    // Set light position to the current sun position
    light_position.x = sun_x;
    light_position.y = sun_y;
    light_position.z = sun_z;

    updateLight();

    // Re-upload the updated light position to the GPU
    glUniform4fv(light_position_location, 1, (GLvoid *)&light_position);

    printf("Light position updated: (%.2f, %.2f, %.2f)\n", light_position.x, light_position.y, light_position.z);
}
