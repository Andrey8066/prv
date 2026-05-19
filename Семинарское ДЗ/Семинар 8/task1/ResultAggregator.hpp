#pragma once

#include <mutex>
#include <vector>

class ResultAggregator {
public:
  void addResult(double value);

  double totalResult() const;

private:
  mutable std::mutex mutex_;

  std::vector<double> results_;
};