#pragma once

#include "ResultAggregator.hpp"

#include <atomic>
#include <thread>

class Storage {
public:
  explicit Storage(ResultAggregator &aggregator);

  void start();

  void stop();

private:
  void storageLoop();

private:
  ResultAggregator &aggregator_;

  std::thread thread_;

  std::atomic<bool> running_ = false;
};