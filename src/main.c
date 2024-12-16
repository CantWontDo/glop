#include <assert.h>

#include "lib/glad/glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include <stdio.h>
#include <stdlib.h>

#include "lib/tracy/public/tracy/TracyC.h"
#include "err.h"
#include "typedefs.h"
#include "glop.h"
#include "arr.h"
#include "map.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    assert(restart_log());
    restart_log();
    log_info("starting GLFW %s\n", glfwGetVersionString());

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Could not initialize GLFW!\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    GLFWvidmode *vmode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "glop", 0, 0);

    if (!window)
    {
        log_err("Couldn't create window!");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log_err("Couldn't load GLAD!");
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // TODO (rahul): Check these out again sometime when they don't break things.
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CW);

    i32 win_pos_x = 0;
    i32 win_pos_y = 0;
    glfwGetWindowPos(window, &win_pos_x, &win_pos_y);

    i32 device_width = 0;
    i32 device_height = 0;
    glfwGetMonitorPhysicalSize(monitor, &device_width, &device_height);

    i32 window_width = 0;
    i32 window_height = 0;
    glfwGetWindowSize(window, &window_width, &window_height);

    f32 pixel_aspect_ratio = ((f32) vmode->height/ vmode->width)
        * ((f32)device_width / (f32)device_height);

    f32 win_phys_aspect_ratio = ((f32)window_width / (f32)window_height)
        * pixel_aspect_ratio;

    f32 fov_x = PI / 2;
    f32 zoom_x = 1 / tanf(fov_x / 2);
    f32 zoom_y = zoom_x * win_phys_aspect_ratio;
    f32 fov_y = 2 * atanf(1 / zoom_y);
    
    log_info("pixel size: %f\n", pixel_aspect_ratio);
    log_info("physical aspect ratio: %f\n", win_phys_aspect_ratio);
    log_info("fov_x: %f\n", fov_x);
    log_info("fov_y: %f\n", fov_y);
    log_info("zoom x: %f\n", zoom_x);
    log_info("zoom y: %f\n", zoom_y);

    f32 near = 0.1f;
    f32 far = 100.0f;
    f32 distance = fabsf(far - near);

    GLfloat points[] = {
        0.25f, 0.25f, 0,
        1.0f, 0.0f, 0.0f,
        0.25f, -0.25f, 0,
        0.0f, 1.0f, 0.0f,
        -0.25f, -0.25f, 0,
        0.0f, 0.0f, 1.0f,
        -0.25f, 0.25f, 0,
        1.0f, 1.0f, 1.0f
    };

    u32 indices[] =
        {0, 1, 3,
         1, 2, 3};

    buf vbo = vbo_new();
    buf_data(&vbo, sizeof(points), points, GL_DYNAMIC_STORAGE_BIT);

    buf ebo = ebo_new();
    buf_data(&ebo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

    vao v = vao_new(&vbo, &ebo, 2, 3, GL_FLOAT, 3, GL_FLOAT);

    shdr s = shdr_new(2, GL_VERTEX_SHADER, "res/test.vsh", GL_FRAGMENT_SHADER, "res/test.fsh");

    f64 prev_time = 0;
    f64 current_time = 0;
    f64 frame_time = 0;
    i32 frame_count = 0;

    f64 fps = 0;

    log_info("near plane: %f\n", near);
    log_info("far plane: %f\n", far);
    log_info("plane distance: %f\n", distance);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    v3 cam_pos = {0, 0, 0.9325};

    f64 cursor_x_pos = window_width / 2;
    f64 cursor_y_pos = window_height / 2;
    f64 cursor_x_prev_pos = 0;
    f64 cursor_y_prev_pos = 0;

    glfwSetCursorPos(window, cursor_x_pos, cursor_y_pos);

    f32 cam_heading = 0;
    f32 cam_pitch = 0;

    while (!glfwWindowShouldClose(window))
    {
        TracyCFrameMark;
        TracyCZoneN(timer, "frame calculations", true);
        prev_time = current_time;
        current_time = glfwGetTime();
        f64 elapsed_time = current_time - prev_time;
        f64 frame_elapsed = current_time - frame_time;

        if (frame_elapsed > 0.25)
        {
            frame_time = current_time;
            fps = (f64)frame_count / frame_elapsed;

            char buf[128];
            sprintf(buf, "glop @ fps: %.2f", fps);
            glfwSetWindowTitle(window, buf);

            frame_count = 0;
        }
        frame_count++;
        TracyCZoneEnd(timer);

        TracyCZoneN(cursor, "cursor calculations", true);
        cursor_x_prev_pos = cursor_x_pos;
        cursor_y_prev_pos = cursor_y_pos;

        glfwGetCursorPos(window, &cursor_x_pos, &cursor_y_pos);

        // f32 cursor_x_offset = cursor_x_pos - cursor_x_prev_pos;
        // f32 cursor_y_offset = cursor_y_pos - cursor_y_prev_pos;

        // f32 heading_change = cursor_x_offset * elapsed_time * PI / 180 * 10;
        // f32 pitch_change = -cursor_y_offset * elapsed_time * PI / 180 * 10;

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
            cam_heading += elapsed_time * 250;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
            cam_heading -= elapsed_time * 250;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP))
            cam_pitch -= elapsed_time * 250;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN))
            cam_pitch += elapsed_time * 250;

        if (cam_pitch > 90)
            cam_pitch = 90;
        if (cam_pitch < -90)
            cam_pitch = -90;

        log_info("cam pitch: %f\n", cam_pitch);
        log_info("cam heading: %f\n", cam_heading);

        TracyCZoneEnd(cursor);

        f32 loop = sinf(glfwGetTime());
        f32 loop2 = cosf(glfwGetTime());

        loop = loop * loop * loop;
        loop2 = loop2 * loop2 * loop2;

        TracyCZoneN(transforms, "transforms", true);
        m4 scale = m4_scale_u(10);
        m4 trans = m4_translate(0, 0, 0);

        m4 perspective = m4_perspective(fov_x, win_phys_aspect_ratio, near, far);

        quat quat_two = quat_from_euler_rad(0, 0, 0);

        m4 rot = quat_to_m4(quat_two);

        scale = m4_mul_m(scale, rot);
        scale = m4_mul_m(scale, trans);

        TracyCZoneN(cam_transforms, "cam transforms", true);

        v3 x_offset = v3_ux;
        v3 z_offset = v3_uz;

        m3 offset_rot = m3_rot_y(cam_heading);
        // x_offset = v3_mul_m3(offset_rot, x_offset);
        // x_offset = v3_norm(x_offset);
        // z_offset = v3_mul_m3(offset_rot, z_offset);
        // z_offset = v3_norm(z_offset);

        v3 cam_offset = v3_zero;
        float cam_speed = 10;

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
            cam_offset = v3_add(cam_offset, x_offset);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
            cam_offset = v3_sub(cam_offset, x_offset);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
            cam_offset = v3_sub(cam_offset, z_offset);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
            cam_offset = v3_add(cam_offset, z_offset);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
            cam_offset.y = 1;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
            cam_offset.y = -1;

        if (cam_offset.x != 0 || cam_offset.y != 0 || cam_offset.z != 0)
            cam_pos = v3_add(cam_pos, v3_mul(v3_norm(cam_offset), elapsed_time * cam_speed));

        m4 view = m4_look_at(cam_pos, v3_add(cam_pos, v3_mul(z_offset, -1)), v3_uy);
        m4 view_rot = quat_to_m4(quat_from_euler(cam_pitch, cam_heading, 0));
        view_rot = m4_transpose(view_rot);
        view = m4_mul_m(view, view_rot);

        TracyCZoneEnd(cam_transforms);
        TracyCZoneEnd(transforms);

        glClearColor(0.6, 0.6, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        TracyCZoneN(shader, "shader stuff", true);
        shdr_bind(&s);
        shdr_m4f(&s, "world", &scale);
        shdr_m4f(&s, "projection", &perspective);
        shdr_m4f(&s, "view", &view);
        f32 time = glfwGetTime();
        shdr_1f(&s, "time", &time);
        TracyCZoneEnd(shader);

        vao_bind(&v);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwPollEvents();
        glfwSwapBuffers(window);

       if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);
    }
    glfwTerminate();
    return 0;
}
