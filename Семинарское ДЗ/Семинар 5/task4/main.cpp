#include "PrinterQueue.hpp"

int main() {

  srand(time(nullptr));

  PrinterQueue printerQueue(2);

  std::vector<std::thread> threads;

  for (int i = 0; i < 10; i++) {

    int priority = rand() % 10;

    threads.emplace_back([&, i, priority]() {
      printerQueue.printJob("document_" + std::to_string(i), priority, 2000, i);
    });

    threads.back().detach();

    std::this_thread::yield();
  }

  std::this_thread::sleep_for(std::chrono::seconds(15));

  std::cout << "\nCompleted jobs: " << printerQueue.get_completed_jobs()
            << std::endl;

  std::cout << "Interrupted jobs: " << printerQueue.get_interrupted_jobs()
            << std::endl;

  return 0;
}