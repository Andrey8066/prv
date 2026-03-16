#include "Logger.hpp"

#include <thread>
#include <vector>
int main() {
  Logger<std::thread::id> l;
  std::vector<std::thread> threads;
  for (int i = 0; i < 4; i++) {
    threads.emplace_back([&] {
      for (int j = 0; j < 100; j++) {
        l.log((std::this_thread::get_id()));
      }
    });
  }
  for (int i = 0; i < 4; i++) {
    threads[i].join();
  }
}