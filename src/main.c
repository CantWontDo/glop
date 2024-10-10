#include <assert.h>

#include "lib/glad/glad.h"
#include "lib/glfw/include/GLFW/glfw3.h"
#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "typedefs.h"

char *read_text_file(char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (!file)
        gl_log_err("Couldn't open file at %s\n", file_name);
    char *buffer = 0;
    fseek(file, 0L, SEEK_END);
    const int buffer_size = ftell(file);
    rewind(file);

    buffer = calloc(buffer_size, sizeof(char));
    int read_size = fread(buffer, sizeof(char), buffer_size, file);
    buffer = (char*)realloc(buffer, read_size + 1);
    fclose(file);
    return buffer;
}

void shdr_check(GLuint shdr_id, char *path)
{
    GLint success = -1;
    glGetShaderiv(shdr_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char info_log[2048];
        glGetShaderInfoLog(shdr_id, sizeof(info_log), 0, info_log);
        gl_log_err("shader log:\n%s\n", info_log);
    }
}

void prog_check(GLuint prog_id)
{
    GLint success = -1;
    glGetProgramiv(prog_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char info_log[2048];
        glGetProgramInfoLog(prog_id, sizeof(info_log), 0, info_log);
        gl_log_err("program log:\n%s\n", info_log);
    }
}
int main(void)
{
    assert(restart_gl_log());
    gl_log("starting GLFW %s\n", glfwGetVersionString());

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
        gl_log_err("Couldn't create window!");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        gl_log_err("Couldn't load GLAD!");
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

    GLuint vbo = 0;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(points), points, GL_DYNAMIC_STORAGE_BIT);

    GLuint vao = 0;
    glCreateVertexArrays(1, &vao);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 6);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);

    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);

    char *v_shdr_src = read_text_file("res/test.vsh");

    char *f_shdr_src = read_text_file("res/test.fsh");

    GLuint v_shdr = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_shdr = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(v_shdr, 1, &v_shdr_src, 0);
    glCompileShader(v_shdr);

    glShaderSource(f_shdr, 1, &f_shdr_src, 0);
    glCompileShader(f_shdr);

    shdr_check(v_shdr, "res/test.vsh");
    shdr_check(f_shdr, "res/test.fsh");
    free(v_shdr_src);
    free(f_shdr_src);

    GLuint shdr_program = glCreateProgram();
    glAttachShader(shdr_program, v_shdr);
    glAttachShader(shdr_program, f_shdr);

    glDeleteShader(v_shdr);
    glDeleteShader(f_shdr);
    glLinkProgram(shdr_program);
    prog_check(shdr_program);

    double prev_sec = 0;
    double current_sec = 0;
    int frame_count = 0;

    m4 test = {1, 2, 12, 1,
                0, 16, 4, 2,
                    2, 0, 4, 2,
                        1, 2, 13, 1};
    float test_det = m4_det(test);
    printf("%f\n", test_det);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.6f, 0.6f, 0.8f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shdr_program);

        glBindVertexArray(vao);
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
