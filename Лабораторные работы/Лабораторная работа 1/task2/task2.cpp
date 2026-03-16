#include "Bank.h"
#include "BankAtomic.h"
#include "BankMutex.h"
#include <iostream>
#include <random>
#include <thread>
#include <threads.h>
#include <vector>

using namespace std;

// Функция используемая в потоках для взаимодействия с балансом
template <typename T>
void randomOperation(int n, int N, vector<long long> &control, T &bank) {
  mt19937 gen(random_device{}());
  uniform_int_distribution<long long> dist(0, 1000000);

  for (int i = 0; i < N; i++) {
    long long sum = dist(gen);
    if (sum % 2 == 1) {
      bank.increase(sum);
      control.at(n * N + i) = sum;
    } else {
      bank.decrease(sum);
      control.at(n * N + i) = -sum;
    }
  }
}

// Вычесление суммы в векторе
long long getSum(vector<long long> &control) {
  long long res = 0;
  for (long long x : control) {
    res += x;
  }
  return res;
}

// Вычисление времени выполнения процесса
template <typename T> int timeProcces(int M, int N, T &bank) {

  auto start = std::chrono::high_resolution_clock::now();

  vector<thread> threads;
  vector<long long> control(N * M);

  for (int i = 0; i < M; i++) {
    threads.emplace_back(randomOperation<T>, i, N, ref(control), ref(bank));
  }

  for (auto &t : threads)
    t.join();

  auto end = std::chrono::high_resolution_clock::now();

  auto time = chrono::duration_cast<chrono::microseconds>(end - start);

  cout << "\033[31mОперации заняли " << time.count() << endl;
  cout << "Баланс счета " << bank.getBalance() << endl;
  cout << "Ожидаемое изменение счета " << getSum(control) << "\033[0m" << endl;

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
