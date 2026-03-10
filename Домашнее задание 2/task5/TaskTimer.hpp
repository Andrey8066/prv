
#include <chrono>
#include <vector>

template <typename T>

class TaskTimer {
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
  void start();
  void stop();

public:
  template <typename func>
  std::vector<int> measureSorting(std::vector<int> v, func f);
  T getDuration();
};

template <typename T>
template <typename func>
std::vector<int> TaskTimer<T>::measureSorting(std::vector<int> v, func f) {
  TaskTimer<T>::start();
  f(v);
  TaskTimer<T>::stop();
  return v;
};

template <typename T> T TaskTimer<T>::getDuration() {
  return std::chrono::duration_cast<T>(endTime - startTime);
}

template <typename T> void TaskTimer<T>::start() {
  this->startTime = std::chrono::high_resolution_clock::now();
}

template <typename T> void TaskTimer<T>::stop() {
  this->endTime = std::chrono::high_resolution_clock::now();
};
