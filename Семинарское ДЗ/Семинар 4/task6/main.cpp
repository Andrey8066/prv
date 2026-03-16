#include "MatrixProcessor.hpp"

template <typename T> T increment(T &a) {
  a = a + 10;
  return a;
}
int main(int, char **) {
  MatrixProcessor<int> m(10, 10, 4);
  m.print();
  std::cout << "----------------------------" << std::endl;
  m.apply(increment<int>);
  m.print();
}
