#include "MultiThreadingSimulator.hpp"
#include <iostream>

int main(int, char **) {
  MultiThreadingSimulator m(10);
  std::cout << 100;
  m.execute();
}
