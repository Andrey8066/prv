#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <ostream>
#include <random>
#include <thread>
#include <type_traits>
#include <vector>

#pragma once

template <typename T> class ParallelSum {
private:
  std::vector<T> vT;
  std::size_t n;
  std::size_t finishedThreadsCount;
  std::mutex mtx;
  std::mutex finishedThreadsCountMtx;
  std::condition_variable cv;
  T res;
  inline T sum_segment(size_t l, size_t r);

public:
  void print(std::vector<T> v);
  ParallelSum<T>(size_t n);
  T computeSum();
};

template <typename T> ParallelSum<T>::ParallelSum(size_t n) {
  std::random_device rd;
  std::mt19937 gen(rd());

  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(1, 1000);
    for (size_t i = 0; i < dist(gen) * n; i++) {
      vT.emplace_back(dist(gen));
    }
  } else {
    std::uniform_real_distribution<T> dist(1, 1000);
    for (size_t i = 0; i < dist(gen) * n; i++) {
      vT.emplace_back(dist(gen));
    }
  }

  this->n = n;
  this->finishedThreadsCount = 0;
  this->res = 0;

  print(vT);
}

template <typename T> T ParallelSum<T>::computeSum() {
  size_t s = vT.size();
  std::vector<std::thread> threadVector;
  for (size_t i = 0; i < this->n; i++) {
    size_t l = i * s / n;
    size_t r = (i + 1) * s / n;
    std::thread t([this, l, r]() {
      T sum = sum_segment(l, r);
      mtx.lock();
      res += sum;
      std::cout << "Поток " << std::this_thread::get_id()
                << " рассчитал значение " << sum << ", результат " << res
                << std::endl;
      mtx.unlock();

      finishedThreadsCountMtx.lock();
      finishedThreadsCount += 1;
      finishedThreadsCountMtx.unlock();
      cv.notify_one();
    });
    t.detach();
  }

  std::unique_lock<std::mutex> lock(finishedThreadsCountMtx);
  cv.wait(lock, [this]() { return finishedThreadsCount == n; });
  return res;
}

template <typename T> inline T ParallelSum<T>::sum_segment(size_t l, size_t r) {
  T s = 0;
  for (int i = l; i < r; i++)
    s += vT[i];
  return s;
}

template <typename T> void ParallelSum<T>::print(std::vector<T> v) {
  std::cout << "[";
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << v[i];
    if (i != v.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]" << std::endl;
}