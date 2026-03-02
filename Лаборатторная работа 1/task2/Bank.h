#pragma once

#include <string>

class Bank {
private:
  long long balance;
  int id;

public:
  Bank();
  void decrease(long long sum);
  void increase(long long sum);
  void randomOperation();
  long long getBalance();
  std::string print();
};
