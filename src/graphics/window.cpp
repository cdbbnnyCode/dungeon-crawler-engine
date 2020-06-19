#include "graphics/window.h"
#include <stdio.h>
#include <stdlib.h>

#include "events/window_event.h"

static bool glfw_is_initialized = false;

////////////////////////////////////////////////////////////////////////////////
// Window object
gfx::Window gfx::Window::instance;

gfx::Window::Window()
{
  if (!glfw_is_initialized)
  {
    if (!glfwInit())
    {
      // cause fatal error here
      log.f("glfwInit failed");
    }
    else
    {
      glfw_is_initialized = true;
    }
  }
}

gfx::Window::~Window()
{
  if (window)
  {
    log.i("destroying window");
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
  }
}

bool gfx::Window::create(unsigned int width, unsigned int height, std::string name)
{
  // Should not have a window here
  if (window) return false;

  // set GL version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
  // Should have a window here
  if (!window) return false;

  // Initialize GL
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log.e("glad init failed");
    return false;
  }

  // Set framebuffer size and register resize handler
  glViewport(0, 0, width, height);

  eventBus.addListener([this](events::Event &e) {
    events::WindowFramebufSizeEvent *ev
      = dynamic_cast<events::WindowFramebufSizeEvent*>(&e);
    if (!ev)
    {
      log.f("event error: ev is not a WindowFramebufSizeEvent (but id=%04x)",
              e.getId());
      abort();
    }
    log.v("update viewport w=%d h=%d", ev->w, ev->h);
    glViewport(0, 0, ev->w, ev->h);
  }, EV_WIN_FB_SIZE);

  return true;
}

void gfx::Window::update()
{
  glfwSwapBuffers(window);
  glfwPollEvents();
  eventBus.pumpEvents(0.005);
}

bool gfx::Window::shouldClose()
{
  return glfwWindowShouldClose(window);
}

GLFWwindow *gfx::Window::getWindow()
{
  return window;
}

events::EventBus &gfx::Window::getEventBus()
{
  return eventBus;
}

resources::ResourceLoader &gfx::Window::getResourceLoader()
{
  return resourceLoader;
}

void gfx::Window::close()
{
  glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// std::unique_ptr<gfx::Window> gfx::Window::instance = std::unique_ptr<gfx::Window>();

gfx::Window *gfx::Window::getInstance()
{
  return &instance;
}
