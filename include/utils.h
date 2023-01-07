#ifndef _UTILS_H_
#define _UTILS_H_
#include "defines.h"
#include <string>
/**
 * try and search target file in parent directories with max depth limit
 */
std::string getPath(const std::string &target, int depth = 5);

struct WindowGuard final {
  WindowGuard(GLFWwindow *&, const int width, const int height, const std::string &title);
  ~WindowGuard();
};
#endif