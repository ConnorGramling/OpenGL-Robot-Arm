#include "myLib.h"
#include <stdio.h>
#include <math.h>

float length(vec4 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

void print_v4(vec4 v) {
    printf("[ %7.3f %7.3f %7.3f %7.3f ]\n", v.x, v.y, v.z, v.w);
}

vec4 add_vv(vec4 x, vec4 y){
    vec4 z = 
        {x.x + y.x,
        x.y +y.y,
        x.z + y.z, 
        x.w +y.w};

    return z ;
}

vec4 scal_v_mult(float s, vec4 x){
    vec4 y = 
        {s * x.x,
        s * x.y ,
        s * x.z, 
        s *x.w};

    return y;
}

vec4 sub_vv(vec4 x, vec4 y){
    vec4 z = 
        {x.x - y.x,
        x.y - y.y,
        x.z - y.z, 
        x.w - y.w};

    return z ;
}

float mag_v(vec4 v){
    float x = v.x * v.x;
    float y = v.y * v.y;
    float z = v.z * v.z;
    float w = v.w * v.w;
    float a = (float)sqrt((double)(x + y + z + w));

    return a;
}

vec4 norm_v(vec4 v){
    float a = 1/ mag_v(v);
    vec4 z = 
        {a *v.x,
        a *v.y,
        a * v.z, 
        a * v.w};
    
    return z;
}

float dot_prod(vec4 x, vec4 y){
    float a = x.x * y.x;
    float b = x.y * y.y;
    float c = x.z * y.z;
    float d = x.w * y.w;
    
    return (a+b+c+d);
}

vec4 cross_prod (vec4 v1, vec4 v2){
    float x = (v1.y * v2.z) - (v1.z * v2.y);
    float y = (v1.z * v2.x) - (v1.x * v2.z);
    float z = (v1.x * v2.y) - (v1.y * v2.x);

    vec4 a = {x, y, z, 0};

    return a;

    }

    
void print_mat4 (mat4 m){
    printf("| %7.3f %7.3f %7.3f %7.3f |\n",  m.x.x, m.y.x, m.z.x, m.w.x);
    printf("| %7.3f %7.3f %7.3f %7.3f |\n",  m.x.y, m.y.y, m.z.y, m.w.y);
    printf("| %7.3f %7.3f %7.3f %7.3f |\n",  m.x.z, m.y.z, m.z.z, m.w.z);
    printf("| %7.3f %7.3f %7.3f %7.3f |\n", m.x.w, m.y.w, m.z.w, m.w.w);
    printf("\n");
}

mat4 scal_mat_mult(float s, mat4 m){
    vec4 x = scal_v_mult(s, m.x); 
    vec4 y = scal_v_mult(s, m.y);
    vec4 z = scal_v_mult(s, m.z);
    vec4 w = scal_v_mult(s, m.w);

    mat4 a = {x, y, z, w};
    return a;
}

mat4 add_mm(mat4 m1, mat4 m2){
    vec4 x = add_vv(m1.x, m2.x); 
    vec4 y = add_vv(m1.y, m2.y);
    vec4 z = add_vv(m1.z, m2.z);
    vec4 w = add_vv(m1.w, m2.w);

    mat4 a = {x, y, z, w};
    return a;
}

mat4 sub_mm(mat4 m1, mat4 m2){
    vec4 x = sub_vv(m1.x, m2.x); 
    vec4 y = sub_vv(m1.y, m2.y);
    vec4 z = sub_vv(m1.z, m2.z);
    vec4 w = sub_vv(m1.w, m2.w);

    mat4 a = {x, y, z, w};
    return a;
}

mat4 mat_mult(mat4 m1, mat4 m2){
    vec4 x = m_vec_mult(m1, m2.x);
    vec4 y = m_vec_mult(m1, m2.y);
    vec4 z = m_vec_mult(m1, m2.z);
    vec4 w = m_vec_mult(m1, m2.w);
    
    mat4 m3 = {x, y, z, w};
    return m3;
}

vec4 m_vec_mult(mat4 m, vec4 v){
    float a = (m.x.x * v.x) +(m.y.x * v.y) +(m.z.x * v.z) +(m.w.x * v.w);
    float b = (m.x.y * v.x) +(m.y.y * v.y) +(m.z.y* v.z) +(m.w.y * v.w);
    float c = (m.x.z * v.x) +(m.y.z * v.y) +(m.z.z * v.z) +(m.w.z * v.w);
    float d = (m.x.w * v.x) +(m.y.w * v.y) +(m.z.w * v.z) +(m.w.w * v.w);
    
    vec4 v2 = {a, b, c, d};
    return v2;
}

mat4 minor_m(mat4 m){
    vec4 x = {det_mat(m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w),
            det_mat(m.y.x, m.z.x, m.w.x, m.y.z, m.z.z, m.w.z, m.y.w, m.z.w, m.w.w),
            det_mat(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.w, m.z.w, m.w.w),
            det_mat(m.y.x, m.z.x, m.w.x, m.y.y, m.z.y, m.w.y, m.y.z, m.z.z, m.w.z)};

    vec4 y = {det_mat(m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w),
            det_mat(m.x.x, m.z.x, m.w.x, m.x.z, m.z.z, m.w.z, m.x.w, m.z.w, m.w.w),
            det_mat(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.w, m.z.w, m.w.w),
            det_mat(m.x.x, m.z.x, m.w.x, m.x.y, m.z.y, m.w.y, m.x.z, m.z.z, m.w.z)};

    vec4 z = {det_mat(m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w),
            det_mat(m.x.x, m.y.x, m.w.x, m.x.z, m.y.z, m.w.z, m.x.w, m.y.w, m.w.w),
            det_mat(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.w, m.y.w, m.w.w),
            det_mat(m.x.x, m.y.x, m.w.x, m.x.y, m.y.y, m.w.y, m.x.z, m.y.z, m.w.z)};

   vec4 w = {det_mat(m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w),
            det_mat(m.x.x, m.y.x, m.z.x, m.x.z, m.y.z, m.z.z, m.x.w, m.y.w, m.z.w),
            det_mat(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.w, m.y.w, m.z.w),
            det_mat(m.x.x, m.y.x, m.z.x, m.x.y, m.y.y, m.z.y, m.x.z, m.y.z, m.z.z)} ;

    mat4 a = {x,y,z,w};
    return a;
}

float det_mat(float a, float b, float c, float d, float e, float f, float g, float h, float i){
    return (a*e*i)+ (b*f*g)+(c*d*h)-(g*e*c)-(h*f*a)-(i*d*b);
}

mat4 cofactor_mat(mat4 m){
    vec4 x = col_cofact(m.x, 1);
    vec4 y = col_cofact(m.y, 2);
    vec4 z = col_cofact(m.z, 1);
    vec4 w = col_cofact(m.w, 2);

    mat4 m2 = {x, y, z, w};
    return m2;
}

vec4 col_cofact(vec4 v, int a){
    if (a== 1){
        vec4 x= {v.x, -v.y, v.z,-v.w};
        return x;
    }
    else{
        vec4 x= {-v.x, v.y, -v.z, v.w};
        return x;
    }
}

mat4 trans_mat(mat4 m){
    vec4 x= {m.x.x, m.y.x, m.z.x, m.w.x};
    vec4 y= {m.x.y, m.y.y, m.z.y, m.w.y};
    vec4 z= {m.x.z, m.y.z, m.z.z, m.w.z};
    vec4 w= {m.x.w, m.y.w, m.z.w, m.w.w};

    mat4 m2= {x,y,z,w};
    return m2;
}

mat4 inv_mat(mat4 m){
    mat4 m2 = trans_mat(cofactor_mat(minor_m(m)));
    float x = ((m.x.x*m2.x.x)+ (m.y.x*m2.x.y)+ (m.z.x*m2.x.z)+ (m.w.x*m2.x.w));
    return scal_mat_mult(1/x, m2);
}

mat4 translate(float x, float y, float z){
    mat4 m = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{x, y, z, 1}};
    return m;
}

mat4 scale (float x, float y, float z){
    mat4 m = {{x,0,0,0},{0,y,0,0},{0,0,z,0},{0, 0, 0, 1}};
    return m;
}

mat4 rotate_x(float angle){
    angle = deg_to_rad(angle);
    float cosine = cos(angle);
    float sine = sin(angle);
    mat4 m = {{1, 0,0,0},{0,cosine,sine,0},{0,-sine,cosine,0},{0, 0, 0, 1}};
    return m;
}

mat4 rotate_y(float angle){
    angle = deg_to_rad(angle);
    float cosine = cos(angle);
    float sine = sin(angle);
    mat4 m = {{cosine, 0,-sine,0},{0,1,0,0},{sine,0,cosine,0},{0, 0, 0, 1}};
    return m;
}

mat4 rotate_z(float angle){
    angle = deg_to_rad(angle);
    float cosine = cos(angle);
    float sine = sin(angle);
    mat4 m = {{cosine, sine,0,0},{-sine,cosine,0,0},{0,0,1,0},{0, 0, 0, 1}};
    return m;
}

float deg_to_rad (float deg){
    return (float)((deg * M_PI) /180.0);
}

float rad_to_deg (double rad){
    return (float)((rad*180.0)/M_PI);
}







