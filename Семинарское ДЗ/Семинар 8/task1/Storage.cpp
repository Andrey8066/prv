#include "Storage.hpp"

#include <chrono>
#include <iostream>

Storage::Storage(ResultAggregator &aggregator) : aggregator_(aggregator) {}

void Storage::start() {
  running_ = true;

  thread_ = std::thread(&Storage::storageLoop, this);
}

void Storage::stop() {
  running_ = false;

  if (thread_.joinable()) {
    thread_.join();
  }
}

void Storage::storageLoop() {
  while (running_) {
    std::cout << "Aggregated result: " << aggregator_.totalResult() << '\n';

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}