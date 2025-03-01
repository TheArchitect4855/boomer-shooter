#ifdef DEBUG
#include <cstdlib>
#include <cstdio>
#define ASSERT(condition) do { if (!(condition)) { std::fprintf(stderr, __FILE__ ":%d: ASSERTION FAILED: " #condition "\n", __LINE__); std::exit(1); } } while (0)
#define UNREACHABLE do { std::fprintf(stderr, __FILE__ ":%d: Unreachable.\n", __LINE__); std::exit(1); } while (0)
#endif

#ifndef DEBUG
#define ASSERT(condition)
#define UNREACUNREACHABLE
#endif
