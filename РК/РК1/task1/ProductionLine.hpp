

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <thread>
#include <vector>

#pragma once

class ProductionLine {
private:
  size_t delay;
  size_t workerPerProcces = 10;
  std::atomic<long long> res = 0;
  std::mutex mtx;
  std::condition_variable cv;
  std::vector<std::thread> threads;
  std::vector<bool> stages = {false, false, false};

public:
  ProductionLine(size_t delay);
  void product();
  void asemble();
  void painting();
  void packaging();
};