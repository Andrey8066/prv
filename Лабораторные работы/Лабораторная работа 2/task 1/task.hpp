#pragma once
#include <chrono>

struct Task {
  long long id;
  std::chrono::milliseconds delay;
  size_t priority;
  Task(long long i, std::chrono::milliseconds d, size_t p)
      : id(i), delay(d), priority(p) {};
  Task() {}
  bool operator<(const Task &other) const { return priority > other.priority; }
};