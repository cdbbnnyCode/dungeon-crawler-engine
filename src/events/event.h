#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <vector>
#include <queue>
#include <functional>
#include <memory>
#include <mutex>

#include "logger.h"

namespace events
{

class Event
{
public:
  virtual int getId() {return 0;};
  virtual ~Event() {}
  virtual std::shared_ptr<Event> clone() const
  {
    return std::make_shared<Event>(*this);
  }
};

typedef std::function<void(Event&)> listenerFunc;

struct Listener
{
  listenerFunc callback;
  int event_id;
};

class EventBus
{
private:
  std::queue<std::shared_ptr<Event>> events;
  std::vector<std::shared_ptr<Listener>> listeners;
  // Mutex to prevent events from being pushed while pumpEvents is running
  std::mutex event_mutex;

  Logger log = Logger("EventBus");
public:
  void pushEvent(Event &ev);
  void pumpEvents(double timeout);

  void *addListener(listenerFunc function, int event_id);
  bool removeListener(void *handle);
};


}

#endif // __EVENT_H__
