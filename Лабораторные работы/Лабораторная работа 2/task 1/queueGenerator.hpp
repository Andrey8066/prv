#include "claster.hpp"
#include "task.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

class QueueGenerator {
public:
  void run() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<long long> delayDist(1000, 10000);
    std::uniform_int_distribution<size_t> priorityDist(0, 5);
    long long i;

    while (true) {
      Task t(i, std::chrono::milliseconds(delayDist(mt)), priorityDist(mt));
      i++;
      c->addTask(t);

      std::cout << "Добавлена задача c " << t.id << " приоритетом "
                << t.priority << " и задержкой " << t.delay << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  QueueGenerator(std::shared_ptr<Claster> claster) : c(claster) {}

private:
  std::shared_ptr<Claster> c;
};