#pragma once

#include "Account.hpp"
#include "Logger.hpp"
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>
class Bank {
public:
  Bank(size_t n);
  void transfer(size_t from, size_t to, long long amount);
  void do_transfer(size_t from, size_t to, long long amount);
  void print();

private:
  std::vector<std::unique_ptr<Account>> accounts;
  std::condition_variable cv;
  std::mutex mtx;
  Logger<std::string> logger;
};