#include "server.hpp"
#include "task.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <semaphore>
#include <thread>

#define N 5

class Claster {
public:
  Claster() : cs(N) {

    for (size_t i = 0; i < N; i++) {
      auto server = std::make_shared<Server>();
      std::thread t(&Server::run, server);
      t.detach();
      servers.emplace(server);
    }

    std::thread t(&Claster::countStat, this);
    t.detach();
  }
  void run() {
    while (true) {
      Task t;
      {
        std::unique_lock<std::mutex> lock(qMutex);
        qcv.wait(lock, [this] { return !q.empty(); });
        t = q.top();
        q.pop();
      }

      std::cout << "Запущена задача c " << t.id << " приоритетом " << t.priority
                << " и задержкой " << t.delay << std::endl;

      cs.acquire();
      used.fetch_add(1);
      auto server = servers.front();
      servers.pop();
      server->addTask(t, [server, this, t] {
        servers.push(server);
        cs.release();
        used.fetch_sub(1);
        std::cout << "Завершена задача c  id " << t.id << " приоритетом "
                  << t.priority << " и задержкой " << t.delay << std::endl;
      });
    }
  }

  void addTask(const Task &task) {
    std::lock_guard<std::mutex> lock(qMutex);
    q.push(task);
    std::cout << "В очереди " << q.size() << " задач" << std::endl;
    qcv.notify_one();
  }

  void countStat() {
    size_t i = 0;
    double n = 0;

    while (true) {
      n += used.load();
      i++;
      if (i == 10) {
        if (n / (10 * serverNumbers) > 0.8) {
          addServer();
        }
        i = 0;
        n = 0;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }

  void addServer() {
    auto server = std::make_shared<Server>();
    std::thread t(&Server::run, server);
    t.detach();
    servers.push(server);

    cs.release();

    serverNumbers.fetch_add(1);
    std::cout << "Добавлен новый сервер, теперь серверов "
              << serverNumbers.load() << std::endl;
  }

private:
  std::priority_queue<Task> q;
  std::queue<std::shared_ptr<Server>> servers;
  std::counting_semaphore<> cs;
  std::mutex qMutex;
  std::condition_variable qcv;
  std::atomic<size_t> used;
  std::atomic<size_t> serverNumbers = N;
};