#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;

using tcp = asio::ip::tcp;

asio::awaitable<void> echo_session(tcp::socket sock) {
  char data[1024];

  try {
    for (;;) {
      auto [ec, n] = co_await sock.async_read_some(
          asio::buffer(data), asio::as_tuple(asio::use_awaitable));

      if (ec == asio::error::eof) {
        std::cout << "Клиент отключился.\n";

        co_return;
      }

      if (ec) {
        throw boost::system::system_error(ec);
      }

      co_await asio::async_write(sock, asio::buffer(data, n),
                                 asio::use_awaitable);
    }
  } catch (std::exception &e) {
    std::cout << "Ошибка сессии: " << e.what() << "\n";
  }
}

asio::awaitable<void> listener(uint16_t port) {
  auto executor = co_await asio::this_coro::executor;

  tcp::acceptor acceptor(executor, tcp::endpoint(tcp::v4(), port));

  std::cout << "Echo-сервер запущен на порту " << port << "\n";

  for (;;) {
    tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);

    std::cout << "Новый клиент подключился.\n";

    asio::co_spawn(executor, echo_session(std::move(socket)), asio::detached);
  }
}

int main() {
  try {
    asio::io_context io_context;

    asio::co_spawn(io_context, listener(12345), asio::detached);

    io_context.run();
  } catch (std::exception &e) {
    std::cout << "Ошибка: " << e.what() << "\n";
  }

  return 0;
}