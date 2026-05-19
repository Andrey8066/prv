#include "SensorSimulator.hpp"

#include <chrono>
#include <random>

SensorSimulator::SensorSimulator(int sensorCount,
                                 ThreadSafeQueue<SensorData> &queue)
    : sensorCount_(sensorCount), queue_(queue) {}

void SensorSimulator::start() {
  running_ = true;

  for (int i = 0; i < sensorCount_; ++i) {
    threads_.emplace_back(&SensorSimulator::sensorWorker, this, i);
  }
}

void SensorSimulator::stop() {
  running_ = false;

  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void SensorSimulator::sensorWorker(int sensorId) {
  std::mt19937 generator(std::random_device{}());

  std::uniform_real_distribution<> distribution(0.0, 1000.0);

  while (running_) {
    SensorData data;

    data.sensorId = sensorId;
    data.value = distribution(generator);
    data.timestamp = std::chrono::steady_clock::now();

    queue_.push(data);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}