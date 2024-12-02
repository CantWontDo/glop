#include <assert.h>

#include "lib/glad/glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include <stdio.h>
#include <stdlib.h>

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

    v3 cam_pos = v3_zero;

    f64 cursor_x_pos = window_width / 2;
    f64 cursor_y_pos = window_height / 2;
    f64 cursor_x_prev_pos = 0;
    f64 cursor_y_prev_pos = 0;

    glfwSetCursorPos(window, cursor_x_pos, cursor_y_pos);

    f64 cam_pitch = 0;
    f64 cam_heading = -PI / 2;
    while (!glfwWindowShouldClose(window))
    {
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

        cursor_x_prev_pos = cursor_x_pos;
        cursor_y_prev_pos = cursor_y_pos;
        glfwGetCursorPos(window, &cursor_x_pos, &cursor_y_pos);

        f32 cursor_x_offset = cursor_x_pos - cursor_x_prev_pos;
        f32 cursor_y_offset = cursor_y_pos - cursor_y_prev_pos;

        f32 pitch_change = cursor_y_offset * PI / 180 * 11;
        f32 heading_change = cursor_x_offset * PI / 180 * 11;

        log_info("pitch change: %f\n ", cursor_x_offset);
        log_info("heading change: %f\n ", cursor_y_offset);

        cam_heading += heading_change;
        cam_pitch += pitch_change;

        if (cam_pitch > 45.0)
            cam_pitch = 45.0;
        if (cam_pitch < -45.0)
            cam_pitch = -45.0;

        f32 loop = sinf(glfwGetTime());
        f32 loop2 = cosf(glfwGetTime());

        loop = loop * loop * loop;
        loop = loop * loop * loop;
        loop2 = loop2 * loop2 * loop2;
        loop2 = loop2 * loop2 * loop2;

        m4 scale = m4_scale(1.5, 1.5, 1);
        // m4 trans = m4_transform(loop2 * 0.5 - loop * 0.5, loop * 0.5 + loop2 * 0.5,
            // (loop2 * 0.5 - loop * 0.5) * -distance);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
            cam_pos.x += elapsed_time * 3;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
            cam_pos.x -= elapsed_time * 3;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
            cam_pos.z -= elapsed_time * 3;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
            cam_pos.z += elapsed_time * 3;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_SPACE))
            cam_pos.y += elapsed_time * 3;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
            cam_pos.y -= elapsed_time * 3;
        m4 trans = m4_transform(0, 0, 0);
        m4 perspective = m4_perspective(fov_x, win_phys_aspect_ratio, near, far);

        m4 view = m4_transform(cam_pos.x, cam_pos.y, cam_pos.z);
        quat quat_view = quat_from_euler(cam_pitch, cam_heading, 0);
        quat quat_view_slerp = quat_from_euler(cam_pitch, cam_heading, 0);
        quat view_result = quat_slerp(quat_view, quat_view_slerp, glfwGetTime());

        m4 view_rot = quat_to_m4(quat_view_slerp);

        view = m4_mul_m(view, view_rot);
        // TODO: optimize inverse by transposing rotation and just negating translation (adjoint is overkill)
        view = m4_inv(view);

        quat quat_one = quat_from_euler(0, 0, 0);
        quat quat_two = quat_from_euler(0, 0, 0);

        quat result = quat_slerp(quat_one, quat_two, (sinf(glfwGetTime() * 2) + 1) / 2);
        m4 rot = quat_to_m4(result);

        scale = m4_mul_m(scale, rot);
        scale = m4_mul_m(scale, trans);

        glClearColor(0.6, 0.6, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shdr_bind(&s);
        shdr_m4f(&s, "world", &scale);
        shdr_m4f(&s, "projection", &perspective);
        shdr_m4f(&s, "view", &view);
        f32 time = glfwGetTime();
        shdr_1f(&s, "time", &time);

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
