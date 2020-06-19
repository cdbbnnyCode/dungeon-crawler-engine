#ifndef __GFX_H__
#define __GFX_H__

#include <string>
#include <memory>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "events/event.h"
#include "resources/resource.h"
#include "logger.h"

namespace gfx
{

class Window
{
private:
  GLFWwindow* window = nullptr;
  // static std::unique_ptr<Window> instance;
  static Window instance;

  events::EventBus eventBus;
  resources::ResourceLoader resourceLoader;

  Logger log = Logger("Window");

  Window();
public:
  ~Window();

  static Window *getInstance();

  bool create(unsigned int width, unsigned int height, std::string name);
  bool shouldClose();

  void update();

  GLFWwindow *getWindow();
  events::EventBus &getEventBus();
  resources::ResourceLoader &getResourceLoader();

  void close();
};

inline Window *getWindow()
{
  return Window::getInstance();
}

}
#endif // __GFX_H__
