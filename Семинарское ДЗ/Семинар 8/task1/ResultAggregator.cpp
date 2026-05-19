#include "ResultAggregator.hpp"

#include <numeric>

void ResultAggregator::addResult(double value) {
  std::lock_guard lock(mutex_);

  results_.push_back(value);
}

double ResultAggregator::totalResult() const {
  std::lock_guard lock(mutex_);

  return std::accumulate(results_.begin(), results_.end(), 0.0);
}