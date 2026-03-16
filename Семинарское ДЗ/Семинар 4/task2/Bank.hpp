#include "Account.hpp"
#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

template <typename T> class Bank {
private:
  std::vector<Account<T>> accounts;
  std::condition_variable cv;
  std::mutex cvMutex;

public:
  Bank(size_t n);
  bool transfer(int from, int to, T amount);
  T getBalance();
  inline bool doTransfer(int from, int to, T amount);
};

template <typename T>
inline bool Bank<T>::doTransfer(int from, int to, T amount) {
  Account<T> *fromAccount = &accounts.at(from);
  Account<T> *toAccount = &accounts.at(to);

  std::unique_lock<std::mutex> cvLock(cvMutex);

  cv.wait(cvLock, [&] { return fromAccount->getBalance() >= amount; });

  fromAccount->withdraw(amount);
  toAccount->deposit(amount);
}

template <typename T> bool Bank<T>::transfer(int from, int to, T amount) {
  std::thread t(&Bank<T>::doTransfer, this, from, to, amount);

  t.detach();
}

template <typename T> Bank<T>::Bank(size_t n) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dist(1, 10000000);
  accounts.reserve(n);
  for (int i = 0; i < n; i++) {
    Account<T> a(dist(gen));
    accounts.emplace_back(a);
  }
}

template <typename T> T Bank<T>::getBalance() {
  T res = 0;
  for (Account<T> &a : accounts) {
    res += a.getBalance();
  }
  return res;
}