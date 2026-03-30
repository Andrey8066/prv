#include "task.hpp"
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class Server {
public:
  void doTask(Task &task) {
    std::this_thread::sleep_for(task.delay);
    activeTasks.pop_back();
    onFinish();
  }
  Server() {}
  void run() {
    while (true) {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this] { return !activeTasks.empty(); });
      doTask(activeTasks.front());
    }
  }
  bool getStatus() { return activeTasks.size() > 0; }
  bool operator<(Server &other) { return activeTasks.size() > 0; }
  void addTask(Task t, std::function<void()> onFinish) {
    activeTasks.push_back(t);
    cv.notify_one();
    this->onFinish = onFinish;
  }

private:
  std::vector<Task> activeTasks;
  std::condition_variable cv;
  std::mutex mtx;
  std::function<void()> onFinish;
  bool running = true;
};