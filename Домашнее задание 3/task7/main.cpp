#include "PriorityQueue.hpp"
#include <thread>

int main() {

  PriorityQueue<int> pq;

  auto producer = [&pq](int start) {
    for (int i = 0; i < 10; i++) {

      pq.push(start + i, rand() % 10);

      std::this_thread::yield();
    }
  };

  auto consumer = [&pq]() {
    for (int i = 0; i < 10; i++) {

      pq.pop();

      std::this_thread::yield();
    }
  };

  std::thread(
      [](PriorityQueue<int> *q) {
        for (int i = 0; i < 10; i++) {
          q->push(i, rand() % 10);
          std::this_thread::yield();
        }
      },
      &pq)
      .detach();

  std::thread(
      [](PriorityQueue<int> *q) {
        for (int i = 0; i < 10; i++) {
          q->pop();
          std::this_thread::yield();
        }
      },
      &pq)
      .detach();

  std::thread p1(producer, 100);
  std::thread p2(producer, 200);

  std::thread c1(consumer);
  std::thread c2(consumer);

  p1.join();
  p2.join();
  c1.join();
  c2.join();
}