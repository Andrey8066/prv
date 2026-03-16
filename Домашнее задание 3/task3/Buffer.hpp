

#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

#pragma once
template <typename T> class Buffer {
public:
  Buffer<T>(size_t c) : capacity(c) {}
  void produce(T value) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return buffer.size() < capacity; });
    buffer.push_back(value);
    std::cout << "Produced: " << value << " by thread "
              << std::this_thread::get_id() << "\n";
    cv.notify_all();
  }
  T consume() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return !buffer.empty(); });
    T value = buffer.back();
    buffer.pop_back();
    std::cout << "Consumed: " << value << " by thread "
              << std::this_thread::get_id() << "\n";
    cv.notify_all();
    return value;
  }

private:
  std::vector<T> buffer;
  size_t capacity;
  std::mutex mtx;
  std::condition_variable cv;
};