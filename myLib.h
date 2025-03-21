#ifndef _MYLIB_H_

#define _MYLIB_H_

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct {
    vec4 x;
    vec4 y;
    vec4 z;
    vec4 w;
} mat4;

// Insert function signatures after this line

float length(vec4 v);

float clamp(float value, float min, float max);

void print_v4(vec4);

vec4 scal_v_mult(float, vec4);

vec4 add_vv(vec4, vec4);

vec4 sub_vv(vec4, vec4);

float mag_v(vec4);

vec4 norm_v(vec4);

float dot_prod (vec4, vec4);

vec4 cross_prod (vec4, vec4);


void print_mat4 (mat4);

mat4 scal_mat_mult(float, mat4);

mat4 add_mm(mat4, mat4);

mat4 sub_mm(mat4, mat4);

mat4 mat_mult(mat4, mat4);

vec4 m_vec_mult(mat4, vec4);

mat4 minor_m(mat4);

float det_mat(float, float, float, float, float, float, float, float, float);

mat4 cofactor_mat(mat4);

vec4 col_cofact(vec4, int);

mat4 trans_mat(mat4);

mat4 inv_mat(mat4);

mat4 translate(float, float, float);

mat4 scale (float, float, float);

mat4 rotate_x(float);

mat4 rotate_y(float);

mat4 rotate_z(float);

float deg_to_rad (float);

float rad_to_deg (double);
// Do not put anything after this line

#endif
