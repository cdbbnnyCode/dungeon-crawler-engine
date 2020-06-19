#include "logger.h"

LogSystem LogSystem::instance;

LogSystem *LogSystem::getInstance()
{
  return &LogSystem::instance;
}
