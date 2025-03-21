/*
 * main.c
 *
 *  Created on: November 28, 2024
 *      Author: Connor Gramling
 */

#ifdef __APPLE__ // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif // __APPLE__

#include "initShader.h"
#include "main.h"
#include "myLib.h"

int num_vertices;
vec4 *positions;
vec4 *colors;
vec4 *normals;

mat4 ctms[NUM_CYLINDERS];

mat4 ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint ctm_location;
mat4 previous_ctm = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

GLuint model_view_location;
mat4 model_view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
GLuint projection_location;
mat4 projection = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

float previous_x;
float previous_y;
bool left_press = true;
int quadrant = 1;

GLuint buffer;
GLuint light_position_location;
GLuint ambient_on_location, diffuse_on_location, specular_on_location;
vec4 *ambient_colors;
vec4 *diffuse_colors;
vec4 *specular_colors;
vec4 light_position = {0, 3, 0, 1};

mat4 base_projection;

vec4 eye, at, up;
float zoom_left = -1, zoom_right = 1, zoom_top = 1, zoom_bottom = -1, zoom_near = 1, zoom_far = 10;

float base_angle = 0.0f;
float lower_joint_angle = 0.0f;
float middle_joint_angle = 0.0f;
float upper_joint_angle = 0.0f;
float wrist_angle = 0.0f;
float finger_angle = 0.0f; // For opening/closing fingers

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Total vertices needed (27060)
    num_vertices = (STACKS * SLICES * 6 + SLICES * 6) * NUM_CYLINDERS;

    printf("Total: %d \n", num_vertices);

    // Allocate memory
    positions = (vec4 *)malloc(num_vertices * sizeof(vec4));
    colors = (vec4 *)malloc(num_vertices * sizeof(vec4));
    normals = (vec4 *)malloc(num_vertices * sizeof(vec4));
    if (positions == NULL || colors == NULL || normals == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    // Initialize CTMs to identity matrices
    for (int i = 0; i < NUM_CYLINDERS; i++)
    {
        ctms[i] = (mat4){{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    }

    createCylinders(STACKS, SLICES, cylinders, NUM_CYLINDERS);

    ambient_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    diffuse_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);
    specular_colors = (vec4 *)malloc(sizeof(vec4) * num_vertices);

    initializeBaseColors();
    updateLight();

    eye = (vec4){0.0, -0.5, 1.5, 1.0};
    at = (vec4){0.0, 0.0, 0.0, 1.0};
    up = (vec4){0.0, 1.0, 0.0, 0.0};

    model_view = lookAt(eye, at, up);

    projection = frustrum(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    base_projection = projection;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices + sizeof(vec4) * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, sizeof(vec4) * num_vertices, normals);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * num_vertices));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(vec4) * 2 * num_vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    model_view_location = glGetUniformLocation(program, "model_view");
    projection_location = glGetUniformLocation(program, "projection");

    light_position_location = glGetUniformLocation(program, "light_position");
    glUniform4fv(light_position_location, 1, (GLvoid *)&light_position);

    ambient_on_location = glGetUniformLocation(program, "ambient_on");
    diffuse_on_location = glGetUniformLocation(program, "diffuse_on");
    specular_on_location = glGetUniformLocation(program, "specular_on");

    glUniform1i(ambient_on_location, ambient_on ? 1 : 0);
    glUniform1i(diffuse_on_location, diffuse_on ? 1 : 0);
    glUniform1i(specular_on_location, specular_on ? 1 : 0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1, 0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Upload projection and model view matrices once
    glUniformMatrix4fv(model_view_location, 1, GL_FALSE, (GLfloat *)&model_view);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);

    // Render each cylinder with its own CTM
    int vertices_per_cylinder = num_vertices / NUM_CYLINDERS;
    for (int i = 0; i < NUM_CYLINDERS; i++)
    {
        glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *)&ctms[i]);
        glDrawArrays(GL_TRIANGLES, i * vertices_per_cylinder, vertices_per_cylinder);
    }
    // print_mat4(ctm);
    // Base rotation
    mat4 base_rotation = rotate_y(base_angle);
    ctms[1] = base_rotation;

    // Lower joint rotation
    mat4 lower_joint_transform = mat_mult(base_rotation, translate(0.0f, 0.2f, 0.0f)); // Move to joint center
    lower_joint_transform = mat_mult(lower_joint_transform, rotate_x(lower_joint_angle));
    lower_joint_transform = mat_mult(lower_joint_transform, translate(0.0f, -0.2f, 0.0f)); // Move back after rotation
    ctms[2] = lower_joint_transform;

    // Lower arm
    mat4 lower_arm_transform = mat_mult(lower_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[3] = lower_arm_transform;

    // Middle joint rotation
    mat4 middle_joint_transform = mat_mult(lower_arm_transform, translate(0.0f, 0.6f, 0.0f)); 
    middle_joint_transform = mat_mult(middle_joint_transform, rotate_x(middle_joint_angle));
    middle_joint_transform = mat_mult(middle_joint_transform, translate(0.0f, -0.6f, 0.0f));
    ctms[4] = middle_joint_transform;

    // Middle arm
    mat4 middle_arm_transform = mat_mult(middle_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[5] = middle_arm_transform;

    // Upper joint rotation
    mat4 upper_joint_transform = mat_mult(middle_arm_transform, translate(0.0f, 1.0f, 0.0f));
    upper_joint_transform = mat_mult(upper_joint_transform, rotate_x(upper_joint_angle));     
    upper_joint_transform = mat_mult(upper_joint_transform, translate(0.0f, -1.0f, 0.0f));    
    ctms[6] = upper_joint_transform;

    // Upper arm
    mat4 upper_arm_transform = mat_mult(upper_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[7] = upper_arm_transform;

    // Wrist rotation
    mat4 wrist_transform = mat_mult(upper_joint_transform, translate(0.0f, 0.0f, 0.0f)); 
    wrist_transform = mat_mult(wrist_transform, rotate_y(wrist_angle));
    wrist_transform = mat_mult(wrist_transform, translate(0.0f, -0.0f, 0.0f)); 
    ctms[8] = wrist_transform;

    // Left finger
    mat4 left_finger_transform = mat_mult(wrist_transform, translate(0.0f, 0.0f, 0.0f)); 
    left_finger_transform = mat_mult(left_finger_transform, rotate_z(finger_angle));
    ctms[9] = left_finger_transform;

    // Right finger
    mat4 right_finger_transform = mat_mult(wrist_transform, translate(0.0f, 0.0f, 0.0f)); 
    right_finger_transform = mat_mult(right_finger_transform, rotate_z(-finger_angle));
    ctms[10] = right_finger_transform;

    glUniform4fv(light_position_location, 1, (GLvoid *)&light_position);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'l')
    {
        glutLeaveMainLoop();
    }
    else if (key == ',')
    { // Rotate sun counterclockwise around X-axis
        rotation_angle -= 45.0f;
        rotateSunX(-45.0f);
    }
    else if (key == '.')
    { // Rotate sun clockwise around X-axis
        rotation_angle += 45.0f;
        rotateSunX(45.0f);
    }
    else if (key == '\'')
    { // Rotate sun counterclockwise around Y-axis
        rotateSunY(-45.0f);
    }
    else if (key == ';')
    { // Rotate sun clockwise around Y-axis
        rotateSunY(45.0f);
    }
    switch (key)
    {
    // Rotate base
    case 'a':
        base_angle -= 5.0f;
        break;
    case 'd':
        base_angle += 5.0f;
        break;

    // Rotate lower joint
    case 'w':
        lower_joint_angle += 5.0f;
        break;
    case 's':
        lower_joint_angle -= 5.0f;
        break;

    // Rotate middle joint
    case 'q':
        middle_joint_angle += 5.0f;
        break;
    case 'e':
        middle_joint_angle -= 5.0f;
        break;

    // Rotate upper joint
    case 'v':
        upper_joint_angle += 5.0f;
        break;
    case 'b':
        upper_joint_angle -= 5.0f;
        break;

    // Rotate wrist
    case 'z':
        wrist_angle -= 5.0f;
        break;
    case 'c':
        wrist_angle += 5.0f;
        break;

    // Finger control
    case 'o':
        finger_angle -= 0.1f;
        break; // Close fingers
    case 'p':
        finger_angle += 0.1f;
        break; // Open fingers
    }

    float angle;
    vec4 direction;
    float radius;
    float current_angle;
    float horizontal_radius;
    float new_angle;
    float x;
    float z;
    switch (key)
    {
    case '2':                                                                                            // Move eye down
        angle = 5.0f * (M_PI / 180.0f);                                                                  // Rotate by 5 degrees
        direction = sub_vv(eye, at);                                                                     // Vector from 'at' to 'eye'
        radius = length(direction);                                                                      // Distance from 'eye' to 'at'
        current_angle = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)); // Current vertical angle

        // New vertical angle after rotation
        new_angle = current_angle + angle;

        // Update eye position using spherical coordinates
        eye.y = at.y + radius * sin(new_angle);
        horizontal_radius = radius * cos(new_angle);
        eye.x = at.x + direction.x / sqrt(direction.x * direction.x + direction.z * direction.z) * horizontal_radius;
        eye.z = at.z + direction.z / sqrt(direction.x * direction.x + direction.z * direction.z) * horizontal_radius;
        break;
    case '8':                                                                                            // Move eye up
        angle = -5.0f * (M_PI / 180.0f);                                                                 // Rotate by -5 degrees
        direction = sub_vv(eye, at);                                                                     // Vector from 'at' to 'eye'
        radius = length(direction);                                                                      // Distance from 'eye' to 'at'
        current_angle = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)); // Current vertical angle

        // New vertical angle after rotation
        new_angle = current_angle + angle;

        // Update eye position using spherical coordinates
        eye.y = at.y + radius * sin(new_angle);
        horizontal_radius = radius * cos(new_angle);
        eye.x = at.x + direction.x / sqrt(direction.x * direction.x + direction.z * direction.z) * horizontal_radius;
        eye.z = at.z + direction.z / sqrt(direction.x * direction.x + direction.z * direction.z) * horizontal_radius;
        break;
    case '4':                           // Move eye left
        angle = 5.0f * (M_PI / 180.0f); // Convert degrees to radians
        direction = sub_vv(eye, at);    // Vector from 'at' to 'eye'
        x = direction.x * cos(angle) - direction.z * sin(angle);
        z = direction.x * sin(angle) + direction.z * cos(angle);
        eye.x = at.x + x;
        eye.z = at.z + z;
        break;
    case '6':                            // Move eye right
        angle = -5.0f * (M_PI / 180.0f); // Convert degrees to radians
        direction = sub_vv(eye, at);     // Vector from 'at' to 'eye'
        x = direction.x * cos(angle) - direction.z * sin(angle);
        z = direction.x * sin(angle) + direction.z * cos(angle);
        eye.x = at.x + x;
        eye.z = at.z + z;
        break;
    case '+': // Zoom in
        zoom_left *= 0.9;
        zoom_right *= 0.9;
        zoom_bottom *= 0.9;
        zoom_top *= 0.9;
        break;
    case '-': // Zoom out
        zoom_left *= 1.1;
        zoom_right *= 1.1;
        zoom_bottom *= 1.1;
        zoom_top *= 1.1;
        break;
    case 'r': // Reset frustum
        zoom_left = -1;
        zoom_right = 1;
        zoom_top = 1;
        zoom_bottom = -1;
        zoom_near = 1;
        zoom_far = -1;
        break;
    }
    model_view = lookAt(eye, at, up);
    updateFrustum();
    //updateLightPosition();
    glutPostRedisplay(); // Trigger a redraw
}

void mouse(int button, int state, int x, int y)
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        left_press = true;
        previous_x = (x * 2.0 / 511.0) - 1;
        previous_y = -((y * 2.0 / 511.0) - 1);
        previous_ctm = ctm;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        left_press = false;
        previous_ctm = ctm;
    }
    if (button == 3)
    {
        zoom_left = .98 * zoom_left;
        zoom_right = .98 * zoom_right;
        zoom_top = .98 * zoom_top;
        zoom_bottom = .98 * zoom_bottom;
        zoom_near = 1.00 * zoom_near;
        zoom_far = 1.00 * zoom_far;
        projection = frustrum(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    }
    if (button == 4)
    {
        zoom_left = 1.02 * zoom_left;
        zoom_right = 1.02 * zoom_right;
        zoom_top = 1.02 * zoom_top;
        zoom_bottom = 1.02 * zoom_bottom;
        zoom_near = .98 * zoom_near;
        zoom_far = 1.02 * zoom_far;
        projection = frustrum(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    }

    glutPostRedisplay();
}
void motion(int x, int y)
{
    if (left_press == true)
    {
        float x_coord = (x * 2.0 / 511.0) - 1;
        float y_coord = -((y * 2.0 / 511.0) - 1);
        if (sqrt((x_coord * x_coord) + (y_coord * y_coord)) <= 1 && sqrt((previous_x * previous_x) + (previous_y * previous_y)) <= 1)
        {
            float z_coord = (float)(1 - (x_coord * x_coord) - (y_coord * y_coord));
            vec4 axis2 = ((vec4){x_coord, y_coord, z_coord, 0});
            vec4 axis1 = ((vec4){previous_x, previous_y, (1 - (previous_x * previous_x) - (previous_y * previous_y)), 0});
            vec4 axis = norm_v(cross_prod(axis1, axis2));
            float d = (float)sqrt((axis.y * axis.y) + (axis.z * axis.z));
            mat4 x_rotate_matrix = {{1, 0, 0, 0}, {0, (axis.z / d), (axis.y / d), 0}, {0, (-axis.y / d), (axis.z / d), 0}, {0, 0, 0, 1}};
            mat4 neg_y_rotate_matrix = {{d, 0, axis.x, 0}, {0, 1, 0, 0}, {-axis.x, 0, d, 0}, {0, 0, 0, 1}};
            float degree = (rad_to_deg(acos(dot_prod(axis1, axis2) / (mag_v(axis1) * mag_v(axis2)))));

            mat4 ctm2 = mat_mult(trans_mat(x_rotate_matrix), mat_mult(trans_mat(neg_y_rotate_matrix), mat_mult(rotate_z(degree), mat_mult(neg_y_rotate_matrix, x_rotate_matrix))));
            ctm = mat_mult(ctm2, previous_ctm);
        }
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    // glutInitWindowPosition(100, 100);
    glutCreateWindow("Robot Arm");
#ifndef __APPLE__
    glewInit();
#endif
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    // glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();

    return 0;
}

void updateFrustum()
{
    projection = frustrum(zoom_left, zoom_right, zoom_bottom, zoom_top, zoom_near, zoom_far);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, (GLfloat *)&projection);
}
