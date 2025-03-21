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

    // Lower joint rotation (rotate at the center of the joint)
    mat4 lower_joint_transform = mat_mult(base_rotation, translate(0.0f, 0.2f, 0.0f)); // Move to joint center
    lower_joint_transform = mat_mult(lower_joint_transform, rotate_x(lower_joint_angle));
    lower_joint_transform = mat_mult(lower_joint_transform, translate(0.0f, -0.2f, 0.0f)); // Move back after rotation
    ctms[2] = lower_joint_transform;

    // Lower arm (positioned relative to the lower joint)
    mat4 lower_arm_transform = mat_mult(lower_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[3] = lower_arm_transform;

    // Middle joint rotation
    mat4 middle_joint_transform = mat_mult(lower_arm_transform, translate(0.0f, 0.0f, 0.0f)); 
    middle_joint_transform = mat_mult(middle_joint_transform, rotate_x(middle_joint_angle));
    middle_joint_transform = mat_mult(middle_joint_transform, translate(0.0f, -0.0f, 0.0f));
    ctms[4] = middle_joint_transform;

    // Middle arm (positioned relative to the middle joint)
    mat4 middle_arm_transform = mat_mult(middle_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[5] = middle_arm_transform;

    // Upper joint rotation
    mat4 upper_joint_transform = mat_mult(middle_arm_transform, translate(0.0f, 0.2f, 0.0f));
    upper_joint_transform = mat_mult(upper_joint_transform, rotate_x(upper_joint_angle));     
    upper_joint_transform = mat_mult(upper_joint_transform, translate(0.0f, -0.2f, 0.0f));    
    ctms[6] = upper_joint_transform;

    // Upper arm
    mat4 upper_arm_transform = mat_mult(upper_joint_transform, translate(0.0f, 0.0f, 0.0f));
    ctms[7] = upper_arm_transform;

    // Wrist rotation (rotate at the center of the wrist joint)
    mat4 wrist_transform = mat_mult(upper_joint_transform, translate(0.0f, 0.2f, 0.0f)); 
    wrist_transform = mat_mult(wrist_transform, rotate_y(wrist_angle));
    wrist_transform = mat_mult(wrist_transform, translate(0.0f, -0.2f, 0.0f)); 
    ctms[8] = wrist_transform;

    // Left finger (positioned and rotated relative to the wrist)
    mat4 left_finger_transform = mat_mult(wrist_transform, translate(0.0f, 0.0f, 0.0f)); 
    left_finger_transform = mat_mult(left_finger_transform, rotate_z(finger_angle));
    ctms[9] = left_finger_transform;

    // Right finger (positioned and rotated relative to the wrist)
    mat4 right_finger_transform = mat_mult(wrist_transform, translate(0.0f, 0.0f, 0.0f)); 
    right_finger_transform = mat_mult(right_finger_transform, rotate_z(-finger_angle));
    ctms[10] = right_finger_transform;

    glUniform4fv(light_position_location, 1, (GLvoid *)&light_position);
    glutSwapBuffers();
}