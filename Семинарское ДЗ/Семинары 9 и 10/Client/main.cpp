#include <iostream>
#include <thread>

#include <boost/asio.hpp>

#include "Client.hpp"

int main() {
  try {
    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);

    auto endpoints = resolver.resolve("127.0.0.1", "12345");

    auto client = std::make_shared<Client>(io_context);

    client->start(endpoints);

    std::thread t([&io_context]() { io_context.run(); });

    std::string message;

    while (true) {
      std::cout << "\nВведите координаты: ";

      std::getline(std::cin, message);

      if (message.empty()) {
        break;
      }

      client->send_message(message);
    }

    io_context.stop();

    t.join();
  } catch (std::exception &e) {
    std::cerr << "Ошибка клиента: " << e.what() << "\n";
  }

  return 0;
}