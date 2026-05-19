#ifndef BANKACCOUNT_HPP
#define BANKACCOUNT_HPP

#include <boost/asio.hpp>

#include <cstdint>

namespace asio = boost::asio;

class BankAccount {
public:
  explicit BankAccount(asio::io_context &io);

  asio::awaitable<void> async_deposit(int64_t amount);

  asio::awaitable<void> async_withdraw(int64_t amount);

  asio::awaitable<int64_t> async_get_balance();

  int64_t get_balance_sync() const;

private:
  int64_t balance_ = 0;

  asio::strand<asio::io_context::executor_type> strand_;
};

#endif