#pragma once

#include <cstdio>

#ifdef DEBUG
#define TRACE(format, ...) std::fprintf(stderr, "[TRACE] " format "\n", ##__VA_ARGS__)
#define INFO(format, ...) std::fprintf(stderr, "[INFO] " format "\n", ##__VA_ARGS__)
#endif

#ifndef DEBUG
#define TRACE(format, ...)
#define INFO(format, ...)
#endif

#define WARN(format, ...) std::fprintf(stderr, "[WARN] " format "\n", ##__VA_ARGS__)
#define ERROR(format, ...) std::fprintf(stderr, "[ERROR] " format "\n", ##__VA_ARGS__)
