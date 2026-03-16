#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
template <typename T> class PriorityQueue {
public:
  void push(T value, size_t priority) {
    {
      std::lock_guard<std::mutex> lovk(mtx);
      q.push(Item(value, priority));
      std::cout << "вставлено: " << value << " priority=" << priority
                << " thread=" << std::this_thread::get_id() << std::endl;
    }
    cv.notify_one();
  }
  T pop() {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [&] { return !q.empty(); });

    Item item = q.top();
    q.pop();

    std::cout << "Извлечено: " << item.value << " priority=" << item.priority
              << " thread=" << std::this_thread::get_id() << std::endl;

    return item.value;
  }

private:
  struct Item {
    size_t priority;
    T value;
    bool operator<(const Item &other) const {
      return priority < other.priority;
    }
    Item(T v, size_t p) {
      value = v;
      priority = p;
    }
  };
  std::priority_queue<Item> q;
  std::mutex mtx;
  std::condition_variable cv;
};