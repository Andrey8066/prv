#include "Bank.h"
#include "BankAtomic.h"
#include "BankMutex.h"
#include <iostream>
#include <random>
#include <thread>
#include <threads.h>
#include <vector>

using namespace std;

template <typename T> void randomOperation(int N, T &bank) {
  mt19937 gen(random_device{}());
  uniform_int_distribution<long long> dist(0, 1000000);

  for (int i = 0; i < N; i++) {
    long long sum = dist(gen);
    if (sum % 2 == 1)
      bank.increase(sum);
    else
      bank.decrease(sum);
  }
}

// Вычисление времени выполнения процесса
template <typename T> int timeProcces(int M, int N, T &bank) {

  auto start = std::chrono::high_resolution_clock::now();

  vector<thread> threads;

  for (int i = 0; i < M; i++) {
    threads.emplace_back(randomOperation<T>, N, ref(bank));
  }

  for (auto &t : threads)
    t.join();

  auto end = std::chrono::high_resolution_clock::now();

  auto time = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "Операции заняли " << time.count() << endl;

  return time.count();
}

int main() {
  int N;
  int M;

  cout << "Введите количество транзакций от одного клиента: ";
  cin >> N;
  cout << "Введите число потоков: ";
  cin >> M;

  Bank bank;
  BankMutex bankMutex;
  BankAtomic bankAtomic;

  vector<thread> threadsMutex;
  vector<thread> threadsAtomic;

  int timeBank = timeProcces(M, N, bank);
  int timeBankMutex = timeProcces(M, N, bankMutex);
  int timeBankAtomic = timeProcces(M, N, bankAtomic);

  bank.print();
  cout << "Время выполнения bank: " << timeBank << endl;
  bankMutex.print();
  cout << "Время выполнения bankMutex: " << timeBankMutex << endl;
  bankAtomic.print();
  cout << "Время выполнения bankAtomic: " << timeBankAtomic << endl;

  return 0;
}
