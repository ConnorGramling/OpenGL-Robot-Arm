#include "myLib.h"
/*
 * presp_func.c
 *
 *  Created on: December 3, 2024
 *      Author: Connor Gramling
 */

mat4 lookAt(vec4 eye, vec4 at, vec4 up){
    vec4 vpn = sub_vv(eye, at);
    vec4 n = norm_v(vpn);
    vec4 u = norm_v(cross_prod(up, n));
    vec4 v = cross_prod(n, u);
    
    mat4 R = (mat4){
        {u.x, v.x, n.x, 0},
        {u.y, v.y, n.y, 0},
        {u.z, v.z, n.z, 0},
        {0, 0, 0, 1}
    };
    mat4 T = translate(-eye.x, -eye.y, -eye.z);
    return mat_mult(R, T);
}

mat4 frustrum(float left, float right, float bottom, float top, float near, float far) {
    return (mat4){
        {2 * near / (right - left), 0, (right + left) / (right - left), 0},
        {0, 2 * near / (top - bottom), (top + bottom) / (top - bottom), 0},
        {0, 0, -(far + near) / (far - near), -(2 * far * near) / (far - near)},
        {0, 0, -1, 0}
    };
}

