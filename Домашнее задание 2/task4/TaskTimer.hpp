#include <algorithm>
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
  std::vector<int> measureSorting(std::vector<int> v);
  T getDuration();
};

template <typename T>
std::vector<int> TaskTimer<T>::measureSorting(std::vector<int> v) {
  TaskTimer<T>::start();
  std::sort(v.begin(), v.end());
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
