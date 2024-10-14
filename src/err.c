//
// Created by iamro on 10/4/2024.
//

#include "err.h"

bool restart_log()
{
    FILE* file = fopen(GL_LOG_FILEPATH, "w");
    if (!file)
    {
        fprintf(stderr, "Couldn't open log at %s\n", GL_LOG_FILEPATH);
        return false;
    }

    time_t now = time(0);
    char *date = ctime(&now);
    fprintf(file, "Began logging at %s\n", date);
    fclose(file);
    return true;
}

bool log_err(const char *msg, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILEPATH, "a");
    if (!file)
    {
        fprintf(stderr, "Couldn't open log at %s\n", GL_LOG_FILEPATH);
        return false;
    }
    va_start(argptr, msg);
    vfprintf(file, msg, argptr);
    va_end(argptr);
    va_start(argptr, msg);
    vfprintf(stderr, msg, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool log_info(const char *msg, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILEPATH, "a");
    if (!file)
    {
        fprintf(stderr, "Couldn't open log at %s\n", GL_LOG_FILEPATH);
        return false;
    }
    va_start(argptr, msg);
    vfprintf(file, msg, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

void glfw_error_callback(int error, const char *description)
{
    log_err("GLFW ERROR %d: %s\n", error, description);
}