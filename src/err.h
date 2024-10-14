//
// Created by Rahul Pardeshi on 10/4/2024.
//

#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>
#define GL_LOG_FILEPATH "gl.log"

bool restart_log();
bool log_err(const char *msg, ...);
bool log(const char *msg, ...);

void glfw_error_callback(int error, const char *description);
#endif //LOG_H
