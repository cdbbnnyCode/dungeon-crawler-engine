#include "events/window_event.h"
#include "graphics/window.h"
#include <GLFW/glfw3.h>

using namespace events;

////////////////////////////////////////////////////////////////////////////////
// Static window event handlers
// These get forwarded to the Window object

/*
const int WindowPosEvent::event_id          = EV_WIN_POS;
const int WindowSizeEvent::event_id         = EV_WIN_SIZE;
const int WindowCloseEvent::event_id        = EV_WIN_CLOSE;
const int WindowRefreshEvent::event_id      = EV_WIN_REFRESH;
const int WindowFocusEvent::event_id        = EV_WIN_FOCUS;
const int WindowIconifyEvent::event_id      = EV_WIN_ICONIFY;
const int WindowMaximizeEvent::event_id     = EV_WIN_MAXIMIZE;
const int WindowFramebufSizeEvent::event_id = EV_WIN_FB_SIZE;
const int WindowContentScaleEvent::event_id = EV_WIN_CONTENT_SCALE;
*/

static void onPos(GLFWwindow *win, int x, int y)
{
  WindowPosEvent ev;
  ev.x = x;
  ev.y = y;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onSize(GLFWwindow *win, int w, int h)
{
  WindowSizeEvent ev;
  ev.w = w;
  ev.h = h;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onClose(GLFWwindow *win)
{
  WindowCloseEvent ev;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onRefresh(GLFWwindow *win)
{
  WindowRefreshEvent ev;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onFocus(GLFWwindow *win, int focused)
{
  WindowFocusEvent ev;
  ev.focused = focused;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onIconify(GLFWwindow *win, int iconified)
{
  WindowIconifyEvent ev;
  ev.iconified = iconified;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onMaximize(GLFWwindow *win, int maximized)
{
  WindowMaximizeEvent ev;
  ev.maximized = maximized;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onFBSize(GLFWwindow *win, int w, int h)
{
  WindowFramebufSizeEvent ev;
  ev.w = w;
  ev.h = h;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

static void onContentScale(GLFWwindow *win, float xs, float ys)
{
  WindowContentScaleEvent ev;
  ev.scaleX = xs;
  ev.scaleY = ys;
  gfx::getWindow()->getEventBus().pushEvent(ev);
}

void events::set_glfw_window_callbacks()
{
  GLFWwindow *win = gfx::getWindow()->getWindow();
  glfwSetWindowPosCallback(win, onPos);
  glfwSetWindowSizeCallback(win, onSize);
  glfwSetWindowCloseCallback(win, onClose);
  glfwSetWindowFocusCallback(win, onFocus);
  glfwSetWindowIconifyCallback(win, onIconify);
  glfwSetWindowMaximizeCallback(win, onMaximize);
  glfwSetFramebufferSizeCallback(win, onFBSize);
  glfwSetWindowContentScaleCallback(win, onContentScale);
}
