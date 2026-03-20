#include "ProductionLine.hpp"
#include "Worker.hpp"

#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

ProductionLine::ProductionLine(size_t delay) { this->delay = delay; }

void ProductionLine::asemble() {
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "Начата сборка, поток " << std::this_thread::get_id()
            << std::endl;
  std::vector<std::thread> workerThreads;
  for (size_t i = 0; i < workerPerProcces; i++) {
    Worker w(delay);
    workerThreads.emplace_back(std::thread(&Worker::work, w, std::ref(res)));
  }

  for (auto &t : workerThreads) {
    t.join();
  }
  stages[0] = true;
  std::cout << "Закончена сборка, поток " << std::this_thread::get_id()
            << " , результат " << res.load() << std::endl;
  cv.notify_all();
}

void ProductionLine::painting() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&] { return stages[0]; });
  std::cout << "Начата покраска, поток " << std::this_thread::get_id()
            << std::endl;
  std::vector<std::thread> workerThreads;
  for (size_t i = 0; i < workerPerProcces; i++) {
    Worker w(delay);
    workerThreads.emplace_back(std::thread(&Worker::work, w, std::ref(res)));
  }

  for (auto &t : workerThreads) {
    t.join();
  }
  stages[1] = true;
  std::cout << "Закончена покраска, поток " << std::this_thread::get_id()
            << " , результат " << res.load() << std::endl;
  cv.notify_all();
}

void ProductionLine::packaging() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&] { return stages[1]; });
  std::cout << "Начата упаковка, поток " << std::this_thread::get_id()
            << std::endl;
  std::vector<std::thread> workerThreads;
  for (size_t i = 0; i < workerPerProcces; i++) {
    Worker w(delay);
    workerThreads.emplace_back(std::thread(&Worker::work, w, std::ref(res)));
  }

  for (auto &t : workerThreads) {
    t.join();
  }
  stages[2] = true;
  std::cout << "Закончена упаковка, поток " << std::this_thread::get_id()
            << " , результат " << res.load() << std::endl;
  cv.notify_all();
}

void ProductionLine::product() {
  std::cout << "Начата работа" << std::endl;
  threads.emplace_back(std::thread(&ProductionLine::asemble, this));
  threads.emplace_back(std::thread(&ProductionLine::painting, this));
  threads.emplace_back(std::thread(&ProductionLine::packaging, this));

  for (auto &t : threads) {
    t.join();
  }
}