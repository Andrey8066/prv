#include "OrderProcessing.hpp"
#include <cstddef>
#include <memory>
#include <random>
#include <thread>

OrderProcessing::OrderProcessing(size_t size, size_t numberOfProcessers,
                                 size_t numberOfDelievers)
    : processersSemaphore(numberOfProcessers),
      delieversSemaphore(numberOfDelievers) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<size_t> dist(1, 100);
  for (size_t i = 0; i < size; i++) {
    orders.emplace_back(std::make_shared<Order>(dist(gen), dist(gen), i));
  }
}

void OrderProcessing::process() {
  for (size_t i = 0; i < orders.size(); i++) {
    threads.emplace_back(std::thread(&OrderProcessing::processOrder, this, i));
  }
  for (size_t i = 0; i < orders.size(); i++) {
    threads.emplace_back(std::thread(&OrderProcessing::deliverOrder, this, i));
  }
  for (auto &t : threads) {
    t.join();
  }
  print();
}

void OrderProcessing::processOrder(size_t i) {
  processersSemaphore.acquire();
  std::shared_ptr<Order> order = orders.at(i);
  order->processOrder();
  processersSemaphore.release();
}

void OrderProcessing::deliverOrder(size_t i) {

  orders.at(i)->readyForDeliver.acquire();
  delieversSemaphore.acquire();

  orders.at(i)->deliverOrder();
  delieversSemaphore.release();
}

void OrderProcessing::print() {
  for (auto &p : orders) {
    p->print();
  }
}