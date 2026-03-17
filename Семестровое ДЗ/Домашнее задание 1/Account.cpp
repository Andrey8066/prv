#include "Account.hpp"
#include <mutex>

Account::Account(long long balance) { this->balance = balance; }

long long Account::getBalance() {
  std::lock_guard<std::mutex> lock(mtx);

  return this->balance;
}
void Account::deposit(long long amount) {
  std::lock_guard<std::mutex> lock(mtx);
  balance += amount;
}

void Account::withdraw(long long amount) {
  std::lock_guard<std::mutex> lock(mtx);
  balance -= amount;
}
