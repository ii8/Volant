#ifndef LOG_H
#define LOG_H

#include "vmath.h"

int _vlog_glerr(char*, int);

#define vlog_glerr() _vlog_glerr(__FILE__, __LINE__)

void vlog_start();

void vlog(const char*, ...);

void vlog_err(const char*, ...);

void vlog_mat(mat4);

void glfw_error_callback (int, const char*);

void vlog_params();

#endif
