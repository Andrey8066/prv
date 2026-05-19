#pragma once

#include <chrono>

struct SensorData {
  int sensorId;
  double value;
  std::chrono::steady_clock::time_point timestamp;
};