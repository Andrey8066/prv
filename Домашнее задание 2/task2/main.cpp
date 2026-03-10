#include <chrono>
#include <iostream>
#include <thread>
int main() {
  int n;
  std::cout << "Введите n: ";
  std::cin >> n;

  while (n > 0) {
    std::cout << "Осталось: " << n << " секунд" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    n--;
  }
  std::cout << "Врем вышло!" << std::endl;
}