#include "ParallelSum.hpp"
#include <iostream>

int main() {
  ParallelSum<long long> pS(4);
  std::cout << pS.computeSum() << std::endl;
}