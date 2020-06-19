#ifndef __GLHELPER_H__
#define __GLHELPER_H__

#include <string>

#include "resources/resource.h"

namespace gfx
{

#define ERR_SHADER_VERT_COMPILE -10
#define ERR_SHADER_FRAG_COMPILE -11
#define ERR_SHADER_LINK         -12

class Shader
{
private:
  unsigned int vertexShader;
  unsigned int fragmentShader;
  unsigned int shaderProgram;
  int compiled = 0;

  Logger log = Logger("Shader");
public:
  Shader() {}
  ~Shader();
  Shader(Shader&) = delete; // no copy for you

  int compile(std::string vsh, std::string fsh);

  unsigned int getShaderId();
};



} // namespace gfx

#endif // __GLHELPER_H__
