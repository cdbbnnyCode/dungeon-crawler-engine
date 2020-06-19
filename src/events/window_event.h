#ifndef __WINDOW_EVENT_H__
#define __WINDOW_EVENT_H__

#include "events/event.h"
#include <memory>

namespace events
{

#define EV_WIN_POS           0x1001
#define EV_WIN_SIZE          0x1002
#define EV_WIN_CLOSE         0x1003
#define EV_WIN_REFRESH       0x1004
#define EV_WIN_FOCUS         0x1005
#define EV_WIN_ICONIFY       0x1006
#define EV_WIN_MAXIMIZE      0x1007
#define EV_WIN_FB_SIZE       0x1008
#define EV_WIN_CONTENT_SCALE 0x1009

struct WindowPosEvent : public Event
{
  int getId() override {return EV_WIN_POS;};
  int x;
  int y;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowPosEvent>(*this);
  }
};

struct WindowSizeEvent : public Event
{
  int getId() override {return EV_WIN_SIZE;};
  int w;
  int h;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowSizeEvent>(*this);
  }
};

struct WindowCloseEvent : public Event
{
  int getId() override {return EV_WIN_CLOSE;};
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowCloseEvent>(*this);
  }
};

struct WindowRefreshEvent : public Event
{
  int getId() override {return EV_WIN_REFRESH;};
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowRefreshEvent>(*this);
  }
};

struct WindowFocusEvent : public Event
{
  int getId() override {return EV_WIN_FOCUS;};
  int focused;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowFocusEvent>(*this);
  }
};

struct WindowIconifyEvent : public Event
{
  int getId() override {return EV_WIN_ICONIFY;};
  int iconified;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowIconifyEvent>(*this);
  }
};

struct WindowMaximizeEvent : public Event
{
  int getId() override {return EV_WIN_MAXIMIZE;};
  int maximized;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowMaximizeEvent>(*this);
  }
};

struct WindowFramebufSizeEvent : public Event
{
  int getId() override {return EV_WIN_FB_SIZE;};
  int w;
  int h;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowFramebufSizeEvent>(*this);
  }
};

struct WindowContentScaleEvent : public Event
{
  int getId() override {return EV_WIN_CONTENT_SCALE;};
  float scaleX;
  float scaleY;
  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<WindowContentScaleEvent>(*this);
  }
};

void set_glfw_window_callbacks();

} // namespace event

#endif // __WINDOW_EVENT_H__
