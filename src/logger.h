#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <stdio.h>
#include <chrono>
#include <memory>
#include <mutex>

#define LOG_LEVEL_VERBOSE 0
#define LOG_LEVEL_DEBUG   1
#define LOG_LEVEL_INFO    2
#define LOG_LEVEL_WARN    3
#define LOG_LEVEL_ERROR   4
#define LOG_LEVEL_FATAL   5

#define LOG_LEVEL_ALL     0
#define LOG_LEVEL_NONE    6

const std::string log_levels[6] = {
  "VERBOSE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};


class LogSystem
{
private:
  LogSystem() {}

  static LogSystem instance;
public:
  std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
  std::mutex logMtx;
  static LogSystem *getInstance();
};

class Logger
{
private:
  std::string tag;
public:
  Logger(std::string tag) : tag(tag) {}

  template<class... Args>
  void log(int level, std::string message, Args... args);

  template<class... Args>
  inline void v(std::string message, Args... args);

  template<class... Args>
  inline void d(std::string message, Args... args);

  template<class... Args>
  inline void i(std::string message, Args... args);

  template<class... Args>
  inline void w(std::string message, Args... args);

  template<class... Args>
  inline void e(std::string message, Args... args);

  template<class... Args>
  inline void f(std::string message, Args... args);
};

template<class... Args>
void Logger::log(int level, std::string message, Args... args)
{
  if (level < LOG_LEVEL_ALL) level = LOG_LEVEL_ALL;
  else if (level >= LOG_LEVEL_NONE) level = LOG_LEVEL_NONE-1;


  std::chrono::time_point t_now = std::chrono::steady_clock::now();
  std::chrono::duration duration =
      std::chrono::duration_cast<std::chrono::microseconds>(t_now - LogSystem::getInstance()->start_time);

  double seconds = duration.count() / 1000000.0;
  std::string levelname = log_levels[level];
  std::string msg_str = "[%9.3fs] [%s/%s] " + message + "\n";

  // prevent overwriting when using multiple threads
  std::lock_guard<std::mutex> lock(LogSystem::getInstance()->logMtx);
  printf(msg_str.c_str(), seconds, tag.c_str(), levelname.c_str(), std::forward<Args>(args)...);
}

template<class... Args>
inline void Logger::v(std::string message, Args... args)
{ log(LOG_LEVEL_VERBOSE, message, std::forward<Args>(args)...); }

template<class... Args>
inline void Logger::d(std::string message, Args... args)
{ log(LOG_LEVEL_DEBUG, message, std::forward<Args>(args)...); }

template<class... Args>
inline void Logger::i(std::string message, Args... args)
{ log(LOG_LEVEL_INFO, message, std::forward<Args>(args)...); }

template<class... Args>
inline void Logger::w(std::string message, Args... args)
{ log(LOG_LEVEL_WARN, message, std::forward<Args>(args)...); }

template<class... Args>
inline void Logger::e(std::string message, Args... args)
{ log(LOG_LEVEL_ERROR, message, std::forward<Args>(args)...); }

template<class... Args>
inline void Logger::f(std::string message, Args... args)
{ log(LOG_LEVEL_FATAL, message, std::forward<Args>(args)...); }

#endif // __LOGGER_H__
