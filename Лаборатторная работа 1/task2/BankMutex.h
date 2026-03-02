#pragma once

#include <mutex>
#include <string>

class BankMutex {
private:
  std::mutex mtx;
  static std::mutex coutMtx;
  long long balance;
  int id;

public:
  BankMutex();
  void decrease(long long sum);
  void increase(long long sum);
  void randomOperation();
  long long getBalance();
  std::string print();
};
