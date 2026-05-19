#include "Dispatcher.hpp"

Dispatcher::Dispatcher(
    ThreadSafeQueue<SensorData> &inputQueue,
    ThreadSafeQueue<std::vector<SensorData>> &processingQueue)
    : inputQueue_(inputQueue), processingQueue_(processingQueue) {}

void Dispatcher::start() {
  running_ = true;

  thread_ = std::thread(&Dispatcher::dispatchLoop, this);
}

void Dispatcher::stop() {
  running_ = false;

  if (thread_.joinable()) {
    thread_.join();
  }
}

void Dispatcher::dispatchLoop() {
  while (running_) {
    std::vector<SensorData> batch;

    for (int i = 0; i < 100; ++i) {
      batch.push_back(inputQueue_.pop());
    }

    processingQueue_.push(batch);
  }
}