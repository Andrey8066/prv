#pragma once

#include "SensorData.hpp"
#include "ThreadSafeQueue.hpp"

#include <atomic>
#include <thread>
#include <vector>

class Dispatcher {
public:
  Dispatcher(ThreadSafeQueue<SensorData> &inputQueue,
             ThreadSafeQueue<std::vector<SensorData>> &processingQueue);

  void start();

  void stop();

private:
  void dispatchLoop();

private:
  ThreadSafeQueue<SensorData> &inputQueue_;

  ThreadSafeQueue<std::vector<SensorData>> &processingQueue_;

  std::thread thread_;

  std::atomic<bool> running_ = false;
};