#pragma once

#include "SimpleSemaphore.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

struct PrintJob {

  std::string doc;

  int priority;

  int timeout_ms;

  int job_id;

  bool interrupted = false;

  bool operator<(const PrintJob &other) const {

    return priority < other.priority;
  }
};

class PrinterQueue {
private:
  int n_printers;

  SimpleSemaphore semaphore;

  std::mutex queue_mutex;

  std::mutex cout_mutex;

  std::priority_queue<PrintJob> jobs;

  std::atomic<int> completed_jobs{0};

  std::atomic<int> interrupted_jobs{0};

public:
  explicit PrinterQueue(int printers)
      : n_printers(printers), semaphore(printers) {}

  void printJob(std::string doc, int priority, int timeout_ms, int job_id) {

    {
      std::lock_guard<std::mutex> lock(queue_mutex);

      jobs.push({doc, priority, timeout_ms, job_id, false});

      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[ADD JOB] id = " << job_id << " | priority = " << priority
                << std::endl;
    }

    process();
  }

  void process() {

    PrintJob job;

    {
      std::lock_guard<std::mutex> lock(queue_mutex);

      if (jobs.empty())
        return;

      job = jobs.top();

      jobs.pop();
    }

    bool acquired = semaphore.try_acquire_for(job.timeout_ms);

    if (!acquired) {

      {
        std::lock_guard<std::mutex> lock(queue_mutex);

        jobs.push(job);
      }

      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[TIMEOUT] job = " << job.job_id << " returned to queue"
                << std::endl;

      return;
    }

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[PRINT START] Thread " << std::this_thread::get_id()
                << " | job = " << job.job_id << " | priority = " << job.priority
                << std::endl;
    }

    int work_time = 1000 + rand() % 3000;

    for (int i = 0; i < work_time; i += 200) {

      std::this_thread::sleep_for(std::chrono::milliseconds(200));

      std::this_thread::yield();

      // случайное прерывание
      if (rand() % 20 == 0) {

        job.interrupted = true;

        interrupted_jobs++;

        {
          std::lock_guard<std::mutex> lock(queue_mutex);

          jobs.push(job);
        }

        {
          std::lock_guard<std::mutex> out(cout_mutex);

          std::cout << "[INTERRUPTED] job = " << job.job_id << std::endl;
        }

        semaphore.release();

        return;
      }
    }

    completed_jobs++;

    {
      std::lock_guard<std::mutex> out(cout_mutex);

      std::cout << "[PRINT COMPLETE] Thread " << std::this_thread::get_id()
                << " | job = " << job.job_id << std::endl;
    }

    semaphore.release();
  }

  int get_completed_jobs() const { return completed_jobs.load(); }

  int get_interrupted_jobs() const { return interrupted_jobs.load(); }
};