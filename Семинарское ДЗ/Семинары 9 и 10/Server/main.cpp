#include <iostream>
#include <thread>
#include <vector>

#include <boost/asio.hpp>

#include "GlobalState.hpp"
#include "Server.hpp"

int main() {
  try {
    boost::asio::io_context io_context;

    global_strand = std::make_shared<
        boost::asio::strand<boost::asio::io_context::executor_type>>(
        io_context.get_executor());

    Server server(io_context, 12345);

    const int num_threads = 4;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
      threads.emplace_back([&io_context]() { io_context.run(); });
    }

    for (auto &thread : threads) {
      thread.join();
    }
  } catch (std::exception &e) {
    std::cerr << "Ошибка сервера: " << e.what() << "\n";
  }

  return 0;
}