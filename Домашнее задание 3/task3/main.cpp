#include "Buffer.hpp"

int main() {

  const size_t BUFFER_SIZE = 5;
  const int ITEMS_TO_PRODUCE = 10;

  Buffer<int> buf(BUFFER_SIZE);

  auto producer = [&](int start) {
    for (int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
      buf.produce(start + i);
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };

  auto consumer = [&]() {
    for (int i = 0; i < ITEMS_TO_PRODUCE; ++i) {
      buf.consume();
      std::this_thread::yield();
      std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
  };

  std::thread p1(producer, 100);
  std::thread p2(producer, 200);
  std::thread c1(consumer);
  std::thread c2(consumer);

  p1.join();
  p2.join();
  c1.join();
  c2.join();

  return 0;
}