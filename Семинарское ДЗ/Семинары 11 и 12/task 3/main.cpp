#include "BankAccount.hpp"

#include <boost/asio.hpp>

#include <atomic>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

namespace asio = boost::asio;

asio::awaitable<void> worker(BankAccount &account, std::atomic<int> &done,
                             std::atomic<int64_t> &expected) {
  try {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> dist(1, 100);

    for (int i = 0; i < 10; ++i) {
      int value = dist(gen);

      expected += value;

      co_await account.async_deposit(value);
    }

    for (int i = 0; i < 10; ++i) {
      int value = dist(gen);

      try {
        co_await account.async_withdraw(value);

        expected -= value;
      } catch (...) {
      }
    }
  } catch (...) {
  }

  ++done;

  co_return;
}
int main() {
  asio::io_context io;

  BankAccount account(io);

  const int workers_count = 100;

  std::atomic<int64_t> expected = 0;

  std::atomic<int> done = 0;

  for (int i = 0; i < workers_count; ++i) {
    asio::co_spawn(io,

                   worker(account, done, expected),

                   asio::detached);
  }

  std::vector<std::thread> threads;

  for (int i = 0; i < 4; ++i) {
    threads.emplace_back([&io]() { io.run(); });
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Ожидаемый баланс: " << expected.load() << "\n";

  std::cout << "Финальный баланс: " << account.get_balance_sync() << "\n";

  return 0;
}