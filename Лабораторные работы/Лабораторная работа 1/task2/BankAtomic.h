#pragma once

#include <atomic>
#include <string>

class BankAtomic {
private:
  std::atomic<long long> balance;
  int id;

public:
  BankAtomic();
  void decrease(long long sum);
  void increase(long long sum);
  void randomOperation();
  long long getBalance();
  std::string print();
};
