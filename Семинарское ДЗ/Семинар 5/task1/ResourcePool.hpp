#include "atomic"
#include <condition_variable>
#include <iostream>
#include <queue>
#include <semaphore>
#include <thread>
#include <vector>

#pragma once

using namespace std;

struct WaitingThread {
  int priority;
  thread::id tid;

  bool operator<(const WaitingThread &other) const {
    return priority < other.priority;
  }
};

using namespace std;

template <typename T> class ResourcePool {
private:
  std::vector<T> resources;
  std::counting_semaphore<> semaphore;
  std::mutex mtx;
  std::condition_variable_any cv;

  std::atomic<int> failed_attempts;

  priority_queue<WaitingThread> waiting_queue;

public:
public:
  ResourcePool(const vector<T> &initial_resources)
      : resources(initial_resources), semaphore(initial_resources.size()) {}

  T acquire(int priority, int timeout_ms) {

    thread::id tid = this_thread::get_id();

    {
      lock_guard<mutex> lock(mtx);

      waiting_queue.push({priority, tid});

      cout << "[WAIT] Thread " << tid << " | priority = " << priority << endl;
    }

    auto start = chrono::steady_clock::now();

    while (true) {

      bool acquired =
          semaphore.try_acquire_for(chrono::milliseconds(timeout_ms));

      if (!acquired) {

        failed_attempts++;

        lock_guard<mutex> lock(mtx);

        cout << "[TIMEOUT] Thread " << tid << " | priority = " << priority
             << endl;

        throw runtime_error("Resource acquire timeout");
      }

      unique_lock<mutex> lock(mtx);

      if (!waiting_queue.empty() && waiting_queue.top().tid == tid) {

        waiting_queue.pop();

        T res = resources.back();
        resources.pop_back();

        cout << "[ACQUIRE] Thread " << tid << " | priority = " << priority
             << " | resource = " << res << endl;

        return res;
      }

      semaphore.release();

      lock.unlock();

      this_thread::yield();

      auto now = chrono::steady_clock::now();

      auto elapsed =
          chrono::duration_cast<chrono::milliseconds>(now - start).count();

      if (elapsed >= timeout_ms) {

        failed_attempts++;

        lock_guard<mutex> lock2(mtx);

        cout << "[FAILED] Thread " << tid << " | priority = " << priority
             << endl;

        throw runtime_error("Priority waiting timeout");
      }
    }
  }

  void release(T res) {

    {
      lock_guard<mutex> lock(mtx);

      resources.push_back(res);

      cout << "[RELEASE] Thread " << this_thread::get_id()
           << " | resource = " << res << endl;
    }

    semaphore.release();

    cv.notify_all();
  }

  void add_resource(T res) {

    {
      lock_guard<mutex> lock(mtx);

      resources.push_back(res);

      cout << "[ADD RESOURCE] " << res << endl;
    }

    semaphore.release();
  }

  void remove_resource() {

    lock_guard<mutex> lock(mtx);

    if (!resources.empty()) {

      T res = resources.back();

      resources.pop_back();

      cout << "[REMOVE RESOURCE] " << res << endl;
    }
  }

  int get_failed_attempts() const { return failed_attempts.load(); }
};