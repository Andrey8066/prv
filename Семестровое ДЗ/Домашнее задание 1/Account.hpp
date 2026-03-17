#include <mutex>

#pragma once

class Account {
public:
  Account(long long balance);
  long long getBalance();
  void deposit(long long amount);
  void withdraw(long long amount);
  void print();

private:
  long long balance;
  std::mutex mtx;
};