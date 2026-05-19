#include "ProcessingPool.hpp"

#include <algorithm>
#include <execution>
#include <numeric>

ProcessingPool::ProcessingPool(ThreadSafeQueue<std::vector<SensorData>> &queue,
                               ResultAggregator &aggregator)
    : queue_(queue), aggregator_(aggregator) {}

void ProcessingPool::start() {
  running_ = true;

  unsigned int count = std::thread::hardware_concurrency();

  for (unsigned int i = 0; i < count; ++i) {
    threads_.emplace_back(&ProcessingPool::worker, this);
  }
}

void ProcessingPool::stop() {
  running_ = false;

  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void ProcessingPool::worker() {
  while (running_) {
    auto batch = queue_.pop();

    std::sort(
        std::execution::par, batch.begin(), batch.end(),
        [](const auto &lhs, const auto &rhs) { return lhs.value < rhs.value; });

    std::vector<double> values;

    values.reserve(batch.size());

    std::vector<double> processed(batch.size());

    std::transform(std::execution::par_unseq, batch.begin(), batch.end(),
                   processed.begin(), [](const SensorData &data) -> double {
                     return data.value * 1.25;
                   });

    double result = std::transform_reduce(
        std::execution::par_unseq, values.begin(), values.end(), 0.0,
        std::plus<>(), [](double value) { return value * value; });

    std::vector<double> scan(values.size());

    std::inclusive_scan(std::execution::par, values.begin(), values.end(),
                        scan.begin());

    aggregator_.addResult(result);
  }
}