#include "OrderProcessing.hpp"
#include <chrono>
#include <iostream>
#include <locale>

int main() {
  OrderProcessing op(100, 5, 3);
  std::chrono::time_point<std::chrono::high_resolution_clock> start =
      std::chrono::high_resolution_clock::now();
  op.process();
  std::chrono::time_point<std::chrono::high_resolution_clock> finish =
      std::chrono::high_resolution_clock::now();
  std::cout << "Обработка заняла "
            << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
                                                                     start)
            << " миллисекунд" << std::endl;
}