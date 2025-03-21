/*
 * presp_func.h
 *
 *  Created on: November 18, 2024
 *      Author: Alli Batell
 */

#ifndef PRESPFUNC_H_

#define PRESPFUNC_H_

#include "myLib.h"

mat4 lookAt(vec4, vec4, vec4);

mat4 ortho(float, float, float,float,float, float);

mat4 frustrum(float, float, float,float,float, float);

#endif