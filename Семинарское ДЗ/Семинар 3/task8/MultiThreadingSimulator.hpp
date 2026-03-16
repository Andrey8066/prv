#include "VirtualThread.hpp"

class MultiThreadingSimulator {
private:
  VirtualThread vt1;
  VirtualThread vt2;
  void worker(VirtualThread &vt, int threadId);

public:
  MultiThreadingSimulator(int n);
  void execute();
};