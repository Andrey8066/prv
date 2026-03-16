#include "Cache.hpp"
#include <random>
#include <thread>
#include <vector>
int main() {
  Cache<int, long> cache;
  std::vector<std::thread> threads;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 100);

  for (int i = 0; i < 8; i++) {
    threads.emplace_back(std::thread([&] {
      for (int j = 0; j < 50; j++) {
        cache.set(dist(gen), dist(gen));
      }
      std::this_thread::yield();
    }));
  }
  for (int i = 0; i < 8; i++) {
    threads.emplace_back(std::thread([&] {
      for (int j = 0; j < 50; j++) {
        cache.get(dist(gen));
      }
      std::this_thread::yield();
    }));
  }
  for (int i = 0; i < 16; i++) {
    threads[i].join();
  }
}