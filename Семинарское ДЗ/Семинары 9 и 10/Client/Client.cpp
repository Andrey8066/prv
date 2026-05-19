#include "Client.hpp"

#include <iostream>

Client::Client(boost::asio::io_context &io_context) : socket_(io_context) {}

void Client::start(const tcp::resolver::results_type &endpoints) {
  do_connect(endpoints);
}

void Client::do_connect(const tcp::resolver::results_type &endpoints) {
  auto self(shared_from_this());

  boost::asio::async_connect(
      socket_, endpoints,

      [this, self](boost::system::error_code ec, tcp::endpoint) {
        if (!ec) {
          connected_ = true;

          std::cout << "Подключение к серверу успешно.\n";

          do_read(); // <-- запускаем постоянное чтение
        } else {
          std::cout << "Ошибка подключения: " << ec.message() << "\n";
        }
      });
}

void Client::send_message(const std::string &message) {
  if (!connected_) {
    std::cout << "Клиент ещё не подключился к серверу.\n";

    return;
  }

  auto self(shared_from_this());

  boost::asio::async_write(
      socket_, boost::asio::buffer(message),

      [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
          std::cout << "Координаты отправлены.\n";

          do_read();
        } else {
          std::cout << "Ошибка отправки: " << ec.message() << "\n";
        }
      });
}

void Client::do_read() {
  auto self(shared_from_this());

  socket_.async_read_some(
      boost::asio::buffer(data_, max_length),

      [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          std::cout << "\nЭхо-ответ сервера: " << std::string(data_, length)
                    << "\n";

          std::cout << "\nВведите координаты: " << std::flush;

          do_read();
        } else {
          std::cout << "\nОшибка чтения: " << ec.message() << "\n";
        }
      });
}