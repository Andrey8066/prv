#include "VirtualThread.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <random>
#include <string>
#include <thread>

VirtualThread::VirtualThread(int threadId, int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> dist(1, 26);

  this->threadId = threadId;
  this->id = 0;
  std::cout << "[";
  for (int i = 0; i < n; i++) {
    char c = ('A' + (dist(gen)) - 1);
    std::string el = "Задача ";
    el.push_back(c);
    this->q.emplace_back(el);
    std::cout << el;
    if (i < n - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

void VirtualThread::runNextTask(bool &status) {
  if (id == q.size()) {
    status = false;
    return;
  }
  std::string a = q.at(id);
  int d = a.back();
  coutMutex.lock();
  std::cout << "Виртуальный поток " << this->threadId << " начал " << a
            << std::endl;
  coutMutex.unlock();
  std::this_thread::sleep_for(std::chrono::milliseconds(d));
  coutMutex.lock();
  std::cout << "Виртуальный поток " << this->threadId << " закончил " << a
            << std::endl;
  coutMutex.unlock();
  id++;
  status = true;
}