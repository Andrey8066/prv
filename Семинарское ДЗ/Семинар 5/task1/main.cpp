#include "ResourcePool.hpp"
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int main() {

  vector<int> initial_resources = {1, 2, 3};

  ResourcePool<int> pool(initial_resources);

  mutex cout_mutex;

  auto worker = [&](int priority, int timeout_ms) {
    try {

      int res = pool.acquire(priority, timeout_ms);

      this_thread::sleep_for(chrono::milliseconds(500 + rand() % 1000));

      this_thread::yield();

      pool.release(res);

    } catch (const exception &e) {

      lock_guard<mutex> lock(cout_mutex);

      cout << "[EXCEPTION] Thread " << this_thread::get_id() << " | "
           << e.what() << endl;
    }
  };

  vector<thread> threads;

  for (int i = 0; i < 10; i++) {

    int priority = rand() % 10;

    threads.emplace_back([&, priority]() { worker(priority, 2000); });

    threads.back().detach();

    this_thread::yield();
  }

  this_thread::sleep_for(chrono::seconds(8));

  cout << "\nFailed attempts: " << pool.get_failed_attempts() << endl;

  return 0;
}