#include "events/event.h"

#include <stdint.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

using namespace events;

void *EventBus::addListener(listenerFunc function, int event_id)
{
  std::shared_ptr<Listener> l = std::make_shared<Listener>();
  l->callback = function;
  l->event_id = event_id;
  listeners.push_back(l);
  return (void *)l.get();
}

bool EventBus::removeListener(void *handle)
{
  size_t i = 0;
  for (std::shared_ptr<Listener> l : listeners)
  {
    if ((void *)l.get() == handle)
    {
      listeners.erase(listeners.begin() + i); // Invalidates the iterator but we are returning anyway
      return true;
    }
    i++;
  }
  return false;
}

void EventBus::pushEvent(Event &ev)
{
  // Wait for pumpEvents to finish
  std::lock_guard<std::mutex> lock(event_mutex);

  // copy the event into the heap and push it into the queue
  events.push(ev.clone());
}

void EventBus::pumpEvents(double timeout)
{
  // Block pushEvent until we are finished
  std::lock_guard<std::mutex> lock(event_mutex);

  // printf("pumpEvents\n");
  double start = glfwGetTime();
  while (!events.empty())
  {
    std::shared_ptr<Event> ev = events.front();
    events.pop();
    log.v("event id=%04x", ev->getId());
    for (std::shared_ptr<Listener> l : listeners)
    {
      if (l->event_id == ev->getId())
      {
        l->callback(*ev);

        double elapsed = glfwGetTime() - start;
        if (elapsed >= timeout)
        {
          log.w("timeout reached (%.3f ms / %.3f ms)", elapsed*1000, timeout*1000);
          return;
        }
      }
    }
  }
}
