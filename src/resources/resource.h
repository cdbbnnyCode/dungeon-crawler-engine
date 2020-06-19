#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <stdint.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
// #include <future>
#include <vector>
#include <unordered_map>

#include "events/event.h"
#include "async.h"
#include "logger.h"

namespace resources
{

// Individual file size should not exceed 1 GiB
#define MAX_BLOB_SIZE 1073741824

#define ERR_BLOB_ALREADY_ALLOCATED -1
#define ERR_BLOB_MALLOC_ERROR      -2
#define ERR_BLOB_NOT_ALLOCATED     -3
#define ERR_FILE_NOT_FOUND         -4
#define ERR_FILE_TOO_BIG           -5
#define ERR_FILE_OPEN              -6
#define ERR_FILE_READ              -7

#define EV_RESOURCE_LOAD       0x2000

class Blob
{
private:
  bool valid = false;
  size_t size = 0;
  std::shared_ptr<char> data;
public:
  std::string path;

  Blob(std::string path="") : path(path) { }


  bool isValid()
  {
    return valid;
  }

  size_t getSize()
  {
    return size;
  }

  void *getData()
  {
    return data.get();
  }

  int alloc(size_t size)
  {
    if (valid) return ERR_BLOB_ALREADY_ALLOCATED;
    data = std::shared_ptr<char>(new char[size]);
    if (!data) return ERR_BLOB_MALLOC_ERROR;
    valid = true;
    this->size = size;
    return 0;
  }

  int dealloc()
  {
    if (!valid) return ERR_BLOB_NOT_ALLOCATED;
    data = std::shared_ptr<char>();
    valid = false;
    size = 0;
    return 0;
  }
};

struct ResourceEvent : public events::Event
{
  int getId() override {return EV_RESOURCE_LOAD;}
  Blob blob;
  std::string filePath;
  int err;

  std::shared_ptr<Event> clone() const override
  {
    return std::make_shared<ResourceEvent>(*this);
  }
};

class ResourceLoader
{
private:
  events::EventBus *evBus;

  SynchronousQueue<Blob> blobQueue;
  std::condition_variable ready_condition;
  std::thread worker;
  std::atomic<bool> workerFinished;

  Logger log = Logger("Resource Loader");
public:
  ResourceLoader();
  ~ResourceLoader();
  int loadFile(std::string path, Blob &blob);
  int loadFileAsync(std::string path);
};

} // namespace resources

#endif // __RESOURCE_H__
