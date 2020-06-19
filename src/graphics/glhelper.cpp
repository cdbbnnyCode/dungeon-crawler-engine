#include "graphics/glhelper.h"

#include "graphics/window.h"

using namespace gfx;

/////////////
// Shader

static int checkCompile(int shaderid, std::string name, Logger &log)
{
  int succ;
  char infoLog[512];
  glGetShaderiv(shaderid, GL_COMPILE_STATUS, &succ);
  if (!succ)
  {
    glGetShaderInfoLog(shaderid, 512, NULL, infoLog);
    log.e("Shader compilation for %s failed:", name.c_str());
    log.e("%s", infoLog);
  }
  return succ;
}

int Shader::compile(std::string vsh, std::string fsh)
{
  log.d("Compiling vertex shader %s", vsh.c_str());
  resources::ResourceLoader &loader = getWindow()->getResourceLoader();
  resources::Blob vshSrc;
  int err = loader.loadFile(vsh, vshSrc);
  if (err) return err;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (char * const*)vshSrc.getData(), NULL);
  glCompileShader(vertexShader);

  if (!checkCompile(vertexShader, vsh, log))
  {
    return ERR_SHADER_VERT_COMPILE;
  }
  compiled = 1;

  log.d("Compiling fragment shader %s", fsh.c_str());
  resources::Blob fshSrc;
  err = loader.loadFile(fsh, fshSrc);
  if (err) return err;

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (char * const*)fshSrc.getData(), NULL);
  if (!checkCompile(fragmentShader, fsh, log))
  {
    return ERR_SHADER_FRAG_COMPILE;
  }
  compiled = 2;

  log.d("Linking shaders");
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for errors here
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    log.e("Shader linking for %s, %s failed:", vsh.c_str(), fsh.c_str());
    log.e("%s", infoLog);
    return ERR_SHADER_LINK;
  }
  compiled = 3;

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return 0;
}

Shader::~Shader()
{
  // clean up shaders
  if (compiled == 1 || compiled == 2) glDeleteShader(vertexShader);
  if (compiled == 2) glDeleteShader(fragmentShader);
  if (compiled == 3) glDeleteProgram(shaderProgram);
}

unsigned int Shader::getShaderId()
{
  return shaderProgram;
}
