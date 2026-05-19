#include "SemaphoreBuffer.hpp"

int main() {

  srand(time(nullptr));

  SemaphoreBuffer<int> buffer(3, 5);

  std::vector<std::thread> threads;

  // PRODUCERS
  for (int i = 0; i < 5; i++) {

    threads.emplace_back([&]() {
      for (int j = 0; j < 10; j++) {

        int random_buffer = rand() % buffer.get_buffer_count();

        int value = rand() % 100;

        buffer.produce(value, random_buffer, 1000);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100 + rand() % 400));
      }
    });

    threads.back().detach();
  }

  // CONSUMERS
  for (int i = 0; i < 5; i++) {

    threads.emplace_back([&]() {
      for (int j = 0; j < 10; j++) {

        int random_buffer = rand() % buffer.get_buffer_count();

        buffer.consume(random_buffer, 1000);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100 + rand() % 500));
      }
    });

    threads.back().detach();
  }

  std::this_thread::sleep_for(std::chrono::seconds(10));

  std::cout << "\nTimeouts: " << buffer.get_timeout_count() << std::endl;

  return 0;
}