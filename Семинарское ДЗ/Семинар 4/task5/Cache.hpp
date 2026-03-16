#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>

#pragma once
template <typename Key, typename Value> class Cache {
private:
  std::map<Key, Value> data;
  std::mutex mtx;
  std::condition_variable cv;

public:
  inline void set(const Key &key, const Value &value) {
    std::lock_guard<std::mutex> lock(mtx);
    { data[key] = value; }
    std::cout << "Поток " << std::this_thread::get_id() << ", вставил ключ "
              << key << ", значение " << value << std::endl;
    cv.notify_all();
  }
  inline Value get(const Key &key) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return data.find(key) != data.end(); });
    std::cout << "Поток " << std::this_thread::get_id() << ", считал ключ "
              << key << ", значение " << data.at(key) << std::endl;
    return data.at(key);
  }
};
