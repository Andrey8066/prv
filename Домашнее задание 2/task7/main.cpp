#include "HyperThreadingSimulator.hpp"
#include "iostream"
int main() {
  int n;
  std::cout << "Введите размер массива: ";
  std::cin >> n;
  HyperThreadingSimulator hts(n);
  hts.execute();
}