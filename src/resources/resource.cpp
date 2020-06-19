#include "resources/resource.h"
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <memory>

#include "graphics/window.h"

using namespace resources;

static int allocateBlob(Blob &blob, Logger &log)
{
  std::string &path = blob.path;
  boost::filesystem::path p(path);
  if (!boost::filesystem::is_regular_file(p))
  {
    log.e("File '%s' does not exist (or is not a regular file)!", path.c_str());
    return ERR_FILE_NOT_FOUND;
  }

  size_t blobSize = boost::filesystem::file_size(p);
  if (blobSize > MAX_BLOB_SIZE)
  {
    log.e("Blob too big (%d bytes)!", blobSize);
    return ERR_FILE_TOO_BIG;
  }

  log.d("Allocating %d bytes", blobSize);
  int err = blob.alloc(blobSize);
  if (err)
  {
    log.e("Blob alloc error: ");
    if (err == ERR_BLOB_ALREADY_ALLOCATED) log.e("  already allocated");
    else if (err == ERR_BLOB_MALLOC_ERROR) log.e("  malloc() error (out of memory)");
    return err;
  }
  return blobSize;
}

ResourceLoader::ResourceLoader()
{
  workerFinished = false;
  evBus = &(gfx::getWindow()->getEventBus());
  worker = std::thread([this] ()
  {
    Logger log("Resource Loader Worker");
    while (!workerFinished)
    {
      blobQueue.wait(1, [this]() -> bool {return workerFinished;});
      if (blobQueue.empty()) break;
      Blob b;
      blobQueue.pop(b);
      log.d("loading %p (%s) -> %p", &b, b.path.c_str(), b.getData());

      // build a ResourceEvent
      ResourceEvent ev;
      ev.filePath = b.path;

      std::ifstream f(b.path);
      if (!f)
      {
        b.dealloc();
        ev.err = ERR_FILE_OPEN;
        evBus->pushEvent(ev);
        continue;
      }

      log.d("reading %d bytes", b.getSize());
      f.read((char *)b.getData(), b.getSize());

      if (!f.good())
      {
        b.dealloc();
        ev.err = ERR_FILE_READ;
        evBus->pushEvent(ev);
        continue;
      }

      ev.err = 0;
      ev.blob = std::move(b);
      evBus->pushEvent(ev);
    }
    log.i("worker finished");
  });
}

ResourceLoader::~ResourceLoader()
{
  workerFinished = true;
  blobQueue.notify_all();
  worker.join();
}

int ResourceLoader::loadFile(std::string path, Blob &blob)
{
  int err = allocateBlob(blob, log);
  if (err < 0) return err;
  size_t blobSize = (size_t)err;

  std::ifstream f(path);
  if (!f)
  {
    printf("File open error\n");
    blob.dealloc();
    return ERR_FILE_OPEN;
  }
  f.read((char *)blob.getData(), blobSize);
  if (!f.good())
  {
    printf("File read error\n");
    blob.dealloc();
    return ERR_FILE_READ;
  }
  return 0;
}

// static int _counter = 0;

int ResourceLoader::loadFileAsync(std::string path)
{
  // push a blob into storage
  Blob blob(path);

  ResourceEvent ev;
  ev.filePath = path;
  int err = allocateBlob(blob, log);
  if (err < 0)
  {
    ev.err = err;
    evBus->pushEvent(ev);
    return err;
  }

  size_t blobSize = (size_t)err;


  blobQueue.push(std::move(blob));
  // this blob is now invalid because we moved it into the queue

  return 0;
}
