#include "Dispatcher.hpp"
#include "ProcessingPool.hpp"
#include "ResultAggregator.hpp"
#include "SensorSimulator.hpp"
#include "Storage.hpp"

#include <chrono>
#include <thread>

int main() {
  ThreadSafeQueue<SensorData> inputQueue;

  ThreadSafeQueue<std::vector<SensorData>> processingQueue;

  ResultAggregator aggregator;

  SensorSimulator simulator(100, inputQueue);

  Dispatcher dispatcher(inputQueue, processingQueue);

  ProcessingPool processingPool(processingQueue, aggregator);

  Storage storage(aggregator);

  simulator.start();

  dispatcher.start();

  processingPool.start();

  storage.start();

  std::this_thread::sleep_for(std::chrono::seconds(10));

  simulator.stop();

  dispatcher.stop();

  processingPool.stop();

  storage.stop();

  return 0;
}