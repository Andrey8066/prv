#include "BankAccount.hpp"

#include <stdexcept>

BankAccount::BankAccount(asio::io_context &io) : strand_(io.get_executor()) {}

asio::awaitable<void> BankAccount::async_deposit(int64_t amount) {
  co_await asio::post(strand_, asio::use_awaitable);

  balance_ += amount;

  co_return;
}

asio::awaitable<void> BankAccount::async_withdraw(int64_t amount) {
  co_await asio::post(strand_, asio::use_awaitable);

  if (balance_ < amount) {
    throw std::invalid_argument("Insufficient funds");
  }

  balance_ -= amount;
}

asio::awaitable<int64_t> BankAccount::async_get_balance() {
  co_await asio::post(strand_, asio::use_awaitable);

  co_return balance_;
}

int64_t BankAccount::get_balance_sync() const { return balance_; }