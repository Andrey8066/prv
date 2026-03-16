#include "Bank.hpp"
#include <iostream>
#include <random>
#include <thread>

int main() {
  size_t n;
  std::cout << "Введите количество потоков" << std::endl;
  std::cin >> n;
  Bank<long long> bank(n);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distTrans(1, n);
  std::uniform_int_distribution<long> distAmount(1, n);
  for (size_t i = 0; i < n; i++) {
    int a = distTrans(gen);
    int b = distTrans(gen);
    std::thread t([&]() { bank.doTransfer(a, b, distAmount(gen)); });
    t.detach();
  }

  std::cout << "Итоговый счет: " << bank.getBalance() << std::endl;
}