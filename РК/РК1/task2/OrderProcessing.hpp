#include "Order.hpp"
#include <cstddef>
#include <memory>
#include <semaphore>
#include <thread>
#include <vector>

#pragma once
class OrderProcessing {
private:
  std::vector<std::shared_ptr<Order>> orders;
  std::vector<std::shared_ptr<Order>> ordersForDelivery;
  std::counting_semaphore<> delieversSemaphore;
  std::counting_semaphore<> processersSemaphore;
  std::vector<std::thread> threads;

public:
  OrderProcessing(size_t size, size_t numberOfProcessers,
                  size_t numberOfDelievers);
  void processOrder(size_t i);
  void deliverOrder(size_t i);
  void process();
  void print();
};