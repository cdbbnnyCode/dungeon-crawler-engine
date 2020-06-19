#include "async.h"
#include <stdio.h>
#include <chrono>
#include <algorithm>

/*
  async.cpp -- Multithreading utilities
*/

/////////
// AsyncCounter

void AsyncCounter::acquire()
{
  std::unique_lock<std::mutex> lock(mutex);
  condition.wait(lock, [&]() { return count < max; });
  count++;
  lock.unlock();
  condition.notify_all();
}

void AsyncCounter::release()
{
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (count == 0)
    {
      printf("AsyncCounter error: underflow\n");
      return;
    }
    count--;
  }
  condition.notify_all();
}

size_t AsyncCounter::getNumAcquired()
{
  return count;
}

size_t AsyncCounter::getMaxAcquireCount()
{
  return max;
}
