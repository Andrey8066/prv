#include "Bank.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <thread>

#define clients 10
#define threadsNum 5
#define operationPerThread 100

void doRandomOperation(Bank &bank) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> amountDist(1, 1000000);
  std::uniform_int_distribution<size_t> accountDist(0, clients - 1);

  for (int i = 0; i < operationPerThread; i++) {

    bank.do_transfer(accountDist(gen), accountDist(gen), amountDist(gen));
  }
}

int main() {
  Bank bank(clients);
  std::vector<std::thread> threads;
  for (int i = 0; i < threadsNum; i++) {

    threads.emplace_back(std::thread(doRandomOperation, std::ref(bank)));
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> start =
      std::chrono::high_resolution_clock::now();
  for (auto &t : threads) {
    t.join();
  }
  std::chrono::time_point<std::chrono::high_resolution_clock> finish =
      std::chrono::high_resolution_clock::now();

  auto dur =
      std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
  std::cout << "Процесс занял " << dur.count() << "миллисекунд" << std::endl;
  bank.print();
  return 0;
}