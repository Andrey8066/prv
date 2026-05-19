#include "Session.hpp"
#include "GlobalState.hpp"

#include <iostream>

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() { do_read(); }

void Session::do_read() {
  auto self(shared_from_this());

  socket_.async_read_some(
      boost::asio::buffer(data_, max_length),

      boost::asio::bind_executor(
          *global_strand,

          [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
              std::string message(data_, length);

              std::cout << "\n====================================\n";
              std::cout << "Получено сообщение от беспилотника:\n";
              std::cout << message << "\n";

              ++global_message_count;

              std::cout << "Всего обработано сообщений: "
                        << global_message_count << "\n";
              std::cout << "====================================\n";

              do_write(length);
            } else {
              std::cout << "Ошибка чтения: " << ec.message() << "\n";
            }
          }));
}

void Session::do_write(std::size_t length) {
  auto self(shared_from_this());

  boost::asio::async_write(
      socket_, boost::asio::buffer(data_, length),

      boost::asio::bind_executor(
          *global_strand,

          [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
              do_read();
            } else {
              std::cout << "Ошибка отправки: " << ec.message() << "\n";
            }
          }));
}