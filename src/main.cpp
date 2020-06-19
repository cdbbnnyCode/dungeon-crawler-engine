#include <unistd.h>
#include <stdio.h>
#include <string>

#include "graphics/window.h"
#include "events/event.h"
#include "events/window_event.h"
#include "resources/resource.h"
#include "logger.h"

int main(void)
{
  Logger log("Main");
  gfx::Window *win = gfx::getWindow();
  if (!win->create(1280, 720, "Test"))
  {
    log.f("Error creating window!\n");
    return 1;
  }
  events::set_glfw_window_callbacks();

  win->getEventBus().addListener([] (events::Event &e)
  {
    Logger log("File event");
    resources::ResourceEvent *ev = dynamic_cast<resources::ResourceEvent*>(&e);
    log.d("File load: %s", ev->filePath.c_str());
    log.d("err=%d", ev->err);
    log.d("blob size=%d", ev->blob.getSize());

  }, EV_RESOURCE_LOAD);

  bool shouldClose = false;
  win->getEventBus().addListener([&] (events::Event &e)
  {
    shouldClose = true;
  }, EV_WIN_CLOSE);

  while (!shouldClose)
  {
    usleep(15000);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    win->update();
  }
  return 0;
}
