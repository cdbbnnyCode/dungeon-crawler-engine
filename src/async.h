#ifndef __ASYNC_H__
#define __ASYNC_H__

#include <future>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <vector>
#include <queue>

#include <stdint.h>

template <class T>
class SynchronousQueue
{
private:
  std::mutex mtx;
  std::queue<T> queue;
  std::condition_variable avail_condition;

public:
  bool empty();

  size_t size();

  T &front();

  T &back();

  void push(T &&item);

  void pop(T &item);

  void wait(size_t available, std::function<bool()> interrupt_cond);

  void notify_all();
};


template <class T>
bool SynchronousQueue<T>::empty()
{
  std::lock_guard<std::mutex> lock(mtx);
  return queue.empty();
}

template <class T>
size_t SynchronousQueue<T>::size()
{
  std::lock_guard<std::mutex> lock(mtx);
  return queue.size();
}

template <class T>
T& SynchronousQueue<T>::front()
{
  std::lock_guard<std::mutex> lock(mtx);
  return queue.front();
}

template <class T>
T& SynchronousQueue<T>::back()
{
  std::lock_guard<std::mutex> lock(mtx);
  return queue.back();
}

template <class T>
void SynchronousQueue<T>::push(T&& item)
{
  std::lock_guard<std::mutex> lock(mtx);
  queue.push(std::move(item));
  avail_condition.notify_all();
}

template <class T>
void SynchronousQueue<T>::pop(T &item)
{
  std::lock_guard<std::mutex> lock(mtx);
  item = std::move(queue.front());
  queue.pop();
}

template <class T>
void SynchronousQueue<T>::wait(size_t available, std::function<bool()> interrupt_cond)
{
  std::unique_lock<std::mutex> lock(mtx);
  if (queue.size() >= available) return; // avoid waiting if our condition is already true
  avail_condition.wait(lock, [&]() {return queue.size() >= available || interrupt_cond();});
}

template <class T>
void SynchronousQueue<T>::notify_all()
{
  avail_condition.notify_all();
}


class AsyncCounter
{
private:
  std::atomic<size_t> count;
  size_t max;
  std::mutex mutex;
  std::condition_variable condition;

public:
  AsyncCounter(size_t max) : max(max), count(0) {}
  AsyncCounter(AsyncCounter &) = delete;

  void acquire();
  // bool tryAcquire(double timeout); // TODO if necessary

  void release();

  size_t getNumAcquired();
  size_t getMaxAcquireCount();
};

class AsyncCounterGuard
{
private:
  AsyncCounter *c;
public:
  AsyncCounterGuard(AsyncCounterGuard &) = delete;

  AsyncCounterGuard(AsyncCounter &c) : c(&c)
  {
    this->c->acquire();
  }

  ~AsyncCounterGuard()
  {
    c->release();
  }
};

#endif // __ASYNC_H__
