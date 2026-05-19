#include "TaskScheduler.hpp"

int main() {

  srand(time(nullptr));

  TaskScheduler scheduler(5);

  for (int i = 0; i < 10; i++) {

    Task task(i, 1 + rand() % 3, 1000 + rand() % 4000, rand() % 10);

    scheduler.submit(task);
  }

  std::vector<std::thread> workers;

  for (int i = 0; i < 4; i++) {

    workers.emplace_back([&]() { scheduler.worker(); });

    workers.back().detach();

    std::this_thread::yield();
  }

  std::this_thread::sleep_for(std::chrono::seconds(20));

  std::cout << "\nCompleted tasks: " << scheduler.get_completed_tasks()
            << std::endl;

  std::cout << "Average wait time: " << scheduler.average_wait_time() << " ms"
            << std::endl;

  return 0;
}