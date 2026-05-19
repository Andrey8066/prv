#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class SimpleSemaphore {
private:
  std::mutex mtx;

  std::condition_variable cv;

  int permits;

public:
  explicit SimpleSemaphore(int count = 0) : permits(count) {}

  bool try_acquire_for(int timeout_ms) {

    std::unique_lock<std::mutex> lock(mtx);

    bool success = cv.wait_for(lock, std::chrono::milliseconds(timeout_ms),
                               [&]() { return permits > 0; });

    if (!success)
      return false;

    permits--;

    return true;
  }

  void release() {

    {
      std::lock_guard<std::mutex> lock(mtx);

      permits++;
    }

    cv.notify_one();
  }
};

template <typename T> class SemaphoreBuffer {
private:
  std::vector<std::vector<T>> buffers;

  std::vector<std::unique_ptr<SimpleSemaphore>> empty;

  std::vector<std::unique_ptr<SimpleSemaphore>> full;

  std::vector<std::unique_ptr<std::mutex>> mtx;

  std::mutex cout_mutex;

  std::atomic<int> timeout_count{0};

public:
  SemaphoreBuffer(int num_buffers, int capacity) {

    buffers.resize(num_buffers);

    for (int i = 0; i < num_buffers; i++) {

      empty.push_back(std::make_unique<SimpleSemaphore>(capacity));

      full.push_back(std::make_unique<SimpleSemaphore>(0));

      mtx.push_back(std::make_unique<std::mutex>());
    }
  }

  void produce(T value, int buffer_index, int timeout_ms) {

    auto tid = std::this_thread::get_id();

    bool acquired = empty[buffer_index]->try_acquire_for(timeout_ms);

    if (!acquired) {

      timeout_count++;

      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[PRODUCER TIMEOUT] Thread " << tid
                << " | buffer = " << buffer_index << std::endl;

      return;
    }

    {
      std::lock_guard<std::mutex> lock(*mtx[buffer_index]);

      buffers[buffer_index].push_back(value);
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[PRODUCE] Thread " << tid << " | buffer = " << buffer_index
                << " | value = " << value << std::endl;
    }

    full[buffer_index]->release();

    std::this_thread::yield();
  }

  T consume(int buffer_index, int timeout_ms) {

    auto tid = std::this_thread::get_id();

    bool acquired = full[buffer_index]->try_acquire_for(timeout_ms);

    if (!acquired) {

      timeout_count++;

      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[CONSUMER TIMEOUT] Thread " << tid
                << " | buffer = " << buffer_index << std::endl;

      return T{};
    }

    T value;

    {
      std::lock_guard<std::mutex> lock(*mtx[buffer_index]);

      value = buffers[buffer_index].back();

      buffers[buffer_index].pop_back();
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[CONSUME] Thread " << tid << " | buffer = " << buffer_index
                << " | value = " << value << std::endl;
    }

    empty[buffer_index]->release();

    std::this_thread::yield();

    return value;
  }

  int get_timeout_count() const { return timeout_count.load(); }

  int get_buffer_count() const { return buffers.size(); }
};