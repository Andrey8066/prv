#include <queue>

#pragma once
class VirtualThread {
public:
  VirtualThread(int n);
  long long run();

private:
  std::queue<int> q;
  long long fact(long long a);
  void print();
};