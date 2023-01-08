#ifndef _SHADER_H_
#define _SHADER_H_
#include "defines.h"
#include <string>

class Shader {
 public:
  Shader() = default;
  /**
   * init shader with shader files
   */
  Shader(const std::string &vsPath, const std::string &fsPath, const std::string &gsPath = "");
  void init(const std::string &vsPath, const std::string &fsPath, const std::string &gsPath);
  /**
   * init shader with shader codes
   */
  void initWithCode(const std::string &vs, const std::string &fs, const std::string &gs);
  /**
   * get shader code from a file
   */
  static std::string getCodeFromFile(const std::string &path);
  /**
   * use shader
   */
  void use() const;
  /**
   * get a uniform variable's location according to its name
   */
  GLint getUniformLocation(const std::string &name) const;
  /**
   * set value of uniform variables
   */
  void setInt(const std::string &name, GLint value) const;
  void setFloat(const std::string &name, GLfloat value) const;
  void setMat3(const std::string &name, const mat3 &value) const;
  void setMat4(const std::string &name, const mat4 &value) const;
  void setVec2(const std::string &name, const vec2 &value) const;
  void setVec3(const std::string &name, const vec3 &value) const;
  void setVec4(const std::string &name, const vec4 &value) const;
 private:
  GLuint id = 0;
};
#endif