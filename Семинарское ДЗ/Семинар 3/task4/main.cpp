#include "TaskTimer.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> generateVector(int n) {
  std::random_device rd;
  std::mt19937 m(rd());
  std::uniform_int_distribution<int> dist(0, n);

  std::vector<int> res;
  for (int i = 0; i < n; i++) {
    res.emplace_back(dist(m));
  }
  return res;
}

int main() {
  TaskTimer<std::chrono::nanoseconds> tt;
  std::vector<int> v = generateVector(100000);
  tt.measureSorting(v);
  std::cout << "Время выполнения сортировки: " << tt.getDuration().count()
            << " наносекунд" << std::endl;
}
