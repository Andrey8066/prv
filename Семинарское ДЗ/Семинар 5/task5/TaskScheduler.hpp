#pragma once

#include "SimpleSemaphore.hpp"
#include "Task.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class TaskScheduler {
private:
  std::priority_queue<Task> tasks;

  SimpleSemaphore resource_semaphore;

  std::mutex queue_mutex;

  std::mutex cout_mutex;

  std::atomic<int> completed_tasks{0};

  std::atomic<long long> total_wait_time{0};

public:
  explicit TaskScheduler(int resources) : resource_semaphore(resources) {}

  void submit(Task task) {

    {
      std::lock_guard<std::mutex> lock(queue_mutex);

      tasks.push(task);
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[SUBMIT] task = " << task.id
                << " | priority = " << task.priority << std::endl;
    }
  }

  inline void execute_task(Task &task) { task.execute(); }

  void worker() {

    while (true) {

      Task task;

      {
        std::lock_guard<std::mutex> lock(queue_mutex);

        if (tasks.empty())
          return;

        task = tasks.top();

        tasks.pop();
      }

      auto start_wait = std::chrono::steady_clock::now();

      resource_semaphore.acquire(task.required_slots);

      auto end_wait = std::chrono::steady_clock::now();

      auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                           end_wait - start_wait)
                           .count();

      total_wait_time += wait_time;

      {
        std::lock_guard<std::mutex> out(cout_mutex);

        std::cout << "[START] Thread " << std::this_thread::get_id()
                  << " | task = " << task.id
                  << " | slots = " << task.required_slots
                  << " | available = " << resource_semaphore.available()
                  << std::endl;
      }

      execute_task(task);

      resource_semaphore.release(task.required_slots);

      completed_tasks++;

      {
        std::lock_guard<std::mutex> out(cout_mutex);

        std::cout << "[COMPLETE] Thread " << std::this_thread::get_id()
                  << " | task = " << task.id << std::endl;
      }

      std::this_thread::yield();
    }
  }

  int get_completed_tasks() const { return completed_tasks.load(); }

  double average_wait_time() const {

    int completed = completed_tasks.load();

    if (completed == 0)
      return 0;

    return static_cast<double>(total_wait_time.load()) / completed;
  }
};