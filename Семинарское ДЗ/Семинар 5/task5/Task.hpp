#pragma once

#include <chrono>
#include <iostream>
#include <thread>

class Task {
public:
  int id;

  int required_slots;

  int duration_ms;

  int priority;

  Task() = default;

  Task(int id, int slots, int duration, int priority)
      : id(id), required_slots(slots), duration_ms(duration),
        priority(priority) {}

  void execute() {

    std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
  }

  bool operator<(const Task &other) const { return priority < other.priority; }
};