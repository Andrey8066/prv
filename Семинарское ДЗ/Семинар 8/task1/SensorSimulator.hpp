#pragma once

#include "SensorData.hpp"
#include "ThreadSafeQueue.hpp"

#include <atomic>
#include <thread>
#include <vector>

class SensorSimulator {
public:
  SensorSimulator(int sensorCount, ThreadSafeQueue<SensorData> &queue);

  void start();

  void stop();

private:
  void sensorWorker(int sensorId);

private:
  int sensorCount_;

  ThreadSafeQueue<SensorData> &queue_;

  std::vector<std::thread> threads_;

  std::atomic<bool> running_ = false;
};