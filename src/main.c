#include <assert.h>

#include "lib/glad/glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "typedefs.h"
#include "glop.h"
#include "arr.h"

int main(void)
{
    assert(restart_log());
    log("starting GLFW %s\n", glfwGetVersionString());

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Could not initialize GLFW!\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *vmode = glfwGetVideoMode(monitor);
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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // should be in clockwise order
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    buf vbo = vbo_new();
    buf_data(&vbo, sizeof(points), points, GL_DYNAMIC_STORAGE_BIT);

    vao v = vao_new(&vbo, 0, 2, 3, GL_FLOAT, 3, GL_FLOAT);

    shdr s = shdr_new(2, GL_VERTEX_SHADER, "res/test.vsh", GL_FRAGMENT_SHADER, "res/test.fsh");

    double prev_sec = 0;
    double current_sec = 0;
    int frame_count = 0;

    f32 *test = arr_new(float, 1);
    arr_add_many(test, 18, points);
    arr_header *head = arr_get_header(test);
    printf("%lu\n", head->count);

    printf("%lu\n", head->cap);

    while (!glfwWindowShouldClose(window))
    {
        float loop = sinf(glfwGetTime());
        loop *= loop * loop;

        m4 scale = m4_scale(0.25 * fabsf(loop) + 0.25, 0.5 * fabsf(loop) + 0.5, 1);
        m4 trans = m4_transform(loop * 0.5, 0, 0);

        m4 concat = m4_mul_m(scale, trans);
        glClearColor(0.6f, 0.6f, 0.8f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shdr_bind(&s);
        int loc = glGetUniformLocation(s.id, "world");
        glUniformMatrix4fv(loc, 1, GL_TRUE, &concat.m[0]);

        vao_bind(&v);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);

        current_sec = glfwGetTime();
        double elapsed_sec = current_sec - prev_sec;
        if (elapsed_sec > 0.25)
        {
            prev_sec = current_sec;
            double fps = (double)frame_count / elapsed_sec;
            char buf[128];
            sprintf(buf, "glop @ fps: %.2f", fps);
            glfwSetWindowTitle(window, buf);

            frame_count = 0;
        }
        frame_count++;
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, true);
    }
    glfwTerminate();
    return 0;
}
