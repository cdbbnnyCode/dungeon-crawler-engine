cmake_minimum_required(VERSION 3.2)
project("dungeon_crawler_engine")
include_directories("src/" "include/")

# Executable
add_executable(dcengine
  # sources here
  src/main.cpp
  src/graphics/window.cpp
  src/graphics/glad.c
  src/events/event.cpp
  src/events/window_event.cpp
  src/resources/resource.cpp
  src/async.cpp
  src/logger.cpp
  src/graphics/glhelper.cpp
)
set_property(TARGET dcengine PROPERTY CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Boost 1.48 REQUIRED COMPONENTS filesystem)
include_directories( ${OPENGL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )

target_link_libraries(dcengine ${OPENGL_LIBRARIES} glfw dl ${Boost_LIBRARIES}
  Threads::Threads )
