#include "Insertion.hpp"
#include <chrono>
#include <memory>
#include <random>
#include <thread>
#include <vector>

#define N 10

int main() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<size_t> dist(0, N - 1);

  std::vector<std::shared_ptr<Intersection>> inter;
  std::vector<std::thread> threads;

  for (size_t i = 0; i < N; i++) {
    std::shared_ptr<Intersection> intr = std::make_shared<Intersection>(i);
    inter.emplace_back(intr);
  }
  for (auto &i : inter) {
    threads.emplace_back(std::thread(&Intersection::run, i));
  }

  for (int i = 0; i < 10000000; i++) {
    char dirs[] = {'N', 'S', 'E', 'W'};
    inter[dist(mt)]->addCar(Car(i, dirs[i % 4], i % 124 == 4));

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  for (auto &t : threads) {
    t.join();
  }
}