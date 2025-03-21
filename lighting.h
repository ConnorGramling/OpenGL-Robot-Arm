/*
 * lighting.h
 *
 *  Created on: November 24, 2024
 *      Author: Connor Gramling
 */

#ifndef LIGHTING_H_
#define LIGHTING_H_

#include "main.h"
#include "myLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void updateLight();
void initializeBaseColors();

extern bool ambient_on;
extern bool diffuse_on;
extern bool specular_on;

#endif /* LIGHTING_H_ */
