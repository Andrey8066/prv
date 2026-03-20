#include "Order.hpp"
#include <iostream>
#include <thread>

Order::Order(size_t pd, size_t dd, size_t i)
    : processingDelay(pd), delieveryDelay(dd), id(i), readyForDeliver(0) {}
void Order::processOrder() {
  std::cout << "Обработка заказа " << this->id << " начата\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(processingDelay));
  std::cout << "Обработка заказа " << this->id << " закончена\n";
  status = "Обработан";
  readyForDeliver.release();
}

void Order::deliverOrder() {
  std::cout << "Доставка заказа " << this->id << " начата\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(delieveryDelay));
  std::cout << "Доставка заказа " << this->id << " закончена\n";
  status = "Доставлен";
  readyForDeliver.release();
}

void Order::print() {
  std::cout << "Заказ " << this->id << " Статус - " << this->status
            << std::endl;
}

std::string Order::getStatus() { return status; }