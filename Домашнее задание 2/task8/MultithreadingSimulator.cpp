#include "MultiThreadingSimulator.hpp"
#include <thread>

MultiThreadingSimulator::MultiThreadingSimulator(int n) : vt1(n), vt2(n) {}
void MultiThreadingSimulator::execute() {
  std::thread t1([&]() { worker(vt1, 1); });
  std::thread t2([&]() { worker(vt2, 2); });
  t1.join();
  t2.join();
}

void MultiThreadingSimulator::worker(VirtualThread &vt, int threadId) {
  while (vt.hasTask()) {
    vt.runStep(threadId);
  }
}
