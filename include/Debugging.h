//
// Created by m4tex on 4/17/23.
//

#pragma once
#include <csignal>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);

void glClearError();
bool glLogCall(const char* function, const char* file, int line);