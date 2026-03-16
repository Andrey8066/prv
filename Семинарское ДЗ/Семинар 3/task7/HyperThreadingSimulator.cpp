#include "HyperThreadingSimulator.hpp"
#include "VirtualThread.hpp"
#include <functional>
#include <thread>

HyperThreadingSimulator::HyperThreadingSimulator(int n)
    : vt1(1, n), vt2(2, n) {}
void HyperThreadingSimulator::execute() {
  bool vt1Status = false;
  bool vt2Status = false;
  std::thread t1(&HyperThreadingSimulator::process, this, std::ref(vt1));
  std::thread t2(&HyperThreadingSimulator::process, this, std::ref(vt2));
  t1.join();
  t2.join();
}

void HyperThreadingSimulator::process(VirtualThread &vt) {
  bool vtStatus = true;
  while (vtStatus) {

    vt.runNextTask(vtStatus);
  }
}