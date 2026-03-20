#include "Worker.hpp"
#include <atomic>
#include <chrono>
#include <cstddef>
#include <thread>

Worker::Worker(size_t delay) { this->delay = delay; }

void Worker::work(std::atomic<long long> &res) {
  for (size_t i = 0; i < delay; i++) {
    res.fetch_add(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}