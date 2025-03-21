/*
 * lighting.c
 *
 *  Created on: November 24, 2024
 *      Author: Connor Gramling
 */

#include "main.h"
#include "myLib.h"
#include "lighting.h"
#include <string.h>

// Flags for lighting components
bool ambient_on = true;
bool diffuse_on = true;
bool specular_on = true;

vec4 *base_colors; // Array to store the original colors

void initializeBaseColors() {
    base_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    memcpy(base_colors, colors, sizeof(vec4) * num_vertices); // Copy original colors
}

void updateLight() {

    //printf("Update Light");

    vec4 *temp_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices); // Allocate temporary array

    // Start with base colors
    memcpy(temp_colors, base_colors, sizeof(vec4) * num_vertices);

    // Ambient
    for (int i = 0; i < num_vertices; i++) {
        if (ambient_on) {
            temp_colors[i] = add_vv(temp_colors[i], scal_v_mult(0.3, base_colors[i]));
        }
    }

    // Diffuse
    for (int i = 0; i < num_vertices; i++) {
        vec4 l = norm_v(sub_vv(light_position, positions[i]));
        vec4 n = normals[i];
        float l_dot_n = dot_prod(l, n);
        if (l_dot_n < 0) l_dot_n = 0;
        vec4 diffuse_contribution = (diffuse_on) ? scal_v_mult(l_dot_n, base_colors[i]) : (vec4){0, 0, 0, 0};
        temp_colors[i] = add_vv(temp_colors[i], diffuse_contribution);
    }

    // Specular
    for (int i = 0; i < num_vertices; i++) {
        vec4 l = norm_v(sub_vv(light_position, positions[i]));
        vec4 v = norm_v(sub_vv(eye, positions[i]));
        vec4 h = norm_v(add_vv(l, v));
        vec4 n = normals[i];
        float h_dot_n = dot_prod(h, n);
        if (h_dot_n < 0) h_dot_n = 0;
        vec4 specular_contribution = (specular_on) ? scal_v_mult(pow(h_dot_n, 50), (vec4){1, 1, 1, 1}) : (vec4){0, 0, 0, 0};
        temp_colors[i] = add_vv(temp_colors[i], specular_contribution);
    }

    // Copy the computed colors back to the `colors` array
    memcpy(colors, temp_colors, sizeof(vec4) * num_vertices);
    free(temp_colors);

    glUniform1i(ambient_on_location, ambient_on ? 1 : 0);
    glUniform1i(diffuse_on_location, diffuse_on ? 1 : 0);
    glUniform1i(specular_on_location, specular_on ? 1 : 0);

    // Update the GPU buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
}