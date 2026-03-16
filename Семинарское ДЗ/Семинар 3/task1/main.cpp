#include <chrono>
#include <iostream>

int sum(int n) {
  int res = 0;
  for (int i = 1; i <= n; i++) {
    res += i;
  }
  return res;
}

int main() {
  int n;
  std::cout << "Введите N : ";
  std::cin >> n;
  std::chrono::time_point start = std::chrono::high_resolution_clock::now();
  sum(n);
  std::chrono::time_point finish = std::chrono::high_resolution_clock::now();

  std::cout << "Время вычисления "
            << std::chrono::duration_cast<std::chrono::nanoseconds>(finish -
                                                                    start)
                   .count()
            << " наносекунд" << std::endl;
  return 0;
}