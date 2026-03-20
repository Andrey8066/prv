#include <atomic>
#include <cstddef>
class Worker {
public:
  Worker(size_t delay);
  void work(std::atomic<long long> &res);

private:
  size_t delay;
};