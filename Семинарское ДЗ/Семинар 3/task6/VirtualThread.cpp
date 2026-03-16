#include "VirtualThread.hpp"
#include <iostream>
#include <random>

VirtualThread::VirtualThread(int n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> dist(1, 20);
  std::cout << "[";
  for (int i = 0; i < n; i++) {
    long long el = dist(gen);
    this->q.push(el);
    std::cout << el;
    if (i < n - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}

long long VirtualThread::run() {
  if (q.empty())
    return 0;
  long long a = q.front();
  q.pop();
  return fact(a);
}

long long VirtualThread::fact(long long a) {
  if (a == 1 || a == 0)
    return 1;
  return a * fact(a - 1);
}