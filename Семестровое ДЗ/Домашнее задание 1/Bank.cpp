#include "Bank.hpp"
#include "Account.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

void Bank::transfer(size_t from, size_t to, long long amount) {
  std::thread t(&Bank::do_transfer, this, from, to, amount);
  t.detach();
}
void Bank::do_transfer(size_t from, size_t to, long long amount) {
  if (from > accounts.size() || to > accounts.size() - 1)
    throw std::runtime_error("Все сломалось, from=" + std::to_string(from) +
                             "to=" + std::to_string(to));

  Account &fromAccount = *accounts[from];
  Account &toAccount = *accounts[to];

  auto timeout = std::chrono::seconds(5);
  auto start = std::chrono::steady_clock::now();

  std::unique_lock<std::mutex> lock(mtx);

  while (fromAccount.getBalance() < amount) {
    if (cv.wait_for(lock, std::chrono::milliseconds(50)) ==
        std::cv_status::timeout) {
      std::this_thread::yield();
    }
    if (std::chrono::steady_clock::now() - start >= timeout) {
      std::stringstream ss;
      ss << "Перевод отменен: недостаточно средств на счете " << from
         << ", баланс " << fromAccount.getBalance();
      logger.log(ss.str());
      return;
    }
  }
  {
    fromAccount.withdraw(amount);
    toAccount.deposit(amount);
    std::stringstream ss;
    ss << "Перевод " << amount << " со счета " << from << " на счет " << to
       << ", в потоке " << std::this_thread::get_id();
    logger.log(ss.str());
  }
  lock.unlock();
  cv.notify_all();
}

Bank::Bank(size_t n) : logger("Bank.log") {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> dist(1, 100000000);
  for (size_t i = 0; i < n; i++) {
    accounts.emplace_back(std::make_unique<Account>(dist(gen)));
  }
  print();
}

void Bank::print() {
  for (int i = 0; i < accounts.size(); i++) {
    std::stringstream ss;
    ss << "Счёт №" << i << " имеет баланс " << accounts[i]->getBalance();
    logger.log(ss.str());
  }
}