#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>

#include <iostream>

namespace asio = boost::asio;

using tcp = asio::ip::tcp;

using namespace asio::experimental::awaitable_operators;

asio::awaitable<std::string> read_from(tcp::socket &socket,
                                       const std::string &name) {
  char data[1024];

  auto [ec, n] = co_await socket.async_read_some(
      asio::buffer(data), asio::as_tuple(asio::use_awaitable));

  if (ec == asio::error::eof) {
    co_return name + ": connection closed\n";
  }

  if (ec) {
    throw boost::system::system_error(ec);
  }

  co_return name + ": " + std::string(data, n);
}

asio::awaitable<void> multiplexer(tcp::socket sock1, tcp::socket sock2) {
  try {
    for (;;) {
      auto result =
          co_await (read_from(sock1, "sock1") || read_from(sock2, "sock2"));

      std::visit([](auto &value) { std::cout << value << "\n"; }, result);
    }
  } catch (std::exception &e) {
    std::cout << "Ошибка multiplexer: " << e.what() << "\n";
  }
}

int main() {
  asio::io_context io;

  tcp::resolver resolver(io);

  auto ep1 = resolver.resolve("127.0.0.1", "1111");

  auto ep2 = resolver.resolve("127.0.0.1", "2222");

  tcp::socket sock1(io);
  tcp::socket sock2(io);

  asio::connect(sock1, ep1);
  asio::connect(sock2, ep2);

  asio::co_spawn(io, multiplexer(std::move(sock1), std::move(sock2)),
                 asio::detached);

  io.run();
}