#include <thread>
#include <chrono>

#include <stdio.h>

struct Thing
{
  int a;
  int b;
  int c;
  char *data = nullptr;
};

void runThread()
{
  Thing thing = {1, 2, 3};
  thing.data = new char[6] {'h', 'e', 'l', 'l', 'o', '\0'};
  std::thread(
    [&, thing] () {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      printf("a=%d, b=%d, c=%d, data=%s\n", thing.a, thing.b, thing.c, thing.data);
      thing.data[0] = 'b';
    }
  ).detach();
}

int main()
{
  printf("threading test\n");

  runThread();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  runThread();
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  return 0;
}
