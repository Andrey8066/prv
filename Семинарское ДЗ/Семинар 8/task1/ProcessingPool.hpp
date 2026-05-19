#pragma once

#include "ResultAggregator.hpp"
#include "SensorData.hpp"
#include "ThreadSafeQueue.hpp"

#include <atomic>
#include <thread>
#include <vector>

class ProcessingPool {
public:
  ProcessingPool(ThreadSafeQueue<std::vector<SensorData>> &queue,
                 ResultAggregator &aggregator);

  void start();

  void stop();

private:
  void worker();

private:
  ThreadSafeQueue<std::vector<SensorData>> &queue_;

  ResultAggregator &aggregator_;

  std::vector<std::thread> threads_;

  std::atomic<bool> running_ = false;
};