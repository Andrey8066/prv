#include "Server.hpp"
#include "Session.hpp"

#include <iostream>

Server::Server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
  std::cout << "Сервер запущен на порту " << port << "\n";

  do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept(

      boost::asio::make_strand(acceptor_.get_executor()),

      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          std::cout << "Новый беспилотник подключился.\n";

          std::make_shared<Session>(std::move(socket))->start();
        } else {
          std::cout << "Ошибка подключения: " << ec.message() << "\n";
        }

        do_accept();
      });
}